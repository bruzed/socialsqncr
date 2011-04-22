/*
 *  CreatureSegment.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CreatureSegment.h"

CreatureSegment::~CreatureSegment() {
	bodyPts.clear();
	tentacles.clear();
}

void CreatureSegment::initiate(int $numPts, float $radius, float $x, float $y, float $z) {
	//status = CREATURE_STATUS_RECORDING;
	numPts = $numPts;
	loc.x = $x;
	loc.y = $y;
	loc.z = $z;
	radius = $radius;
	prevVel.set(0, 0, 0);
	
	segColorPct = 0.f;
	segColorPosPct = 0.f;
	
	extremetySpacing = 10.f;
	
	orientation.up.set(0, 1, 0);
	orientation.forward.set(1, 0, 0);
	orientation.side.set(0, 0, 1);
	
	
	for (int i = 0; i < numPts; i++) {
		ofxVec3f pt;
		pt.set(0, 0, 0);
		bodyPts.push_back(pt);
	}
}



void CreatureSegment::update() {
	addDamping( DAMPING );
	vel += acc;
	vel.limit( 30.f );
	loc += vel;
	acc.set(0, 0, 0);
	
	segColorPct -= .009f;
	if (segColorPct < 0) segColorPct = 0;
	if (segColorPct > 1) segColorPct = 1;
	
	segColorPosPct += .08f;
	if (segColorPct < 0) segColorPosPct = 0.f;
}





