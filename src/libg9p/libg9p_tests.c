// Copyright (C) 2012, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.


/*--- Includes ----------------------------------------------------------*/
#include "g9pConfig.h"
#include "g9pFieldID_tests.h"
#include "g9pHierarchy_tests.h"
#include "g9pHierarchyIO_tests.h"
#include "g9pMask_tests.h"
#include "g9pMaskIO_tests.h"
#include "g9pMaskShapelet_tests.h"
#include "g9pMaskCreator_tests.h"
#include "g9pICMap_tests.h"
#include "g9pDataStore_tests.h"
#include "g9pIDGenerator_tests.h"
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
#define NAME "libg9p"


/*--- Macros ------------------------------------------------------------*/
#define RUNTEST(a, hasFailed)   \
    if (!(local_runtest(a))) {  \
		hasFailed = true;       \
	} else {                    \
		if (!hasFailed)         \
			hasFailed = false;  \
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
		printf("\nRunning tests for g9pFieldID:\n");
	}
	RUNTEST(&g9pFieldID_getStr_test, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pHierarchy:\n");
	}
	RUNTEST(&g9pHierarchy_verifyComplexCreationMethodsAreIdentical,
	        hasFailed);
	RUNTEST(&g9pHierarchy_verifySimpleCreationMethodsAreIdentical,
	        hasFailed);
	RUNTEST(&g9pHierarchy_verifyDimsSelections, hasFailed);
	RUNTEST(&g9pHierarchy_verifyFactorSelections, hasFailed);
	RUNTEST(&g9pHierarchy_verifyReferenceCounting, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pHierarchyIO:\n");
	}
	RUNTEST(&g9pHierarchyIO_verifyConstructionFromIni, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pMask:\n");
	}
	RUNTEST(&g9pMask_verifyCreationOfMinMaxMask, hasFailed);
	RUNTEST(&g9pMask_verifyMaxNumCells, hasFailed);
	RUNTEST(&g9pMask_verifyNumCellsEmptyMask, hasFailed);
	RUNTEST(&g9pMask_verifyCreationOfGridStructure, hasFailed);
	RUNTEST(&g9pMask_verifyCreationOfPatch, hasFailed);
	RUNTEST(&g9pMask_verifyDelete, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pMaskIO:\n");
	}
	RUNTEST(&g9pMaskIO_verifyCreationFromIniExternalHierarchy,
	        hasFailed);
	RUNTEST(&g9pMaskIO_verifyCreationFromIniWithHierarchyFromIni,
	        hasFailed);
	RUNTEST(&g9pMaskIO_verifyReadWriteSimple,
	        hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pMaskShapelet:\n");
	}
	RUNTEST(&g9pMaskShapelet_verifyShapelet, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pMaskCreator:\n");
	}
	RUNTEST(&g9pMaskCreator_verifyEmptyMaskHasMinLevelThroughout,
	        hasFailed);
	RUNTEST(&g9pMaskCreator_verifyMaskIfOneCellIsTagged,
	        hasFailed);
	RUNTEST(&g9pMaskCreator_verifyMaskIfOneCellsAffectsMultipleTiles,
	        hasFailed);
	RUNTEST(&g9pMaskCreator_verifyMaskIfOneCellsAffectsMultTilesPeriodic,
	        hasFailed);
	RUNTEST(&g9pMaskCreator_verifyMaskIfThreeCellsAreTaggedInOneTile,
	        hasFailed);
	RUNTEST(&g9pMaskCreator_verifyMaskIfTwoCellsAreTaggedSlightOverlap,
	        hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pICMap:\n");
	}
	RUNTEST(&g9pICMap_verifySimpleMapCreation, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pDataStore:\n");
	}
	RUNTEST(&g9pDataStore_verifyCreation, hasFailed);
	RUNTEST(&g9pDataStore_verifyFileNames, hasFailed);
#ifdef XMEM_TRACK_MEM
	if (rank == 0)
		xmem_info(stdout);
	global_max_allocated_bytes = 0;
#endif

	if (rank == 0) {
		printf("\nRunning tests for g9pIDGenerator:\n");
	}
	RUNTEST(&g9pIDGenerator_verifyCreation, hasFailed);
	RUNTEST(&g9pIDGenerator_verifyIDGeneration, hasFailed);
	RUNTEST(&g9pIDGenerator_verifyIDResolving, hasFailed);
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
