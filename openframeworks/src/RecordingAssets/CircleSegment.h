/*
 *  CircleSegment.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CIRCLE_SEGMENT_
#define _CIRCLE_SEGMENT_

#include "ofMain.h"

class CircleSegment {
public:
	float startAngle, endAngle;
	float radius, outsideRadius, midRadius;
	float centerX, centerY;
	float hitPct;
	
	// p1 is innerMost pt on inside radius, set by startRadius //
	vector <ofPoint> pts;
	
	// determine rect in here to keep other classes cleaner //
	ofRectangle rect;
	
	unsigned int lastTimeTriggered;
	
	bool bIsActive;
	
	void initiate(float $startAngle, float $endAngle, float $centerX, float $centerY, float $radius, float $outsideRadius);
	void setRect();
	void update();
};

#endif
