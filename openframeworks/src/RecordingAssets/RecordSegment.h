/*
 *  RecordSegment.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RECORD_SEGMENT_
#define _RECORD_SEGMENT_

#include "Particle3D.h"
#include "Constants.h"

class RecordSegment : public Particle3D {
public:
	int numPts;
	vector <Particle3D> bodyPts;
	int numDrawingPts;
	vector <ofxVec3f>	drawingPts;
	
	bool notes[ CREATURE_NUM_NOTES_PER_SEGMENT ];
	
	void initiate(int $numPts, float $radius, float $x, float $y, float $z);
	void reset(float $radius);
	void resetRandom(float $radius, float $randMaxAmt);
	void setNoteActivated(int $noteIndex);
};

#endif