// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Includes ----------------------------------------------------------*/
#include "gridConfig.h"
#include "gridVarTypes_tests.h"
#include "gridVarTypes.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#ifdef WITH_MPI
#  include <mpi.h>
#endif


/*--- Implementations of exported functios ------------------------------*/
extern bool
gridVarTypes_getSizePerElement_test(void)
{
	bool hasPassed = true;
	int  rank      = 0;
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
	if (rank == 0)
		printf("Testing %s... ", __func__);

	if (gridVarTypes_getSizePerElement(GRIDVARTYPES_FPV)
	    != sizeof(fpv_t))
		hasPassed = false;
	if (gridVarTypes_getSizePerElement(GRIDVARTYPES_FPVVEC)
	    != NDIM * sizeof(fpv_t))
		hasPassed = false;
	if (gridVarTypes_getSizePerElement(GRIDVARTYPES_INT32)
	    != sizeof(int32_t))
		hasPassed = false;
	if (gridVarTypes_getSizePerElement(GRIDVARTYPES_INT32VEC)
	    != NDIM * sizeof(int32_t))
		hasPassed = false;

	return hasPassed ? true : false;
}