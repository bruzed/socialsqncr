/*
 *  Slinky.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SLINKY_
#define _SLINKY_

#include "Creature.h"

class Slinky : public Creature {
public:
	
	virtual void initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength);
	virtual void update();
	virtual void draw();
};

#endif