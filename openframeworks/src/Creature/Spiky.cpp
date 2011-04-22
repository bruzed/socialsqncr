/*
 *  Spiky.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Spiky.h"

void Spiky::initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength) {
	Creature::initiate($numPts, $numSegmentPts, $maxRadius, $spineLength, $extremetySpacing, $tentacleDivLength);
	
	type = CREATURE_TYPE_SPIKEY;
	
	for (int i = 0; i < totalPlaneNormals; i++) {
		Particle3D pt;
		spikeParticles.push_back( pt );
	}
	bFirstRun = true;
}

void Spiky::update() {
	Creature::update();
	
	ofxVec3f planeNormal, planeCenter;
	int planeNormalIndex = 0;
	
	if (bFirstRun) {
		Creature::update();
		Creature::update();
		Creature::update();
	}
	
	float springStrength = 0.f;
	ofxVec3f bumpFrc(0, 0, 0);
	bool bBumpIt = false;
	float hollywoodBumpIt = 20.f;
	for (int i = 0; i < numMovementPts; i++) {
		if (i == segmentIndex) {
			springStrength = .95;
			bBumpIt = true;
		} else {
			bBumpIt = false;
			springStrength = .22;
		}
		for(int j = 0; j < spine[i].numPts; j++) {
			if (movementPts[i].bodyPts[j].radius > 3 || bFirstRun) {
				planeNormalIndex = j * numMovementPts + i;
				
				planeNormal.set( planeNormals[planeNormalIndex]->x, planeNormals[planeNormalIndex]->y, planeNormals[planeNormalIndex]->z );
				planeCenter.set( planeCenters[planeNormalIndex]->x, planeCenters[planeNormalIndex]->y, planeCenters[planeNormalIndex]->z );
				
				if (bBumpIt) {
					bumpFrc.set( planeNormal.x, planeNormal.y, planeNormal.z);
					bumpFrc *= hollywoodBumpIt;
				}
				
				planeNormal *= movementPts[i].bodyPts[j].radius * deathPct; // this cat is dying? //
				planeNormal += planeCenter;
				
				if (bBumpIt) {
					planeNormal += bumpFrc * deathPct;
				}
				
				spikeParticles[planeNormalIndex].fixedSpring( planeNormal, springStrength, 10.f, false );
				
				spikeParticles[planeNormalIndex].addDamping( spikeParticles[planeNormalIndex].DAMPING );
				spikeParticles[planeNormalIndex].vel += spikeParticles[planeNormalIndex].acc;
				spikeParticles[planeNormalIndex].loc += spikeParticles[planeNormalIndex].vel;
				spikeParticles[planeNormalIndex].acc.set(0, 0, 0);
				
				if (bFirstRun) {
					spikeParticles[planeNormalIndex].DAMPING = .1;
					spikeParticles[planeNormalIndex].loc.set(planeNormal.x, planeNormal.y, planeNormal.z);
				}
			}
			
		}
	}
	if (bFirstRun) bFirstRun = false;
}

void Spiky::draw() {
	Creature::draw();
	float segNotesBodyPtsRatio = (float)CREATURE_NUM_NOTES_PER_SEGMENT / (float)CREATURE_NUM_BODY_PTS_PER_SEGMENT; // mostly .5 //
	ofColor activeColor, inActiveColor;
	ofxVec3f planeNormal, planeCenter;
	ofxVec3f pt1, pt2, pt3, pt4;
	int planeNormalIndex;
	int noteIndex;
	float colorPct, oneOverColorPct;
	
	//ofEnableAlphaBlending();
	
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			
			colorPct = spine[i].segColorPct;
			oneOverColorPct = 1 - colorPct;
			
			activeColor.r = (colorPct * extremetyHighColor.r) + (oneOverColorPct * extremetyBaseColor.r); 
			activeColor.g = (colorPct * extremetyHighColor.g) + (oneOverColorPct * extremetyBaseColor.g);
			activeColor.b = (colorPct * extremetyHighColor.b) + (oneOverColorPct * extremetyBaseColor.b); 
			activeColor.a = (colorPct * extremetyHighColor.a) + (oneOverColorPct * extremetyBaseColor.a);
			
			// highlights the same as the active notes //
			/* 
			inActiveColor.r = (colorPct * segmentHighColor.r) + (oneOverColorPct * segmentBaseColor.r); 
			inActiveColor.g = (colorPct * segmentHighColor.g) + (oneOverColorPct * segmentBaseColor.g);
			inActiveColor.b = (colorPct * segmentHighColor.b) + (oneOverColorPct * segmentBaseColor.b); 
			inActiveColor.a = (colorPct * segmentHighColor.a) + (oneOverColorPct * segmentBaseColor.a);
			*/
			
			// does not highlight inactive notes //
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
				
					planeNormalIndex = j * numMovementPts + i;
					planeNormal.set( planeNormals[planeNormalIndex]->x, planeNormals[planeNormalIndex]->y, planeNormals[planeNormalIndex]->z );
					planeCenter.set( planeCenters[planeNormalIndex]->x, planeCenters[planeNormalIndex]->y, planeCenters[planeNormalIndex]->z );
					
					planeNormal *= movementPts[i].bodyPts[j].radius * deathPct; // this cat is dying? //
					
					pt1 = spine[i+1].bodyPts[j];
					pt2 = spine[i].bodyPts[j];
					pt3 = spine[i+1].bodyPts[j+1];
					pt4 = spine[i].bodyPts[j+1];
					if (j == spine[i].numPts - 1) {
						pt3 = spine[i+1].bodyPts[0];
						pt4 = spine[i].bodyPts[0];
					}
					
					// colorize this piece //
					noteIndex = ceil((float)j * segNotesBodyPtsRatio);
					
					//cout << "j: " << j << ", noteIndex: " << noteIndex << ", [bool]: " << movementPts[i].notes[noteIndex] << endl;
					// set the colors based on if note is active //
					if (movementPts[i].notes[noteIndex] == true) {
						ofSetColor(activeColor.r, activeColor.g, activeColor.b, activeColor.a);
					} else {
						ofSetColor(inActiveColor.r, inActiveColor.g, inActiveColor.b, inActiveColor.a);
					}
					
					//ofSetColor(255, 255, 255, 150);
					
					ofxQuad(pt1.x, pt1.y, pt1.z,
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z, 
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z,
							pt2.x, pt2.y, pt2.z
							);
					ofxQuad(pt3.x, pt3.y, pt3.z,
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z, 
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z,
							pt4.x, pt4.y, pt4.z
							);
					ofxQuad(pt1.x, pt1.y, pt1.z,
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z, 
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z,
							pt3.x, pt3.y, pt3.z
							);
					ofxQuad(pt2.x, pt2.y, pt2.z,
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z, 
							spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z,
							pt4.x, pt4.y, pt4.z
							);
					
					
					//if (i == segmentIndex) {
						ofSetColor(255, 255, 255);
					//}
					/*
					glBegin(GL_LINES);
					
					glVertex3f(pt1.x, pt1.y, pt1.z);
					glVertex3f(spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z);
					
					glVertex3f(pt2.x, pt2.y, pt2.z);
					glVertex3f(spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z);
					
					glVertex3f(pt3.x, pt3.y, pt3.z);
					glVertex3f(spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z);
					
					glVertex3f(pt4.x, pt4.y, pt4.z);
					glVertex3f(spikeParticles[planeNormalIndex].loc.x, spikeParticles[planeNormalIndex].loc.y, spikeParticles[planeNormalIndex].loc.z);
					
					glEnd();
					*/
				}
				
				
				
				//glLineWidth (1.f);
			}
		}
	}
	//ofDisableAlphaBlending();
}