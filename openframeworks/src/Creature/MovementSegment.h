/*
 *  MovementSegment.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MOVEMENT_SEGMENT_
#define _MOVEMENT_SEGMENT_

#include "Constants.h"
#include "ofxVec3f.h"

class MovementSegment {
public:
	vector <MovementPoint> bodyPts; // defined in Constants //
	int numPts;
	bool notes[ CREATURE_NUM_NOTES_PER_SEGMENT ];
	float radius;
	
	~MovementSegment();
	
	void initiate(int $numPts, float $radius);
	void reset(float $radius);
	void resetRandom(float $radius, float $randMaxAmt);
	
	void setNoteActivated(int $noteIndex);
};

#endif