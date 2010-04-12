// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.


/*--- Includes ----------------------------------------------------------*/
#include "cosmo_config.h"
#include "cosmoPk.h"
#include "cosmoFunc.h"
#include "../libutil/xmem.h"
#include "../libutil/xfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_integration.h>


/*--- Implemention of the ADT structure ---------------------------------*/
#include "cosmoPk_adt.h"


/*--- Prototypes of local functions -------------------------------------*/
static cosmoPk_t
local_new(void);

static void
local_getMem(cosmoPk_t pk, uint32_t numPoints);

static void
local_doInterpolation(cosmoPk_t pk);


/*--- Local structures and typedefs -------------------------------------*/


/*--- Implementations of exported functios ------------------------------*/
extern cosmoPk_t
cosmoPk_newFromFile(const char *fname)
{
	cosmoPk_t pk;
	FILE      *f;

	assert(fname != NULL);

	pk = local_new();
	f  = xfopen(fname, "r");
	if (fscanf(f, "# %" SCNu32 " %lf %lf \n",
	           &(pk->numPoints),
	           &(pk->slopeBeforeKmin),
	           &(pk->slopeBeyondKmax)) != 3) {
		diediedie(EXIT_FAILURE);
	}
	local_getMem(pk, pk->numPoints);
	for (uint32_t i = 0; i < pk->numPoints; i++) {
		if (fscanf(f, " %lf %lf \n", pk->k + i, pk->P + i) != 2) {
			diediedie(EXIT_FAILURE);
		}
	}
	xfclose(&f);
	local_doInterpolation(pk);

	return pk;
}

extern cosmoPk_t
cosmoPk_newFromArrays(uint32_t     numPoints,
                      const double *k,
                      const double *P,
                      double       slopeBeforeKmin,
                      double       slopeBeyondKmax)
{
	cosmoPk_t pk;
	assert(k != NULL && P != NULL);

	pk = local_new();
	local_getMem(pk, numPoints);
	memcpy(pk->k, k, sizeof(double) * numPoints);
	memcpy(pk->P, P, sizeof(double) * numPoints);
	pk->slopeBeforeKmin = slopeBeforeKmin;
	pk->slopeBeyondKmax = slopeBeyondKmax;
	local_doInterpolation(pk);

	return pk;
}

extern void
cosmoPk_del(cosmoPk_t *pk)
{
	assert(pk != NULL && *pk != NULL);

	if ((*pk)->k != NULL)
		xfree((*pk)->k);
	if ((*pk)->acc != NULL)
		gsl_interp_accel_free((*pk)->acc);
	if ((*pk)->spline != NULL)
		gsl_spline_free((*pk)->spline);
	xfree(*pk);
	*pk = NULL;
}

extern void
cosmoPk_dumpToFile(cosmoPk_t pk, const char *fname, uint32_t numSubSample)
{
	FILE *f;
	assert(pk != NULL && fname != NULL);
	double k, P;

	f = xfopen(fname, "w");
	for (uint32_t i= 0; i<pk->numPoints-1; i++) {
		for (uint32_t j=0; j<numSubSample; j++) {
			k = pk->k[i] + j*(pk->k[i+1] - pk->k[i])/numSubSample;
			P = cosmoPk_eval(pk, k);
			fprintf(f, "%15.12g\t%15.12g\n", k, P);
		}
	}
	xfclose(&f);
}

extern double
cosmoPk_eval(cosmoPk_t pk, double k)
{
	assert(pk != NULL && isgreater(k, 0.0));

	if (isless(k, pk->k[0]))
		return pk->P[0] * pow(k / (pk->k[0]), pk->slopeBeforeKmin);

	if (isgreater(k, pk->k[pk->numPoints - 1]))
		return pk->P[0] * pow(k / (pk->k[pk->numPoints - 1]),
		                      pk->slopeBeyondKmax);

	return gsl_spline_eval(pk->spline, k, pk->acc);
}

extern double
cosmoPk_evalGSL(double k, void *param)
{
	assert(param != NULL);
	return cosmoPk_eval((cosmoPk_t)param, k);
}

extern double
cosmoPk_calcMomentFiltered(cosmoPk_t pk,
                           uint32_t moment,
                           double (*windowFunc)(double, void *),
                           void *paramWindowFunc,
                           double kmin,
                           double kmax,
                           double *error)
{
	double                      sigmaSqr;
	double                      exponentK = (2. * moment + 2.);
	cosmoFunc_product3_struct_t param;
	gsl_integration_workspace   *w;
	gsl_function                F;

	assert(pk != NULL && error != NULL && windowFunc != NULL);

	param.f1      = &cosmoPk_evalGSL;
	param.paramF1 = (void *)pk;
	param.f2      = windowFunc;
	param.paramF2 = paramWindowFunc;
	param.f3      = &cosmoFunc_xPowerY;
	param.paramF3 = (void *)&exponentK;
	F.function    = &cosmoFunc_product3;
	F.params      = (void *)&param;

	w             = gsl_integration_workspace_alloc(1000);
	gsl_integration_qags(&F, kmin, kmax, 0, 1e-10, 1000,
	                     w, &sigmaSqr, error);
	gsl_integration_workspace_free(w);

	return 1. / (TWOPI * PI) * sigmaSqr;
}

extern double
cosmoPk_calcSigma8(cosmoPk_t pk,
                   double    kmin,
                   double    kmax,
                   double    *error)
{
	double scale = 8.0;
	double sigma8Sqr;

	assert(pk != NULL && isgreater(kmax, kmin) && isgreater(kmin, 0.0)
	       && error != NULL);

	sigma8Sqr = cosmoPk_calcMomentFiltered(pk, UINT32_C(0),
	                                       &cosmoFunc_tophatSqr,
	                                       &scale, kmin, kmax, error);
	return sqrt(sigma8Sqr);
}

/*--- Implementations of local functions --------------------------------*/
static cosmoPk_t
local_new(void)
{
	cosmoPk_t pk = xmalloc(sizeof(struct cosmoPk_struct));

	pk->numPoints       = UINT32_C(0);
	pk->k               = NULL;
	pk->P               = NULL;
	pk->slopeBeyondKmax = 1e10;
	pk->slopeBeforeKmin = 1e10;
	pk->acc             = NULL;
	pk->spline          = NULL;

	return pk;
}

static void
local_getMem(cosmoPk_t pk, uint32_t numPoints)
{
	if (pk->k != NULL)
		xfree(pk->k);
	pk->k         = xmalloc(sizeof(double) * numPoints * 2);
	pk->P         = pk->k + numPoints;
	pk->numPoints = numPoints;
}

static void
local_doInterpolation(cosmoPk_t pk)
{
	pk->acc    = gsl_interp_accel_alloc();
	pk->spline = gsl_spline_alloc(gsl_interp_cspline,
	                              (int)(pk->numPoints));
	gsl_spline_init(pk->spline, pk->k, pk->P, (int)(pk->numPoints));
}