/*
 *  Tangles.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tangles.h"

void Tangles::initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength) {
	Creature::initiate($numPts, $numSegmentPts, $maxRadius, $spineLength, $extremetySpacing, $tentacleDivLength);
	
	type = CREATURE_TYPE_RECTANGLES;
	
	segmentHighColor.r = 255; segmentHighColor.g = 50; segmentHighColor.b = 100; segmentHighColor.a = 200;
	segmentBaseColor.r = 255; segmentBaseColor.g = 255; segmentBaseColor.b = 255; segmentBaseColor.a = 50;
	
	extremetyHighColor.r = 200; extremetyHighColor.g = 255; extremetyHighColor.b = 50; extremetyHighColor.a = 200;
	extremetyBaseColor.r = 255; extremetyBaseColor.g = 255; extremetyBaseColor.b = 255; extremetyBaseColor.a = 150;
	//extremetyHighColor.r = 255; extremetyHighColor.g = 50; extremetyHighColor.b = 100; extremetyHighColor.a = 200;
	//extremetyBaseColor.r = 255; extremetyBaseColor.g = 255; extremetyBaseColor.b = 255; extremetyBaseColor.a = 50;
	
	for(int i = 0; i < totalPlaneNormals * 4; i++) {
		ofxVec3f pt(0, 0, 0);
		extrusionPts.push_back( pt );
	}
}

void Tangles::update() {
	Creature::update();
	
	////////////////////////////////////////////////////////////////////////////////
	ofxVec3f planeCenter, planeNormal, topRail;
	ofxVec3f pt1, pt2, pt3, pt4;
	ofxVec3f pt1Diff, pt2Diff, pt3Diff, pt4Diff;
	ofxVec3f bumpItVec;
	
	int planeNormalIndex = 0;
	int extrusionPtIndex = 0;
	float diffScale = 0.f;
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			
			for(int j = 0; j < spine[i].numPts; j++) {
				if (movementPts[i].bodyPts[j].radius > 3) {
					planeNormalIndex = (j * numMovementPts + i);
					extrusionPtIndex = planeNormalIndex * 4;
					//planeNormals[planeNormalIndex]->set(v2.x, v2.y, v2.z);
					
					planeNormal.set( planeNormals[planeNormalIndex]->x, planeNormals[planeNormalIndex]->y, planeNormals[planeNormalIndex]->z );
					planeCenter.set( planeCenters[planeNormalIndex]->x, planeCenters[planeNormalIndex]->y, planeCenters[planeNormalIndex]->z );
					
					planeNormal *= movementPts[i].bodyPts[j].radius * deathPct;
					
					if (spine[i].segColorPosPct < 3) {
						bumpItVec.set(planeNormal.x, planeNormal.y, planeNormal.z);
						bumpItVec.normalize();
						bumpItVec *= (3 - spine[i].segColorPosPct) * (planeNormal.length() * .4);
						planeNormal += bumpItVec;
					}
					
					
					
					pt1 = spine[i+1].bodyPts[j];
					pt2 = spine[i].bodyPts[j];
					pt3 = spine[i+1].bodyPts[j+1];
					pt4 = spine[i].bodyPts[j+1];
					if (j == spine[i].numPts - 1) {
						pt3 = spine[i+1].bodyPts[0];
						pt4 = spine[i].bodyPts[0];
					}
					
					topRail = pt2 - pt1;
					//float distBtw = topRail.length()
					pt1.set( (topRail.x * .45) + pt1.x, (topRail.y * .45) + pt1.y, (topRail.z * .45) + pt1.z);
					topRail = pt1 - pt2;
					pt2.set( (topRail.x * .45) + pt2.x, (topRail.y * .45) + pt2.y, (topRail.z * .45) + pt2.z);
					
					topRail = pt4 - pt3;
					pt3.set( (topRail.x * .45) + pt3.x, (topRail.y * .45) + pt3.y, (topRail.z * .45) + pt3.z);
					topRail = pt3 - pt4;
					pt4.set( (topRail.x * .45) + pt4.x, (topRail.y * .45) + pt4.y, (topRail.z * .45) + pt4.z);
					
					topRail = pt3 - pt1;
					pt1.set( (topRail.x * .25) + pt1.x, (topRail.y * .25) + pt1.y, (topRail.z * .25) + pt1.z);
					topRail = pt1 - pt3;
					pt3.set( (topRail.x * .25) + pt3.x, (topRail.y * .25) + pt3.y, (topRail.z * .25) + pt3.z);
					
					topRail = pt4 - pt2;
					pt2.set( (topRail.x * .25) + pt2.x, (topRail.y * .25) + pt2.y, (topRail.z * .25) + pt2.z);
					topRail = pt2 - pt4;
					pt4.set( (topRail.x * .25) + pt4.x, (topRail.y * .25) + pt4.y, (topRail.z * .25) + pt4.z);
					
					
					
					//float diffScale = movementPts[i].bodyPts[j].radius * .07;
					diffScale = movementPts[i].bodyPts[j].radius * .16;
					diffScale = diffScale > 15.f ? 15.f : diffScale;
					//float diffScale = .08f;
					// is there not a better way to do this? //
					pt1Diff = pt1 - planeCenter;
					pt1Diff.normalize();
					pt1Diff *= diffScale;
					pt2Diff = pt2 - planeCenter;
					pt2Diff.normalize();
					pt2Diff *= diffScale;
					pt3Diff = pt3 - planeCenter;
					pt3Diff.normalize();
					pt3Diff *= diffScale;
					pt4Diff = pt4 - planeCenter;
					pt4Diff.normalize();
					pt4Diff *= diffScale;
					
					
					extrusionPts[extrusionPtIndex    ].set( planeNormal.x + pt1.x + pt1Diff.x, planeNormal.y + pt1.y + pt1Diff.y, planeNormal.z + pt1.z + pt1Diff.z );
					extrusionPts[extrusionPtIndex + 1].set( planeNormal.x + pt2.x + pt2Diff.x, planeNormal.y + pt2.y + pt2Diff.y, planeNormal.z + pt2.z + pt2Diff.z );
					extrusionPts[extrusionPtIndex + 2].set( planeNormal.x + pt3.x + pt3Diff.x, planeNormal.y + pt3.y + pt3Diff.y, planeNormal.z + pt3.z + pt3Diff.z );
					extrusionPts[extrusionPtIndex + 3].set( planeNormal.x + pt4.x + pt4Diff.x, planeNormal.y + pt4.y + pt4Diff.y, planeNormal.z + pt4.z + pt4Diff.z );
				}
				
			}
			
		}
	}
	
}

void Tangles::draw() {
	Creature::draw();
	
	//ofxVec3f planeCenter, planeNormal, topRail;
	ofxVec3f pt1, pt2, pt3, pt4;
	//ofxVec3f pt1Diff, pt2Diff, pt3Diff, pt4Diff;
	int noteIndex;
	ofColor activeColor, inActiveColor;
	float colorPct, oneOverColorPct;
	
	int extrusionPtIndex = 0;
	
	//ofEnableAlphaBlending();
	
	float segNotesBodyPtsRatio = (float)CREATURE_NUM_NOTES_PER_SEGMENT / (float)CREATURE_NUM_BODY_PTS_PER_SEGMENT; // mostly .5 //
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			
			colorPct = spine[i].segColorPct;
			oneOverColorPct = 1 - colorPct;
			
			activeColor.r = (colorPct * extremetyHighColor.r) + (oneOverColorPct * extremetyBaseColor.r); 
			activeColor.g = (colorPct * extremetyHighColor.g) + (oneOverColorPct * extremetyBaseColor.g);
			activeColor.b = (colorPct * extremetyHighColor.b) + (oneOverColorPct * extremetyBaseColor.b); 
			activeColor.a = (colorPct * extremetyHighColor.a) + (oneOverColorPct * extremetyBaseColor.a);
			
			/* // Highlights all of the extremeties equally //
			inActiveColor.r = (colorPct * segmentHighColor.r) + (oneOverColorPct * segmentBaseColor.r); 
			inActiveColor.g = (colorPct * segmentHighColor.g) + (oneOverColorPct * segmentBaseColor.g);
			inActiveColor.b = (colorPct * segmentHighColor.b) + (oneOverColorPct * segmentBaseColor.b); 
			inActiveColor.a = (colorPct * segmentHighColor.a) + (oneOverColorPct * segmentBaseColor.a);
			*/
			
			// NO highlight of inactive extremeties //
			/*
			inActiveColor.r = extremetyBaseColor.r; 
			inActiveColor.g = extremetyBaseColor.g;
			inActiveColor.b = extremetyBaseColor.b; 
			inActiveColor.a = extremetyBaseColor.a;
			*/
			
			// highlights the inActive colors less than the active colors //
			colorPct = colorPct * .5;
			oneOverColorPct = 1 - colorPct;
			
			inActiveColor.r = (colorPct * segmentHighColor.r) + (oneOverColorPct * segmentBaseColor.r); 
			inActiveColor.g = (colorPct * segmentHighColor.g) + (oneOverColorPct * segmentBaseColor.g);
			inActiveColor.b = (colorPct * segmentHighColor.b) + (oneOverColorPct * segmentBaseColor.b); 
			inActiveColor.a = (colorPct * segmentHighColor.a) + (oneOverColorPct * segmentBaseColor.a);
			//////////////////////////////////////////////////////////////
			
			if (bRenderWireFrame) {
				if (i == segmentIndex) {
					//ofSetColor(segmentHighColor.r, segmentHighColor.g, segmentHighColor.b, segmentHighColor.a);
					activeColor.r = extremetyHighColor.r;
					activeColor.g = extremetyHighColor.g;
					activeColor.b = extremetyHighColor.b;
					activeColor.a = extremetyHighColor.a;
				} else if (i < segmentIndex) {
					//ofSetColor(segmentBaseColor.r, segmentBaseColor.g, segmentBaseColor.b, segmentBaseColor.a);
					activeColor.r = extremetyBaseColor.r;
					activeColor.g = extremetyBaseColor.g;
					activeColor.b = extremetyBaseColor.b;
					activeColor.a = extremetyBaseColor.a;
				} else {
					ofColor deadColor = ColorPalette::getInactiveCreatureColor();
					//ofSetColor(deadColor.r, deadColor.g, deadColor.b, deadColor.a);
					inActiveColor.r = deadColor.r;
					inActiveColor.g = deadColor.g;
					inActiveColor.b = deadColor.b;
					inActiveColor.a = deadColor.a;
				}
			}
			
			
			for(int j = 0; j < spine[i].numPts; j++) {
				if (movementPts[i].bodyPts[j].radius > 3) {
					noteIndex = ceil((float)j * segNotesBodyPtsRatio);
					//cout << "j: " << j << ", noteIndex: " << noteIndex << ", [bool]: " << movementPts[i].notes[noteIndex] << endl;
					// set the colors based on if note is active //
					if (movementPts[i].notes[noteIndex] == true) {
						ofSetColor(activeColor.r, activeColor.g, activeColor.b, activeColor.a);
					} else {
						ofSetColor(inActiveColor.r, inActiveColor.g, inActiveColor.b, inActiveColor.a);
					}
					
					extrusionPtIndex = (j * numMovementPts + i) * 4;
					
					pt1 = spine[i+1].bodyPts[j];
					pt2 = spine[i].bodyPts[j];
					pt3 = spine[i+1].bodyPts[j+1];
					pt4 = spine[i].bodyPts[j+1];
					if (j == spine[i].numPts - 1) {
						pt3 = spine[i+1].bodyPts[0];
						pt4 = spine[i].bodyPts[0];
					}
					
					
					
					
					
					/*
					ofxVec3f pt1Norm = (planeNormal+pt1+pt1Diff) - pt1;
					ofxVec3f pt2Norm = (planeNormal+pt2+pt2Diff) - pt2;
					ofxVec3f pt3Norm = (planeNormal+pt3+pt3Diff) - pt3;
					ofxVec3f pt4Norm = (planeNormal+pt4+pt4Diff) - pt4;
					 
					//if (i == 4) {
						
						//ofSetColor(255, 255, 255, 150);
						
						int numDivisions = planeNormal.length() / spine[i].extremetySpacing; // 1 for good luck //
						//cout << "This is the num of divs " << numDivisions << endl;
						float spacing = planeNormal.length() / (float) numDivisions;
						
						for (int k = 0; k < numDivisions; k++) {
							
							glBegin(GL_QUAD_STRIP);
							float currDist = (float)k * spacing + .2;
							//cout << currDist << endl;
							pt1Norm.normalize();
							pt1Norm *= currDist;
							//cout << "1. pt1Norm.length " << pt1Norm.length() << endl;
							glVertex3f(pt1.x + pt1Norm.x, pt1.y + pt1Norm.y, pt1.z + pt1Norm.z);
							pt1Norm.normalize();
							pt1Norm *= (currDist + spacing);
							//cout << "2. pt1Norm.length " << pt1Norm.length() << endl;
							glVertex3f(pt1.x + pt1Norm.x, pt1.y + pt1Norm.y, pt1.z + pt1Norm.z);
							//glVertex3f(0, 0, 0);
							
							pt2Norm.normalize();
							pt2Norm *= currDist;
							glVertex3f(pt2.x + pt2Norm.x, pt2.y + pt2Norm.y, pt2.z + pt2Norm.z);
							pt2Norm.normalize();
							pt2Norm *= (currDist + spacing);
							glVertex3f(pt2.x + pt2Norm.x, pt2.y + pt2Norm.y, pt2.z + pt2Norm.z);
							
							pt4Norm.normalize();
							pt4Norm *= currDist;
							glVertex3f(pt4.x + pt4Norm.x, pt4.y + pt4Norm.y, pt4.z + pt4Norm.z);
							pt4Norm.normalize();
							pt4Norm *= (currDist + spacing);
							glVertex3f(pt4.x + pt4Norm.x, pt4.y + pt4Norm.y, pt4.z + pt4Norm.z);
							
							pt3Norm.normalize();
							pt3Norm *= currDist;
							glVertex3f(pt3.x + pt3Norm.x, pt3.y + pt3Norm.y, pt3.z + pt3Norm.z);
							pt3Norm.normalize();
							pt3Norm *= (currDist + spacing);
							glVertex3f(pt3.x + pt3Norm.x, pt3.y + pt3Norm.y, pt3.z + pt3Norm.z);
							
							
							pt1Norm.normalize();
							pt1Norm *= currDist;
							glVertex3f(pt1.x + pt1Norm.x, pt1.y + pt1Norm.y, pt1.z + pt1Norm.z);
							pt1Norm.normalize();
							pt1Norm *= (currDist + spacing);
							glVertex3f(pt1.x + pt1Norm.x, pt1.y + pt1Norm.y, pt1.z + pt1Norm.z);
							
							
							glEnd();
								
							
						}
					
						ofxQuad(planeNormal.x + pt1.x + pt1Diff.x, planeNormal.y + pt1.y + pt1Diff.y, planeNormal.z + pt1.z + pt1Diff.z, 
								planeNormal.x + pt2.x + pt2Diff.x, planeNormal.y + pt2.y + pt2Diff.y, planeNormal.z + pt2.z + pt2Diff.z, 
								planeNormal.x + pt4.x + pt4Diff.x, planeNormal.y + pt4.y + pt4Diff.y, planeNormal.z + pt4.z + pt4Diff.z,
								planeNormal.x + pt3.x + pt3Diff.x, planeNormal.y + pt3.y + pt3Diff.y, planeNormal.z + pt3.z + pt3Diff.z 
								);
						*/
					
					
					
					glBegin(GL_QUADS);
					
					glVertex3f(pt1.x, pt1.y, pt1.z);
					glVertex3f(extrusionPts[extrusionPtIndex    ].x, extrusionPts[extrusionPtIndex    ].y, extrusionPts[extrusionPtIndex    ].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 1].x, extrusionPts[extrusionPtIndex + 1].y, extrusionPts[extrusionPtIndex + 1].z);
					glVertex3f(pt2.x, pt2.y, pt2.z);
								   
					// top //
					glVertex3f(extrusionPts[extrusionPtIndex    ].x, extrusionPts[extrusionPtIndex    ].y, extrusionPts[extrusionPtIndex    ].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 1].x, extrusionPts[extrusionPtIndex + 1].y, extrusionPts[extrusionPtIndex + 1].z); 
					glVertex3f(extrusionPts[extrusionPtIndex + 3].x, extrusionPts[extrusionPtIndex + 3].y, extrusionPts[extrusionPtIndex + 3].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 2].x, extrusionPts[extrusionPtIndex + 2].y, extrusionPts[extrusionPtIndex + 2].z); 
					/////////
					glVertex3f(pt3.x, pt3.y, pt3.z);
					glVertex3f(extrusionPts[extrusionPtIndex + 2].x, extrusionPts[extrusionPtIndex + 2].y, extrusionPts[extrusionPtIndex + 2].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 3].x, extrusionPts[extrusionPtIndex + 3].y, extrusionPts[extrusionPtIndex + 3].z);
					glVertex3f(pt4.x, pt4.y, pt4.z);
							
					glVertex3f(pt1.x, pt1.y, pt1.z);
					glVertex3f(extrusionPts[extrusionPtIndex    ].x, extrusionPts[extrusionPtIndex    ].y, extrusionPts[extrusionPtIndex    ].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 2].x, extrusionPts[extrusionPtIndex + 2].y, extrusionPts[extrusionPtIndex + 2].z);
					glVertex3f(pt3.x, pt3.y, pt3.z);
							
					glVertex3f(pt2.x, pt2.y, pt2.z);
					glVertex3f(extrusionPts[extrusionPtIndex + 1].x, extrusionPts[extrusionPtIndex + 1].y, extrusionPts[extrusionPtIndex + 1].z);
					glVertex3f(extrusionPts[extrusionPtIndex + 3].x, extrusionPts[extrusionPtIndex + 3].y, extrusionPts[extrusionPtIndex + 3].z);
					glVertex3f(pt4.x, pt4.y, pt4.z);
					
					glEnd();
					
					
					//ofSetColor(255, 0, 0);
					//ofSetColor(0, 0, 255);
					/*
					glBegin(GL_LINES);
					
					glVertex3f(pt1.x, pt1.y, pt1.z);
					glVertex3f(planeNormal.x + pt1.x + pt1Diff.x, planeNormal.y + pt1.y + pt1Diff.y, planeNormal.z + pt1.z + pt1Diff.z);
					
					glVertex3f(planeNormal.x + pt1.x + pt1Diff.x, planeNormal.y + pt1.y + pt1Diff.y, planeNormal.z + pt1.z + pt1Diff.z);
					glVertex3f(planeNormal.x + pt2.x + pt2Diff.x, planeNormal.y + pt2.y + pt2Diff.y, planeNormal.z + pt2.z + pt2Diff.z);
					
					glVertex3f(pt2.x, pt2.y, pt2.z);
					glVertex3f(planeNormal.x + pt2.x + pt2Diff.x, planeNormal.y + pt2.y + pt2Diff.y, planeNormal.z + pt2.z + pt2Diff.z);
					
					glVertex3f(planeNormal.x + pt2.x + pt2Diff.x, planeNormal.y + pt2.y + pt2Diff.y, planeNormal.z + pt2.z + pt2Diff.z);
					glVertex3f(planeNormal.x + pt4.x + pt4Diff.x, planeNormal.y + pt4.y + pt4Diff.y, planeNormal.z + pt4.z + pt4Diff.z);
					
					glVertex3f(pt3.x, pt3.y, pt3.z);
					glVertex3f(planeNormal.x + pt3.x + pt3Diff.x, planeNormal.y + pt3.y + pt3Diff.y, planeNormal.z + pt3.z + pt3Diff.z);
					
					glVertex3f(planeNormal.x + pt3.x + pt3Diff.x, planeNormal.y + pt3.y + pt3Diff.y, planeNormal.z + pt3.z + pt3Diff.z);
					glVertex3f(planeNormal.x + pt4.x + pt4Diff.x, planeNormal.y + pt4.y + pt4Diff.y, planeNormal.z + pt4.z + pt4Diff.z);
					
					glVertex3f(pt4.x, pt4.y, pt4.z);
					glVertex3f(planeNormal.x + pt4.x + pt4Diff.x, planeNormal.y + pt4.y + pt4Diff.y, planeNormal.z + pt4.z + pt4Diff.z);
					
					glVertex3f(planeNormal.x + pt1.x + pt1Diff.x, planeNormal.y + pt1.y + pt1Diff.y, planeNormal.z + pt1.z + pt1Diff.z);
					glVertex3f(planeNormal.x + pt3.x + pt3Diff.x, planeNormal.y + pt3.y + pt3Diff.y, planeNormal.z + pt3.z + pt3Diff.z);
					
					glEnd();
					*/ 
						
					//glLineWidth (1.f);
				}
			}
		}
	}
	//ofDisableAlphaBlending();
}