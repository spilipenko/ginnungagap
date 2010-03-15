// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.


/*--- Includes ----------------------------------------------------------*/
#include "CONF_HEADER"
#include "boiler.h"
#include <assert.h>
#include "../libutil/xmem.h"


/*--- Implemention of main structure ------------------------------------*/
#include "boiler_adt.h"


/*--- Local defines -----------------------------------------------------*/


/*--- Prototypes of local functions -------------------------------------*/


/*--- Implementations of exported functios ------------------------------*/
extern boiler_t
boiler_new(void)
{
	boiler_t boiler;

	boiler = xmalloc(sizeof(struct boiler_struct));

	return boiler;
}

extern void
boiler_del(boiler_t *boiler)
{
	assert(boiler != NULL && *boiler != NULL);

	xfree(*boiler);

	*boiler = NULL;
}

/*--- Implementations of local functions --------------------------------*/