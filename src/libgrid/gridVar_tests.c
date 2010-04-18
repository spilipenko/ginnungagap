// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Includes ----------------------------------------------------------*/
#include "gridConfig.h"
#include "gridVar_tests.h"
#include "gridVar.h"
#include "gridVarType.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#ifdef WITH_MPI
#  include <mpi.h>
#endif
#ifdef XMEM_TRACK_MEM
#  include "../libutil/xmem.h"
#endif


/*--- Implemention of main structure ------------------------------------*/
#include "gridVar_adt.h"


/*--- Local defines -----------------------------------------------------*/
#define LOCAL_TESTNAME "ad12dklkg3t13t"


/*--- Prototypes of local functions -------------------------------------*/


/*--- Implementations of exported functios ------------------------------*/
extern bool
gridVar_new_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (strcmp(gridVar->name, LOCAL_TESTNAME) != 0)
		hasPassed = false;
	if (gridVar->type != GRIDVARTYPE_DOUBLE)
		hasPassed = false;
	if (gridVar->numComponents != NDIM)
		hasPassed = false;
	if (gridVar->isFFTWPadded)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_del_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	gridVar_del(&gridVar);
	if (gridVar != NULL)
		hasPassed = false;
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_getRef_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t var, varBackup;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	var       = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	varBackup = gridVar_getRef(var);
	if (varBackup->name != var->name)
		hasPassed = false;
	if (varBackup->refCounter != var->refCounter)
		hasPassed = false;
	if (varBackup->refCounter != 2)
		hasPassed = false;
	gridVar_del(&var);
	if (varBackup->refCounter != 1)
		hasPassed = false;
	gridVar_del(&varBackup);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
} /* gridVar_getRef_test */

extern bool
gridVar_getSizePerElement_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (gridVar_getSizePerElement(gridVar) != sizeof(double) * NDIM)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_getNumComponents_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (gridVar_getNumComponents(gridVar) != NDIM)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_getType_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (gridVar_getType(gridVar) != GRIDVARTYPE_DOUBLE)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_getName_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (strcmp(gridVar_getName(gridVar), LOCAL_TESTNAME) != 0)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_setMemFuncs_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	gridVar_setMemFuncs(gridVar, NULL, NULL);
	if (gridVar->mallocFunc != NULL || gridVar->freeFunc != NULL)
		hasPassed = false;
	gridVar_setMemFuncs(gridVar, &malloc, &free);
	if (gridVar->mallocFunc != &malloc || gridVar->freeFunc != &free)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_getMemory_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	void      *tmp;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	tmp     = gridVar_getMemory(gridVar, UINT64_C(1024));
	if (tmp == NULL)
		hasPassed = false;
	gridVar_freeMemory(gridVar, tmp);
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_freeMemory_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	void      *tmp;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	tmp     = gridVar_getMemory(gridVar, UINT64_C(1024));
	gridVar_freeMemory(gridVar, tmp);
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_setFFTWPadded_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	gridVar_setFFTWPadded(gridVar);
	if (!gridVar->isFFTWPadded)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_unsetFFTWPadded_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	gridVar_setFFTWPadded(gridVar);
	gridVar_unsetFFTWPadded(gridVar);
	if (gridVar->isFFTWPadded)
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

extern bool
gridVar_isFFTWPadded_test(void)
{
	bool      hasPassed = true;
	int       rank      = 0;
	gridVar_t gridVar;
#ifdef XMEM_TRACK_MEM
	size_t    allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	gridVar = gridVar_new(LOCAL_TESTNAME, GRIDVARTYPE_DOUBLE, NDIM);
	if (gridVar_isFFTWPadded(gridVar))
		hasPassed = false;
	gridVar_setFFTWPadded(gridVar);
	if (!gridVar_isFFTWPadded(gridVar))
		hasPassed = false;
	gridVar_del(&gridVar);
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}


/*--- Implementations of local functions --------------------------------*/
