/*
 *  CircleSegmentManager.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CircleSegmentManager.h"

void CircleSegmentManager::setup( float $centerX, float $centerY) {
	nSegments			= CREATURE_NUM_NOTES_PER_SEGMENT;
	float angle			= 0.f;
	float angleInc		= TWO_PI / (float) (nSegments);
	float outerRadius	= 420.f;
	float radius		= 390.f;
	float outerAngle;
	
	visible = true;
	alpha = 255;
	
	center.x = $centerX;
	center.y = $centerY;
	
	colors = new ofColor[4];
	
	colors[0].r = 6; colors[0].g = 199; colors[0].b = 242; // blue low
	colors[1].r = 145; colors[1].g = 226; colors[1].b = 246; // blue high
	
	colors[2].r = 130; colors[2].g = 166; colors[2].b = 56; // green low
	colors[3].r = 166; colors[3].g = 197; colors[3].b = 102; // green high
	
	for(int i = 0; i < nSegments; i++) {
		
		outerAngle = angle + angleInc;
		
		CircleSegment seg;
		//seg.initiate(angle, outerAngle, $centerX, $centerY, radius, outerRadius);
		seg.initiate(angle, outerAngle, 0, 0, radius, outerRadius);
		segments.push_back( seg );
		angle += angleInc;
	}
}

void CircleSegmentManager::colorize( float $r, float $g, float $b ) {
	colors[0].r = $r; colors[0].g = $g; colors[0].b = $b; // blue low
	colors[2].r = $r; colors[2].g = $g; colors[2].b = $b; // green low
}

void CircleSegmentManager::draw() {
	ofColor lowColor, highColor;
	ofColor hitColor;
	hitColor.r = 255; hitColor.g = 255; hitColor.b = 255;
	float radBump;
	float colorPct, oneOverColorPct;
	for(int i = 0; i < nSegments; i++) {
		float pct = (float) i / nSegments;
		colorPct = segments[i].hitPct;
		oneOverColorPct = 1 - colorPct;
		
		radBump = colorPct * 20;
		
		if (i < 4) {
			highColor = colors[3];
			lowColor = colors[2];
		} else {
			highColor = colors[1];
			lowColor = colors[0];
		}
		
		highColor.r = (oneOverColorPct * highColor.r) + (colorPct * hitColor.r);
		highColor.g = (oneOverColorPct * highColor.g) + (colorPct * hitColor.g);
		highColor.b = (oneOverColorPct * highColor.b) + (colorPct * hitColor.b);
		
		lowColor.r = (oneOverColorPct * lowColor.r) + (colorPct * hitColor.r);
		lowColor.g = (oneOverColorPct * lowColor.g) + (colorPct * hitColor.g);
		lowColor.b = (oneOverColorPct * lowColor.b) + (colorPct * hitColor.b);
		
		if (!segments[i].bIsActive) { // the button is not active, so set to greyzors //
			lowColor.r = lowColor.g = lowColor.b = 120;
		}
		
		ofSetColor(lowColor.r, lowColor.g, lowColor.b, alpha);
		renderCircleSegmentTriangles(segments[i].startAngle, segments[i].endAngle, segments[i].centerX, segments[i].centerY, 20,
									 segments[i].radius + radBump, segments[i].outsideRadius + radBump);
		// draw highlights //
		//glPushMatrix();
		//glTranslatef(0, 0, .1);
		//ofSetColor(highColor.r, highColor.g, highColor.b);
		//renderCircleSegmentTriangles(segments[i].startAngle, segments[i].endAngle, segments[i].centerX, segments[i].centerY, 20,
		//							 segments[i].midRadius + radBump, segments[i].outsideRadius + 1 + radBump);
		//glPopMatrix();
		
		if (panel->getValueB("B_SHOW_TRIANGLES")) {
			glPolygonMode (GL_BACK, GL_LINE);		// Draw Backfacing Polygons As Wireframes
			glLineWidth (1.f);
			ofSetColor(255, 255, 255);
			renderCircleSegmentTriangles(segments[i].startAngle, segments[i].endAngle, segments[i].centerX, segments[i].centerY, 20, segments[i].radius, segments[i].outsideRadius);
			glPolygonMode (GL_BACK, GL_FILL);
		}
		
		if (panel->getValueB("B_SHOW_BOUNDING_BOXES")) {
			ofNoFill();
			ofSetColor(255, 0, 0);
			ofRect(segments[i].rect.x, segments[i].rect.y, segments[i].rect.width, segments[i].rect.height);
			ofFill();
		}
	}
}

void CircleSegmentManager::renderCircleSegmentTriangles(float $startAngle, float $endAngle, float $centerX, float $centerY, int $res, float $radius, float $outsideRadius) {
	
	glBegin(GL_TRIANGLE_STRIP);
	
	float cosAngle, sinAngle;
	
	float angleStep = ($endAngle - $startAngle) / (float) $res;
	float angle = $startAngle;
	
	for (int i = 0; i < $res; i++) {
		cosAngle = cos( angle );
		sinAngle = sin( angle );
		
		glVertex2d(cosAngle * $radius + $centerX, sinAngle * $radius + $centerY);
		glVertex2d(cosAngle * $outsideRadius + $centerX, sinAngle * $outsideRadius + $centerY);
		
		angle += angleStep;
	}
	
	glEnd();
}