/*
 *  CircleSegment.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CircleSegment.h"

void CircleSegment::initiate(float $startAngle, float $endAngle, float $centerX, float $centerY, float $radius, float $outsideRadius) {
	startAngle = $startAngle;
	endAngle = $endAngle;
	radius = $radius;
	outsideRadius = $outsideRadius;
	
	midRadius = (outsideRadius - radius) * .5 + radius;
	
	centerX = $centerX;
	centerY = $centerY;
	
	bIsActive = false;
	
	lastTimeTriggered = ofGetElapsedTimeMillis();
	
	for(int i = 0; i < 4; i++) {
		pts.push_back(ofPoint());
	}
	
	hitPct = 0.f;
	
	float cosAngle = cos( startAngle );
	float sinAngle = sin( startAngle );
	
	pts[0].x = radius * cosAngle + centerX;
	pts[0].y = radius * sinAngle + centerY;
	
	pts[1].x = outsideRadius * cosAngle + centerX;
	pts[1].y = outsideRadius * sinAngle + centerY;
	
	cosAngle = cos( endAngle );
	sinAngle = sin( endAngle );
	
	pts[2].x = radius * cosAngle + centerX;
	pts[2].y = radius * sinAngle + centerY;
	
	pts[3].x = outsideRadius * cosAngle + centerX;
	pts[3].y = outsideRadius * sinAngle + centerY;
	
	setRect();
	
	//cout << "Rectangle = " << "x: " << rect.x << ", y: " << rect.y << ", width: " << rect.width << ", height: " << rect.height << endl;
}

void CircleSegment::setRect() {
	float minX = 10000;
	float minY = 10000;
	float maxX = -10000;
	float maxY = -10000;
	
	for(int i = 0; i < 4; i++) {
		if (pts[i].x < minX) {
			minX = pts[i].x;
		}
		if (pts[i].x > maxX) {
			maxX = pts[i].x;
		}
		if (pts[i].y < minY) {
			minY = pts[i].y;
		}
		if (pts[i].y > maxY) {
			maxY = pts[i].y;
		}
	}
	
	rect.x = minX;
	rect.y = minY;
	rect.width = maxX - minX;
	rect.height = maxY - minY;
}

void CircleSegment::update() {
	
}