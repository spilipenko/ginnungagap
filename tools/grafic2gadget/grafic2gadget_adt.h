// Copyright (C) 2010, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.

#ifndef GRAFIC2GADGET_ADT_H
#define GRAFIC2GADGET_ADT_H


/*--- Includes ----------------------------------------------------------*/
#include "grafic2gadgetConfig.h"
#include <stdbool.h>


/*--- Implemention of main structure ------------------------------------*/
struct grafic2gadget_struct {
	char   *graficFileNameVx;
	char   *graficFileNameVy;
	char   *graficFileNameVz;
	int    numGadgetFiles;
	char   *gadgetFileStem;
	bool   force;
	bool   useLongIDs;
	double omegaBaryon0;
	bool   doGas;
	double posFactor; // Scales from Mpc/h to whatever (for kpc/h: 1000.)
	double velFactor; // Sacles from km/s to whatever (for m/s: 1000.)
};


#endif
