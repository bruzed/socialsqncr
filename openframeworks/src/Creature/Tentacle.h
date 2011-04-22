/*
 *  Tentacle.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TENTACLE_
#define _TENTACLE_

#include "Particle3D.h"
#include "Constants.h"

class Tentacle {
public:
	vector <Particle3D> pts;
	vector <Orientation> orientations;
	int numPts;
	int halfPts;
	float tentacleSpacing;
	
	void addPoint(float $x, float $y, float $z);
	void clearPoints();
	void update();
};

#endif