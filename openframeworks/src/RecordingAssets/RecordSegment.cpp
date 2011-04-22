/*
 *  RecordSegment.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RecordSegment.h"

void RecordSegment::initiate(int $numPts, float $radius, float $x, float $y, float $z) {
	numPts = $numPts;
	loc.x = $x;
	loc.y = $y;
	loc.z = $z;
	radius = $radius;
	
	for (int i = 0; i < numPts; i++) {
		Particle3D pt;
		pt.loc.set(0, 0, 0);
		bodyPts.push_back(pt);
	}
	
	numDrawingPts = numPts * 4;
	for (int i = 0; i < numDrawingPts; i++) {
		ofxVec3f vec(0, 0, 0);
		drawingPts.push_back( vec );
	}
	
	reset( radius );
}

void RecordSegment::reset(float $radius) {
	float angle = 0;
	float angleInc = TWO_PI / (float)numPts;
	angle = angleInc * .5;
	for (int i = 0; i < numPts; i++) {
		float x = cos(angle) * $radius + loc.x;
		float y = sin(angle) * $radius + loc.y;
		bodyPts[i].vel.set(0, 0, 0);
		bodyPts[i].acc.set(0, 0, 0);
		bodyPts[i].loc.x = x;
		bodyPts[i].loc.y = y;
		bodyPts[i].loc.z = loc.z;
		bodyPts[i].prevLoc.set(bodyPts[i].loc.x, bodyPts[i].loc.y, bodyPts[i].loc.z);
		angle += angleInc;
	}
	
	angleInc = TWO_PI / ((float)numDrawingPts * .5); // since two in a row are placed in same location //
	angle = angleInc * .5;
	for (int i = 0; i < numDrawingPts; i++) {
		float x = cos(angle) * $radius + loc.x;
		float y = sin(angle) * $radius + loc.y;
		drawingPts[i].x = x;
		drawingPts[i].y = y;
		drawingPts[i].z = loc.z;
		if (i % 2 == 1 && i > 0)
			angle += angleInc;
	}
}

void RecordSegment::resetRandom(float $radius, float $randMaxAmt) {
	float angle = 0;
	float angleInc = TWO_PI / (float)numPts;
	angle = angleInc * .5;
	for (int i = 0; i < numPts; i++) {
		float rand = ofRandom(0, $randMaxAmt);
		float x = cos(angle) * ($radius + rand) + loc.x;
		float y = sin(angle) * ($radius + rand) + loc.y;
		bodyPts[i].vel.set(0, 0, 0);
		bodyPts[i].acc.set(0, 0, 0);
		bodyPts[i].loc.x = x;
		bodyPts[i].loc.y = y;
		bodyPts[i].loc.z = loc.z;
		angle += angleInc;
	}
}


void RecordSegment::setNoteActivated(int $noteIndex) {
	notes[$noteIndex] = true;
}







