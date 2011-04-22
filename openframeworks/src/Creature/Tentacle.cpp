/*
 *  Tentacle.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tentacle.h"

void Tentacle::addPoint(float $x, float $y, float $z) {
	Particle3D p;
	p.loc.set($x, $y, $z);
	pts.push_back(p);
	Orientation o;
	o.up.set( 0, 1, 0 );
	o.forward.set(1, 0, 0);
	o.side.set(0, 0, 1);
	orientations.push_back( o );
}

void Tentacle::clearPoints() {
	pts.clear();
}

void Tentacle::update() {
	for (int i = 1; i < pts.size(); i++) {
		bool bUpdateBothAcc = true;
		if (i == 1)
			bUpdateBothAcc = false;
		pts[i].fixedSpring(pts[i-1], .85, tentacleSpacing, bUpdateBothAcc);
	}
	for (int i = 0; i < pts.size(); i++) {
		pts[i].addDamping( .6 );
		pts[i].vel += pts[i].acc;
		pts[i].loc += pts[i].vel;
		pts[i].acc.set(0, 0, 0);
	}
	 
}

