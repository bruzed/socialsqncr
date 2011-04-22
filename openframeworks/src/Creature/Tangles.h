/*
 *  Tangles.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TANGLES_
#define _TANGLES_

#include "Creature.h"

class Tangles : public Creature {
public:
	virtual void initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength);
	virtual void update();
	virtual void draw();
};

#endif
