// Copyright (C) 2011, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Doxygen file description ------------------------------------------*/

/**
 * @file  cubepmFactory_tests.c
 * @ingroup  GROUP
 * @brief  SHORT DESC
 */


/*--- Includes ----------------------------------------------------------*/
#include "util_config.h"
#include "cubepmFactory_tests.h"
#include "cubepmFactory.h"
#include <stdio.h>
#include <string.h>
#ifdef WITH_MPI
#  include <mpi.h>
#endif
#ifdef ENABLE_XMEM_TRACK_MEM
#  include "../libutil/xmem.h"
#endif


/*--- Local defines -----------------------------------------------------*/


/*--- Prototypes of local functions -------------------------------------*/


/*--- Implementations of exported functions -----------------------------*/
extern bool
cubepmFactory_EXAMPLE_test(void)
{
	bool            hasPassed = true;
	int             rank      = 0;
	cubepmFactory_t cubepmFactory;
#ifdef XMEM_TRACK_MEM
	size_t          allocatedBytes = global_allocated_bytes;
#endif
#ifdef WITH_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

	if (rank == 0)
		printf("Testing %s... ", __func__);

	// INSERT TESTFUNCTION HERE
#ifdef XMEM_TRACK_MEM
	if (allocatedBytes != global_allocated_bytes)
		hasPassed = false;
#endif

	return hasPassed ? true : false;
}

/*--- Implementations of local functions --------------------------------*/
