/*
 *  MovementSegment.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MovementSegment.h"

MovementSegment::~MovementSegment() {
	bodyPts.clear();
	//delete notes;
}

void MovementSegment::initiate(int $numPts, float $radius) {
	//status = CREATURE_STATUS_RECORDING;
	numPts = $numPts;
	radius = $radius;
	
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		notes[i] = false;
	}
	
	for (int i = 0; i < numPts; i++) {
		MovementPoint pt;
		pt.loc.set(0, 0, 0);
		bodyPts.push_back(pt);
	}
	//reset( radius );
}


void MovementSegment::reset(float $radius) {
	float angle = 0;
	float angleInc = TWO_PI / (float)numPts;
	for (int i = 0; i < numPts; i++) {
		float x = cos(angle) * $radius;
		float y = sin(angle) * $radius;
		bodyPts[i].loc.x = x;
		bodyPts[i].loc.y = y;
		//bodyPts[i].z = loc.z;
		angle += angleInc;
	}
}

void MovementSegment::resetRandom(float $radius, float $randMaxAmt) {
	float angle = 0;
	float angleInc = TWO_PI / (float)numPts;
	angle = angleInc * .5;
	for (int i = 0; i < numPts; i++) {
		float rand = ofRandom(0, $randMaxAmt);
		float x = cos(angle) * ($radius + rand);
		float y = sin(angle) * ($radius + rand);
		bodyPts[i].loc.x = x;
		bodyPts[i].loc.y = y;
		bodyPts[i].loc.z = 0;
		angle += angleInc;
	}
}

void MovementSegment::setNoteActivated(int $noteIndex) {
	notes[$noteIndex] = true;
}


