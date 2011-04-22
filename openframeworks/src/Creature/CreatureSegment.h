/*
 *  CreatureSegment.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CREATURE_SEGMENT_
#define _CREATURE_SEGMENT_

#include "Constants.h"
#include "ofxVec3f.h"
#include "Particle3D.h"
#include "Tentacle.h" 

// Orientation Struct is located in Constants //

// extends Particle3D to serve as the spine pt //
class CreatureSegment : public Particle3D {
public:
	vector <ofxVec3f> bodyPts;
	int status;
	int numPts;
	ofxVec3f prevVel;
	Orientation orientation;
	
	float segColorPct; // when the segment is hit on a beat, what percent of the highlight color are you?
	float segColorPosPct; // used for scaling through tentacles and spikes and tangles // 0 is first part of tentacles //
	
	// set from the slinky class //
	vector <Tentacle> tentacles; // only used for Slinky, tried making a derived class, fail, shouldn't be too ineff with it :(
	float extremetySpacing; // for division of tentacles and division for drawing of spikes and tangles //
	int numTentacles;
	
	float radFromCenter; // how far from center location is it, while not recording //
	float radFromCenterRecording; // how far from center location is it, while recording //
	
	~CreatureSegment();
	virtual void initiate(int $numPts, float $radius, float $x, float $y, float $z);
	virtual void update();
	
};

#endif