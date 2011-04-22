/*
 *  CircleSegmentManager.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CIRCLE_SEGMENT_MANAGER_
#define _CIRCLE_SEGMENT_MANAGER_

#include "CircleSegment.h"
#include "Constants.h"
#include "ofxControlPanel.h"

class CircleSegmentManager {
public:
	vector <CircleSegment> segments;
	int nCurveVertexes;
	int nSegments;
	ofColor* colors;
	
	bool visible;
	float alpha;
	
	ofxVec3f center;
	
	ofxControlPanel* panel;
	
	void setup(float $centerX, float $centerY);
	void colorize( float $r, float $g, float $b );
	void draw();
	void renderCircleSegmentTriangles(float $startAngle, float $endAngle, float $centerX, float $centerY, int $res, float $radius, float $outsideRadius);
};

#endif