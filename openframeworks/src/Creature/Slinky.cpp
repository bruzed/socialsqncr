/*
 *  Slinky.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Slinky.h"

void Slinky::initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength) {
	Creature::initiate($numPts, $numSegmentPts, $maxRadius, $spineLength, $extremetySpacing, $tentacleDivLength); // sets the tentacle length //
	
	//cout << endl << "ExremetySpacing argument is " << $extremetySpacing << endl << endl;
	type = CREATURE_TYPE_SLINKY;
	
	for (int i = 0; i < numBodyPts; i++) {
		//spine[i].extremetySpacing = 15.f;
		spine[i].extremetySpacing = $extremetySpacing;
		spine[i].numTentacles = spine[i].numPts - 1;
		for (int j = 0; j < spine[i].numTentacles; j++) {
			Tentacle tent;
			spine[i].tentacles.push_back(tent);
			// tentacles in each CreatureSegment are initiated, but do not have the points in them yet, which will
			// be determined by the length of the movement pts 
		}
	}
	
	bFirstRun = true;
}


void Slinky::update() {
	Creature::update();
	
	// set up tentacles based on normals //
	if (bFirstRun) {
		Creature::update();
		Creature::update();
		Creature::update();
		ofxVec3f planeNormal, planeCenter, v1, v2;
		int planeNormalIndex = 0;
		//cout << "Slinky :: update : this is the first run for the slinky man" << endl;
		
		if (spine[0].tentacles.size() > 0) {
			for (int i = 0; i < numBodyPts; i++) {
				if (i < numBodyPts - 1) {
					for(int j = 0; j < spine[i].numPts; j++) {
						if (j < spine[i].numTentacles) {
							spine[i].tentacles[j].clearPoints();
						}
					}
				}
			}
		}
		
		for (int i = 0; i < numBodyPts; i++) {
			if (i < numBodyPts - 1) {
				for(int j = 0; j < spine[i].numPts; j++) {
					
					// this code is redundant as a mofo //
					
					int pt2Index = j+1;
					if (j == spine[i].numPts - 1) {
						pt2Index = 0;
					} 
					//v1.set(spine[i+1].bodyPts[j] - spine[i].bodyPts[j]);
					v1 = spine[i+1].bodyPts[j] - spine[i].bodyPts[j];
					v2 = spine[i+1].bodyPts[j] - spine[i+1].bodyPts[pt2Index];
					
					if (i == numBodyPts - 2) {
						v1 = spine[i].bodyPts[j] - spine[i].bodyPts[pt2Index];
						v2 = spine[i].bodyPts[j] - spine[i+1].bodyPts[j];
					}
					
					planeNormalIndex = j * numMovementPts + i;
					planeNormal.set( planeNormals[planeNormalIndex]->x, planeNormals[planeNormalIndex]->y, planeNormals[planeNormalIndex]->z );
					
					//ofxVec3f planeCenter = (spine[i].bodyPts[pt2Index] + spine[i+1].bodyPts[j]) * .5;
					planeCenter.set( planeCenters[planeNormalIndex]->x, planeCenters[planeNormalIndex]->y, planeCenters[planeNormalIndex]->z );
					
					float planeLength = movementPts[i].bodyPts[j].radius;
					
					float tentacleRadius = v2.length() * .5;
					
					if (i == numBodyPts - 2) tentacleRadius = v1.length() * .5;
					
					//cout  << "This is the v2.length " << v2.length() << endl;
					//cout << "Slinky :: update planeLength = " << planeLength << ", spine[i].extremetySpacing = " << spine[i].extremetySpacing << endl;
					
					//float tentacleRadius = 3.f;
					tentacleRadius = tentacleRadius < 0 ? tentacleRadius * -1 : tentacleRadius;
					
					if (j < spine[i].numTentacles) {
						//if ( i == 4) cout << "tentacleDivLength = " << tentacleDivLength << endl;
						//int numTentaclePts = round(planeLength / spine[i].extremetySpacing);
						int numTentaclePts = round(planeLength / tentacleDivLength);
						//cout << "Slinky :: update : numTentaclePts adding to tentacle " << numTentaclePts << endl;
						//cout << "Slinky :: Update : spine[" << i << "].tentacles[" << j << "].numPts = " << numTentaclePts << endl;
						spine[i].tentacles[j].numPts = ((float)numTentaclePts * 2);
						//spine[i].tentacles[j].halfPts = numTentaclePts;
						if (numTentaclePts > 1) {
							spine[i].tentacles[j].addPoint(planeCenter.x, planeCenter.y, planeCenter.z);
							spine[i].tentacles[j].tentacleSpacing = spine[i].extremetySpacing;
							
							for (int k = 0; k < spine[i].tentacles[j].numPts; k++) {
								ofxVec3f tempLoc = planeNormal * ((float)k * spine[i].extremetySpacing + spine[i].extremetySpacing);
								// add in the tentacles along the path of the normal for now //
								spine[i].tentacles[j].addPoint(tempLoc.x + planeCenter.x, tempLoc.y + planeCenter.y, tempLoc.z + planeCenter.z);
							}
							spine[i].tentacles[j].numPts = spine[i].tentacles[j].pts.size();
							
							
							for (int k = 0; k < spine[i].tentacles[j].numPts; k++) {
								spine[i].tentacles[j].pts[k].radius = tentacleRadius * (1-((float)k/(float)spine[i].tentacles[j].numPts)) + .1;
								//spine[i].tentacles[j].pts[k].radius = 2.f;
								//cout << "spine[" << i << "].tentacles[" << j << "].pts[" << k << "].radius = " << spine[i].tentacles[j].pts[k].radius << endl;
							}
						} else {
							spine[i].tentacles[j].numPts = 0;
							spine[i].tentacles[j].halfPts = 0;
						}
					}
				}
			}
		}
		
		bFirstRun = false;
	}
	
	ofxVec3f planeCenter, planeNormal, dirNormal;
	ofxVec3f T1, T2, A, v2, tempLoc;
	ofxVec3f approx_up, new_side, new_up;
	float alpha;
	int planeNormalIndex;
	float springMult = 0.f;
	float springStrength = 0.f;
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			for(int j = 0; j < spine[i].numTentacles; j++) {
				
				if (spine[i].tentacles[j].numPts > 1) {
					
					v2 = (spine[i+1].bodyPts[j] - spine[i].bodyPts[j]);
					
					planeNormalIndex = j * numMovementPts + i;
					planeNormal.set( planeNormals[planeNormalIndex]->x, planeNormals[planeNormalIndex]->y, planeNormals[planeNormalIndex]->z );
					planeCenter.set( planeCenters[planeNormalIndex]->x, planeCenters[planeNormalIndex]->y, planeCenters[planeNormalIndex]->z );
					
					for(int k = 0; k < spine[i].tentacles[j].numPts; k++) {
						
						if (i > 0 && i < numBodyPts - 1) {
							dirNormal = spine[i].tentacles[j].pts[k-1].loc - spine[i].tentacles[j].pts[k].loc;
						} else if (i == numBodyPts - 1) {
							dirNormal = spine[i].tentacles[j].pts[k-1].loc - spine[i].tentacles[j].pts[k].loc;
						} else if (i == 0) {
							dirNormal = spine[i].tentacles[j].pts[k].loc - spine[i].tentacles[j].pts[k+1].loc;
						}
						dirNormal.normalize();
						
						if (k == 0) {
							spine[i].tentacles[j].pts[0].loc.set(planeCenter.x, planeCenter.y, planeCenter.z);
							spine[i].tentacles[j].orientations[0].up.set(-v2.x, -v2.y, -v2.z);
							spine[i].tentacles[j].orientations[0].up.normalize();
							//spine[i].tentacles[j].orientations[0].forward.set(planeNormal.x, planeNormal.y, planeNormal.z);
							spine[i].tentacles[j].orientations[0].forward.set(dirNormal.x, dirNormal.y, dirNormal.z);
						} else {
							
							//if (k < spine[i].tentacles[j].halfPts) {
							tempLoc = (planeNormal * ((float)k * spine[i].extremetySpacing + spine[i].extremetySpacing)) * deathPct; // kill this cat !!//
							tempLoc += planeCenter;
							if (i == segmentIndex) {
								springMult = .95;
							} else {
								springMult = .15;
							}
							springStrength = ((1 - (float)k / (float)spine[i].tentacles[j].numPts)) * springMult;
							
							spine[i].tentacles[j].pts[k].fixedSpring(tempLoc, springStrength, spine[i].extremetySpacing, false);
							
							T1.set(spine[i].tentacles[j].pts[k-1].loc.x,
										spine[i].tentacles[j].pts[k-1].loc.y,
										spine[i].tentacles[j].pts[k-1].loc.z
										);
							T2.set(spine[i].tentacles[j].pts[k].loc.x,
										spine[i].tentacles[j].pts[k].loc.y,
										spine[i].tentacles[j].pts[k].loc.z
										);
							A = T1.crossed(T2);
							alpha = T1.angleRad(T2);
							
							approx_up.set(-spine[i].tentacles[j].orientations[k-1].up.x, 
											   -spine[i].tentacles[j].orientations[k-1].up.y, 
											   -spine[i].tentacles[j].orientations[k-1].up.z);
							approx_up.normalize();
							approx_up.rotateRad(alpha, A);
							//ofxVec3f new_forward(dirNormal.x, dirNormal.y, dirNormal.z);
							//ofxVec3f new_side = new_forward.crossed(approx_up);
							new_side = dirNormal.crossed(approx_up);
							new_up = dirNormal.crossed( new_side );
							//ofxVec3f new_up = new_forward.crossed(new_side);
							
							spine[i].tentacles[j].orientations[k].up.set(new_up.x, new_up.y, new_up.z);
							//spine[i].tentacles[j].orientations[k].forward.set(new_forward.x, new_forward.y, new_forward.z);
							spine[i].tentacles[j].orientations[k].forward.set(dirNormal.x, dirNormal.y, dirNormal.z);
							spine[i].tentacles[j].orientations[k].side.set(new_side.x, new_side.y, new_side.z);
						}
					}
					
					spine[i].tentacles[j].update();
				}
			}
			
		}
		
	}
}


void Slinky::draw() {
	Creature::draw();
	int tentacleRes = 3;
	int colorSegIndex, noteIndex;
	float colorPct, oneOverColorPct;
	float tentacleAngleInc = TWO_PI / (float)tentacleRes;
	ofColor activeColor, inActiveColor;
	ofxVec3f cross;
	float segNotesBodyPtsRatio = (float)CREATURE_NUM_NOTES_PER_SEGMENT / (float)CREATURE_NUM_BODY_PTS_PER_SEGMENT; // mostly .5 //
	
	//ofEnableAlphaBlending();
	
	ofxVec3f loczors[tentacleRes];
	ofxVec3f firstLoc;
	
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			for(int j = 0; j < spine[i].numPts; j++) {
				
				if (j < spine[i].numTentacles) {
					
					colorSegIndex = (float)spine[i].tentacles[j].pts.size() * spine[i].segColorPosPct;
					for(int k = 0; k < spine[i].tentacles[j].pts.size(); k++) {
						
						float diff = k - colorSegIndex;
						colorPct = 0.f;
						
						if (diff <= 0) {
							colorPct = 1 + (diff * .02); // since diff is negative //
							if (colorPct < 0) colorPct = 0;
						}
						oneOverColorPct = 1 - colorPct;
						/*
						ofSetColor((colorPct * extremetyHighColor.r) + (oneOverColorPct * extremetyBaseColor.r), 
								   (colorPct * extremetyHighColor.g) + (oneOverColorPct * extremetyBaseColor.g), 
								   (colorPct * extremetyHighColor.b) + (oneOverColorPct * extremetyBaseColor.b),
								   (colorPct * extremetyHighColor.a) + (oneOverColorPct * extremetyBaseColor.a));
						*/
						
						noteIndex = ceil((float)j * segNotesBodyPtsRatio);
						noteIndex = noteIndex > CREATURE_NUM_NOTES_PER_SEGMENT ? CREATURE_NUM_NOTES_PER_SEGMENT : noteIndex;
						// set the colors based on if note is active //
						if (movementPts[i].notes[noteIndex] == true) {
							activeColor.r = (colorPct * extremetyHighColor.r) + (oneOverColorPct * extremetyBaseColor.r); 
							activeColor.g = (colorPct * extremetyHighColor.g) + (oneOverColorPct * extremetyBaseColor.g);
							activeColor.b = (colorPct * extremetyHighColor.b) + (oneOverColorPct * extremetyBaseColor.b); 
							activeColor.a = (colorPct * extremetyHighColor.a) + (oneOverColorPct * extremetyBaseColor.a);
							
							ofSetColor(activeColor.r, activeColor.g, activeColor.b, activeColor.a);
						} else {
							/*
							inActiveColor.r = (colorPct * segmentHighColor.r) + (oneOverColorPct * segmentBaseColor.r); 
							inActiveColor.g = (colorPct * segmentHighColor.g) + (oneOverColorPct * segmentBaseColor.g);
							inActiveColor.b = (colorPct * segmentHighColor.b) + (oneOverColorPct * segmentBaseColor.b); 
							inActiveColor.a = (colorPct * extremetyHighColor.a) + (oneOverColorPct * extremetyBaseColor.a);
							 */
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
							
							
							ofSetColor(inActiveColor.r, inActiveColor.g, inActiveColor.b, inActiveColor.a);
						}
						
						// only used in the record scene, to keep track of the current index //
						if (bRenderWireFrame) {
							if (i == segmentIndex) {
								ofSetColor(segmentHighColor.r, segmentHighColor.g, segmentHighColor.b, segmentHighColor.a);
							} else if (i < segmentIndex) {
								ofSetColor(segmentBaseColor.r, segmentBaseColor.g, segmentBaseColor.b, segmentBaseColor.a);
							} else {
								ofColor deadColor = ColorPalette::getInactiveCreatureColor();
								ofSetColor(deadColor.r, deadColor.g, deadColor.b, deadColor.a);
							}
						}
						
						
						// segColorPosPct
						//float angle = 0.f;
						float dist = spine[i].tentacles[j].pts[k].radius;
						//float dist = 4.f;
						if ( k > 0) {
							glBegin(GL_TRIANGLE_STRIP);
						}
						
						cross = spine[i].tentacles[j].orientations[k].forward.perpendiculared(spine[i].tentacles[j].orientations[k].up);
						cross.normalize();
						cross *= dist * MAX(deathPct, .1);
						for (int m = 0; m < tentacleRes; m++) {
							if (m > 0) cross.rotateRad( tentacleAngleInc, spine[i].tentacles[j].orientations[k].forward);
							
							if ( k > 0) {
								glVertex3f(spine[i].tentacles[j].pts[k].loc.x + cross.x, spine[i].tentacles[j].pts[k].loc.y + cross.y, spine[i].tentacles[j].pts[k].loc.z + cross.z);
								glVertex3f(loczors[m].x, loczors[m].y, loczors[m].z);
							}
							if (m == tentacleRes - 1) {
								
								glVertex3f(loczors[0].x, loczors[0].y, loczors[0].z);
								glVertex3f(firstLoc.x, firstLoc.y, firstLoc.z);
								
							}
							
							loczors[m].set(spine[i].tentacles[j].pts[k].loc.x + cross.x, spine[i].tentacles[j].pts[k].loc.y + cross.y, spine[i].tentacles[j].pts[k].loc.z + cross.z);
							
							//ofSetColor(255, 0, 0);
							//glPushMatrix();
							//glTranslatef(spine[i].tentacles[j].pts[k].loc.x + cross.x, spine[i].tentacles[j].pts[k].loc.y + cross.y, spine[i].tentacles[j].pts[k].loc.z + cross.z);
							//ofCircle(0, 0, 1);
							//glPopMatrix();
							
							//angle += tentacleAngleInc;
						}
						firstLoc.set(loczors[0].x, loczors[0].y, loczors[0].z);
						
						if ( k > 0) {
							glEnd();
						}
						
					}
				}
			}
		}
	}
	//ofDisableAlphaBlending();
}



