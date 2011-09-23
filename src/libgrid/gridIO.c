// Copyright (C) 2010, 2011, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Includes ----------------------------------------------------------*/
#include "gridConfig.h"
#include "gridIO.h"
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "../libutil/xfile.h"
#include "../libutil/byteswap.h"
#include "../libutil/diediedie.h"


/*--- Local variables ---------------------------------------------------*/
static const char *local_typeBovStr     = "bov";
static const char *local_typeSiloStr    = "silo";
static const char *local_typeGraficStr  = "grafic";
static const char *local_typeHDF5Str    = "hdf5";
static const char *local_typeUnknownStr = "unknown";


/*--- Prototypes of local functions -------------------------------------*/


/*--- Implementations of exported functios ------------------------------*/
extern gridIO_type_t
gridIO_getTypeFromName(const char *name)
{
	gridIO_type_t rtn;

	assert(name != NULL);

	if (strcmp(name, local_typeBovStr) == 0)
		rtn = GRIDIO_TYPE_BOV;
	else if (strcmp(name, local_typeSiloStr) == 0)
		rtn = GRIDIO_TYPE_SILO;
	else if (strcmp(name, local_typeGraficStr) == 0)
		rtn = GRIDIO_TYPE_GRAFIC;
	else if (strcmp(name, local_typeHDF5Str) == 0)
		rtn = GRIDIO_TYPE_HDF5;
	else
		rtn = GRIDIO_TYPE_UNKNOWN;

	return rtn;
}

extern const char *
gridIO_getNameFromType(gridIO_type_t type)
{
	const char *rtn;

	if (type == GRIDIO_TYPE_BOV)
		rtn = local_typeBovStr;
	else if (type == GRIDIO_TYPE_SILO)
		rtn = local_typeSiloStr;
	else if (type == GRIDIO_TYPE_GRAFIC)
		rtn = local_typeGraficStr;
	else if (type == GRIDIO_TYPE_HDF5)
		rtn = local_typeHDF5Str;
	else
		rtn = local_typeUnknownStr;

	return rtn;
}

/*--- Implementations of local functions --------------------------------*/
