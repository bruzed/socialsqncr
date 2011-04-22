/*
 *  Spiky.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SPIKY_
#define _SPIKY_

#include "Creature.h"

class Spiky : public Creature {
public:
	virtual void initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength);
	virtual void update();
	virtual void draw();
	
private:
	bool bFirstRun;
	vector <Particle3D> spikeParticles;
};

#endif