// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Includes ----------------------------------------------------------*/
#include "gridConfig.h"
#ifdef WITH_SILO
#  include "gridWriterSilo.h"
#  include <assert.h>
#  include <string.h>
#  include <silo.h>
#  ifdef WITH_MPI
#    include <mpi.h>
#    include <pmpio.h>
#  endif
#  include "../libutil/xmem.h"
#  include "../libutil/xstring.h"
#  include "../libutil/diediedie.h"


/*--- Implemention of main structure ------------------------------------*/
#  include "gridWriterSilo_adt.h"


/*--- Local defines -----------------------------------------------------*/
#  define LOCAL_FILESUFFIX ".silo"
#  ifdef WITH_MPI
#    define LOCAL_DIRPREFIX     "domain_"
#    define LOCAL_NUMFILEDIGITS 3
#    define LOCAL_NUMDIRDIGITS  5
#  endif

/*--- Prototypes of local functions -------------------------------------*/
static void *
local_createDB(const char *fname, const char *dname, void *udata);


#  ifdef WITH_MPI
static void *
local_openDB(const char     *fname,
             const char     *dname,
             PMPIO_iomode_t iomode,
             void           *udata);

#  endif

static void
local_closeDB(void *file, void *udata);

static void
local_createFileName(gridWriterSilo_t writer);

static void
local_createDirName(gridWriterSilo_t writer);

static char *
local_getGridName(gridWriterSilo_t writer, const char *base);

static char **
local_getPatchNames(const char *base, int numPatches);

static void
local_delPatchNames(char **patchNames, int numPatches);

static void
local_writePatchData(gridWriterSilo_t writer,
                     gridPatch_t      patch,
                     const char       *patchName);

inline static int
local_getVarType(gridVar_t var);

inline static int
local_getVarCentering(gridVar_t var);

static void *
local_getPatchVarName(gridVar_t var, const char *patchName, char *varName);


/*--- Implementations of exported functios ------------------------------*/
extern gridWriterSilo_t
gridWriterSilo_new(const char *prefix, int dbType)
{
	gridWriterSilo_t writer;

	assert(prefix != NULL);
	assert(dbType == DB_HDF5 || dbType == DB_PDB);

	writer              = xmalloc(sizeof(struct gridWriterSilo_struct));
	writer->prefix      = xstrdup(prefix);
	writer->dbType      = dbType;
	writer->isActive    = false;
	writer->f           = NULL;
	writer->fileName    = NULL;
	writer->dirName     = NULL;
#  ifdef WITH_MPI
	writer->baton       = NULL;
	writer->groupRank   = -1;
	writer->rankInGroup = -1;
	writer->globalRank  = -1;
#  endif

	return writer;
}

extern void
gridWriterSilo_del(gridWriterSilo_t *writer)
{
	assert(writer != NULL && *writer != NULL);

	if ((*writer)->isActive)
		gridWriterSilo_deactivate(*writer);

	if ((*writer)->fileName != NULL)
		xfree((*writer)->fileName);
	if ((*writer)->dirName != NULL)
		xfree((*writer)->dirName);
	xfree((*writer)->prefix);
#  ifdef WITH_MPI
	if ((*writer)->baton != NULL)
		PMPIO_Finish((*writer)->baton);
#  endif
	xfree(*writer);

	*writer = NULL;
}

extern void
gridWriterSilo_activate(gridWriterSilo_t writer)
{
	assert(writer != NULL);
#  ifdef WITH_MPI
	assert(writer->baton != NULL);
#  endif

	if (!writer->isActive) {
		local_createFileName(writer);
		local_createDirName(writer);
#  ifdef WITH_MPI
		writer->f = PMPIO_WaitForBaton(writer->baton, writer->fileName,
		                               writer->dirName);
#  else
		writer->f = local_createDB(writer->fileName, writer->dirName,
		                           writer);
#  endif
		writer->isActive = true;
	}
}

extern void
gridWriterSilo_deactivate(gridWriterSilo_t writer)
{
	assert(writer != NULL);
#  ifdef WITH_MPI
	assert(writer->baton != NULL);
#  endif

	if (writer->isActive) {
#  ifdef WITH_MPI
		PMPIO_HandOffBaton(writer->baton, writer->f);
#  else
		local_closeDB(writer->f, writer);
#  endif
		writer->isActive = false;
	}
}

extern void
gridWriterSilo_writeGridPatch(gridWriterSilo_t writer,
                              gridPatch_t      patch,
                              const char       *patchName,
                              gridPointDbl_t   origin,
                              gridPointDbl_t   delta)
{
	double            *coords[NDIM];
	gridPointInt_t    dims;
	gridPointUint32_t idxLo;

	assert(writer != NULL && writer->isActive);
	assert(patch != NULL);
	assert(patchName != NULL);

	gridPatch_getIdxLo(patch, idxLo);

	for (int i = 0; i < NDIM; i++) {
		// need left and right coords of the cell, hence the +1
		dims[i]   = (int)(gridPatch_getOneDim(patch, i) + 1);
		coords[i] = xmalloc(sizeof(double) * dims[i]);
		for (uint32_t j = 0; j < dims[i]; j++)
			coords[i][j] = origin[i] + (j + idxLo[i]) * delta[i];
	}

	DBPutQuadmesh(writer->f, patchName, NULL, coords, dims, NDIM,
	              DB_DOUBLE, DB_COLLINEAR, NULL);

	for (int i = 0; i < NDIM; i++)
		xfree(coords[i]);

	local_writePatchData(writer, patch, patchName);
}

extern void
gridWriterSilo_writeGridRegular(gridWriterSilo_t writer,
                                gridRegular_t    grid)
{
	int            numPatches;
	gridPointDbl_t origin;
	gridPointDbl_t delta;
	char           **patchNames;
	char           *gridName;
	int            *meshTypes;

	assert(writer != NULL && writer->isActive);
	assert(grid != NULL);

	gridName   = local_getGridName(writer, gridRegular_getName(grid));
	numPatches = gridRegular_getNumPatches(grid);
	meshTypes  = xmalloc(sizeof(int) * numPatches);
	patchNames = local_getPatchNames(gridName, numPatches);
	gridRegular_getOrigin(grid, origin);
	gridRegular_getDelta(grid, delta);
	for (int i = 0; i < numPatches; i++) {
		gridPatch_t patch = gridRegular_getPatchHandle(grid, i);
		gridWriterSilo_writeGridPatch(writer, patch, patchNames[i],
		                              origin, delta);
		meshTypes[i] = DB_QUAD_RECT;
	}
	DBPutMultimesh(writer->f, gridName, numPatches, patchNames,
	               meshTypes, NULL);

	xfree(meshTypes);
	local_delPatchNames(patchNames, numPatches);
	xfree(gridName);
}

#  ifdef WITH_MPI
extern void
gridWriterSilo_initParallel(gridWriterSilo_t writer,
                            int              numFiles,
                            MPI_Comm         mpiComm,
                            int              mpiTag)
{
	int rank;

	assert(numFiles > 0);

	MPI_Comm_rank(mpiComm, &rank);

	writer->baton = PMPIO_Init(numFiles, PMPIO_WRITE, mpiComm, mpiTag,
	                           &local_createDB, &local_openDB,
	                           &local_closeDB, (void *)writer);

	writer->groupRank   = PMPIO_GroupRank(writer->baton, rank);
	writer->rankInGroup = PMPIO_RankInGroup(writer->baton, rank);
	writer->globalRank  = rank;
}

#  endif


/*--- Implementations of local functions --------------------------------*/
static void *
local_createDB(const char *fname, const char *dname, void *udata)
{
	gridWriterSilo_t writer = (gridWriterSilo_t)udata;

	writer->f = DBCreate(fname, DB_CLOBBER, DB_LOCAL, NULL,
	                     writer->dbType);
	if ((dname != NULL) && (dname[0] != '\0')) {
		DBMkDir(writer->f, dname);
		DBSetDir(writer->f, dname);
	}

	return writer->f;
}

#  ifdef WITH_MPI
static void *
local_openDB(const char     *fname,
             const char     *dname,
             PMPIO_iomode_t iomode,
             void           *udata)
{
	gridWriterSilo_t writer = (gridWriterSilo_t)udata;
	int              mode   = DB_APPEND;

	if (iomode == PMPIO_READ)
		mode = DB_READ;
	if (writer->f == NULL) {
		writer->f = DBOpen(fname, writer->dbType, mode);
		if ((dname != NULL) && (dname[0] != '\0')) {
			if (mode == DB_APPEND)
				DBMkDir(writer->f, dname);
			DBSetDir(writer->f, dname);
		}
	}

	return writer->f;
}

#  endif

static void
local_closeDB(void *file, void *udata)
{
	gridWriterSilo_t writer = (gridWriterSilo_t)udata;

	assert((DBfile *)file == writer->f);

	DBClose((DBfile *)file);
	writer->f = NULL;
}

static void
local_createFileName(gridWriterSilo_t writer)
{
	size_t charsToAlloc = 1;

	charsToAlloc    += strlen(writer->prefix);
	charsToAlloc    += strlen(LOCAL_FILESUFFIX);
#  ifdef WITH_MPI
	charsToAlloc    += 1 + LOCAL_NUMFILEDIGITS;
#  endif
	writer->fileName = xmalloc(charsToAlloc * sizeof(char));
#  ifdef WITH_MPI
	sprintf(writer->fileName, "%s_%0*i%s",
	        writer->prefix, LOCAL_NUMFILEDIGITS,
	        writer->groupRank, LOCAL_FILESUFFIX);
#  else
	sprintf(writer->fileName, "%s%s", writer->prefix, LOCAL_FILESUFFIX);
#  endif
}

static void
local_createDirName(gridWriterSilo_t writer)
{
	size_t charsToAlloc = 0;

	charsToAlloc    = 1;
#  ifdef WITH_MPI
	charsToAlloc   += strlen(LOCAL_DIRPREFIX) + LOCAL_NUMDIRDIGITS;
#  endif
	writer->dirName = xmalloc(charsToAlloc * sizeof(char));
#  ifdef WITH_MPI
	sprintf(writer->dirName, "%s%0*i",
	        LOCAL_DIRPREFIX, LOCAL_NUMDIRDIGITS, writer->globalRank);
#  else
	writer->dirName[0] = '\0';
#  endif
}

static char *
local_getGridName(gridWriterSilo_t writer, const char *base)
{
	char *name;
	int  charsToAlloc = strlen(base) + 1;
#  ifdef WITH_MPI
	charsToAlloc += 1 + 3;
#  endif

	name = xmalloc(sizeof(char) * charsToAlloc);
#  ifdef WITH_MPI
	sprintf(name, "%s_%03i", base, writer->rankInGroup);
#  else
	sprintf(name, "%s", base);
#  endif

	return name;
}

static char **
local_getPatchNames(const char *base, int numPatches)
{
	char **names;
	int  charsToAlloc = strlen(base) + 1 + 3 + 1;

	names = xmalloc(sizeof(char *) * numPatches);
	for (int i = 0; i < numPatches; i++) {
		names[i] = xmalloc(sizeof(char) * charsToAlloc);
		sprintf(names[i], "%s_%03i", base, i);
	}

	return names;
}

static void
local_delPatchNames(char **patchNames, int numPatches)
{
	for (int i = 0; i < numPatches; i++)
		xfree(patchNames[i]);
	xfree(patchNames);
}

static void
local_writePatchData(gridWriterSilo_t writer,
                     gridPatch_t      patch,
                     const char       *patchName)
{
	int            numVars  = gridPatch_getNumVars(patch);
	char           *varName = NULL;
	gridPointInt_t dims;


	for (int i = 0; i < numVars; i++) {
		gridVar_t var          = gridPatch_getVarHandle(patch, i);
		void      *data        = gridPatch_getVarDataHandle(patch, i);
		int       varType      = local_getVarType(var);
		int       varCentering = local_getVarCentering(var);

		for (int j = 0; j < NDIM; j++) {
			dims[j] = (int)(gridPatch_getOneDim(patch, j));
			if (varCentering == DB_NODECENT)
				dims[j]++;
		}

		varName = local_getPatchVarName(var, patchName, varName);
		DBPutQuadvar1(writer->f, varName, patchName, data, dims, NDIM,
		              NULL, 0, varType, varCentering, NULL);
	}

	if (varName != NULL)
		xfree(varName);
}

inline static int
local_getVarType(gridVar_t var)
{
	int varType;

	switch (gridVar_getType(var)) {
	case GRIDVARTYPE_INT:
		varType = DB_INT;
		break;
	case GRIDVARTYPE_DOUBLE:
		varType = DB_DOUBLE;
		break;
	case GRIDVARTYPE_FPV:
		varType = (sizeof(fpv_t) == 4 ? DB_FLOAT : DB_DOUBLE);
		break;
	default:
		// We should never ever end up here.
		diediedie(EXIT_FAILURE);
	}

	return varType;
}

inline static int
local_getVarCentering(gridVar_t var)
{
	// Eventually we need to query var for its centering, but at the
	// moment that is not needed and everything is zone centered.
	return DB_ZONECENT;
}

static void *
local_getPatchVarName(gridVar_t var, const char *patchName, char *varName)
{
	char *baseVarName = gridVar_getName(var);
	int  lenBaseName  = strlen(baseVarName);
	int  lenSuffix    = strlen(patchName);

	varName = xrealloc(varName, lenBaseName + lenSuffix + 1 + 1);
	sprintf(varName, "%s_%s", baseVarName, patchName);

	return varName;
}

#endif
