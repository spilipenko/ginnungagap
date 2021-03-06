// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.


/*--- Includes ----------------------------------------------------------*/
#include "dataConfig.h"
#include "dataVar_tests.h"
#include "dataVarType_tests.h"
#include "dataParticle_tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef WITH_MPI
#  include <mpi.h>
#endif
#ifdef XMEM_TRACK_MEM
#  include "../libutil/xmem.h"
#endif


/*--- Local defines -----------------------------------------------------*/
#define NAME "libdata"


/*--- Macros ------------------------------------------------------------*/
#define RUNTEST(a, hasFailed)  \
    if (!(local_runtest(a))) { \
		hasFailed = true;      \
	} else {                   \
		if (!hasFailed)        \
			hasFailed = false; \
	}


/*--- Prototypes of local functions -------------------------------------*/
static bool
local_runtest(bool (*f)(void));


/*--- M A I N -----------------------------------------------------------*/
int
main(int argc, char **argv)
{
	bool hasFailed = false;
	int  rank      = 0;
	int  size      = 1;

#ifdef WITH_MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif
	if (rank == 0) {
		printf("\nTesting %s on %i %s\n",
		       NAME, size, size > 1 ? "tasks" : "task");
	}

	if (rank == 0) {
		printf("\nRunning tests for dataVar:\n");
	}
	RUNTEST(&dataVar_new_test, hasFailed);
	RUNTEST(&dataVar_clone_test, hasFailed);
	RUNTEST(&dataVar_del_test, hasFailed);
	RUNTEST(&dataVar_getRef_test, hasFailed);
	RUNTEST(&dataVar_getSizePerElement_test, hasFailed);
	RUNTEST(&dataVar_getNumComponents_test, hasFailed);
	RUNTEST(&dataVar_getType_test, hasFailed);
	RUNTEST(&dataVar_setMemFuncs_test, hasFailed);
	RUNTEST(&dataVar_getMemory_test, hasFailed);
	RUNTEST(&dataVar_freeMemory_test, hasFailed);
	RUNTEST(&dataVar_getPointerByOffset_test, hasFailed);
	RUNTEST(&dataVar_setFFTWPadded_test, hasFailed);
	RUNTEST(&dataVar_unsetFFTWPadded_test, hasFailed);
	RUNTEST(&dataVar_isFFTWPadded_test, hasFailed);
	RUNTEST(&dataVar_setComplexified_test, hasFailed);
	RUNTEST(&dataVar_unsetComplexified_test, hasFailed);
	RUNTEST(&dataVar_isComplexified_test, hasFailed);
#ifdef WITH_MPI
	RUNTEST(&dataVar_getMPIDatatype_test, hasFailed);
	RUNTEST(&dataVar_getMPICount_test, hasFailed);
#endif
#ifdef WITH_HDF5
	RUNTEST(&dataVar_getHDF5Datatype_test, hasFailed);
#endif
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for dataVarType:\n");
	}
	RUNTEST(&dataVarType_sizeof_test, hasFailed);
	RUNTEST(&dataVarType_isFloating_test, hasFailed);
	RUNTEST(&dataVarType_isInteger_test, hasFailed);
	RUNTEST(&dataVarType_isNativeFloat_test, hasFailed);
	RUNTEST(&dataVarType_isNativeDouble_test, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for dataParticle:\n");
	}
	RUNTEST(&dataParticle_new_test, hasFailed);
	RUNTEST(&dataParticle_getRef_test, hasFailed);
	RUNTEST(&dataParticle_del_test, hasFailed);
	RUNTEST(&dataParticle_addVar_test, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

#ifdef WITH_MPI
	MPI_Finalize();
#endif

	if (hasFailed) {
		if (rank == 0)
			fprintf(stderr, "\nSome tests failed!\n\n");
		return EXIT_FAILURE;
	}
	if (rank == 0)
		printf("\nAll tests passed successfully!\n\n");

	return EXIT_SUCCESS;
} /* main */

/*--- Implementations of local functions --------------------------------*/
static bool
local_runtest(bool (*f)(void))
{
	bool hasPassed = f();
	int  rank      = 0;
#ifdef WITH_MPI
	int  failedGlobal;
	int  failedLocal = hasPassed ? 0 : 1;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Allreduce(&failedLocal, &failedGlobal, 1, MPI_INT, MPI_MAX,
	              MPI_COMM_WORLD);
	if (failedGlobal != 0)
		hasPassed = false;
#endif

	if (!hasPassed) {
		if (rank == 0)
			printf("!! FAILED !!\n");
	} else {
		if (rank == 0)
			printf("passed\n");
	}

	return hasPassed;
}
