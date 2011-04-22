/*
 *  SelectCreatureScene.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SelectCreatureScene.h"

void SelectCreatureScene::setUpPanel(ofxControlPanel* $panel) {
	panel = $panel;
	
	panel->addPanel("Select Creature Scene", 2, false);
	panel->setWhichPanel("Select Creature Scene");
	panel->setWhichColumn(0);
	
	//addSlider2D(string sliderName, string xmlName, float valueX, float valueY, float minX, float maxX, float minY, float maxY, bool isInt)
	panel->addSlider2D("0 Loc", "SC_CREATURE_0_LOC", 550.f, 500.f, 0.f, 1000.f, 100.f, ofGetHeight(), false);
	panel->addSlider("0 loc.z", "SC_CREATURE_0_LOC_Z", 600.f, 0.f, 1000.f, false);
	panel->addSlider2D("0 rotX Y", "SC_CREATURE_0_ROT_X_Y", 0.f, 0.f, -90.f, 90.f, -90.f, 90.f, false);
	panel->addSlider2D("1 Loc", "SC_CREATURE_1_LOC", 550.f, 500.f, 0.f, 1000.f, 100.f, ofGetHeight(), false);
	panel->addSlider("1 loc.z", "SC_CREATURE_1_LOC_Z", 600.f, 0.f, 1000.f, false);
	panel->addSlider2D("1 rotX Y", "SC_CREATURE_1_ROT_X_Y", 0.f, 0.f, -90.f, 90.f, -90.f, 90.f, false);
	panel->setWhichColumn(1);
	panel->addSlider2D("2 Loc", "SC_CREATURE_2_LOC", 550.f, 500.f, 0.f, 1000.f, 100.f, ofGetHeight(), false);
	panel->addSlider("2 loc.z", "SC_CREATURE_2_LOC_Z", 600.f, 0.f, 1000.f, false);
	panel->addSlider2D("2 rotX Y", "SC_CREATURE_2_ROT_X_Y", 0.f, 0.f, -90.f, 90.f, -90.f, 90.f, false);
	
}

void SelectCreatureScene::initiate() {
	up.set(0, 1, 0);
	side.set(1, 0, 0);
	center.set(ofGetWidth() * .5, ofGetHeight() * .5, 0.f);
	selectedCreatureIndex = -1;
	rotYinc = .1f;
	
	for(int i = 0; i < 3; i++) {
		creatures[i] = new SelectCreature();
	}
	
	creatures[0]->creature = new Tangles();
	creatures[1]->creature = new Spiky();
	creatures[2]->creature = new Slinky();
	
	screenLocs = new double[3 * 3];
	
	// cout << endl << "ExremetySpacing argument is " << panel->getValueF("F_EXTREMETY_SPACING") << endl << endl;
	
	for (int i = 0; i < 3; i++) {
		creatures[i]->creature->initiate(panel->getValueI("I_NUM_CREATURE_SEGS"), CREATURE_NUM_BODY_PTS_PER_SEGMENT, panel->getValueF("F_SPINE_MAX_RAD"),
										 panel->getValueF("F_SPINE_LENGTH"), panel->getValueF("F_EXTREMETY_SPACING"), panel->getValueF("F_TENTACLE_DIV_LENGTH"));
		//creatures[i]->creature->setUpForRecording(center.x, center.y, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"));
		// creatures[i]->creature->radDiffScalar = panel->getValueF("CREATURE_RAD_DIFF_MULT"); // now in RecordScene //
		creatures[i]->creature->setBodyRadiusFromMotion( .5f, .5f, panel->getValueF("F_SPINE_MIN_RAD"), panel->getValueF("F_SPINE_MAX_RAD") );
		//ofColor color = ColorPalette::getCurrentColorForCreature( i + 1 );
		//ofColor baseColor = ColorPalette::getBaseBodyColor();
		ofColor highColor = ColorPalette::getWhiteHighlightColor();
		ofColor lowColor = ColorPalette::getCurrentColorForCreature( i + 1 );
		creatures[i]->creature->setColors( highColor.r, highColor.g, highColor.b, highColor.a, lowColor.r, lowColor.g, lowColor.b, lowColor.a );
		creatures[i]->creature->segmentIndex = -1;
		creatures[i]->rect.width = 2 * creatures[i]->creature->length + 20.f;
		creatures[i]->rect.height = creatures[i]->rect.width;
	}
	
}

void SelectCreatureScene::advanceSegments() {
	for (int i = 0; i < 3; i++) {
		creatures[i]->creature->advanceSegment();
	}
}

void SelectCreatureScene::animateIn() {
	lastBtnChecktime = ofGetElapsedTimeMillis();
	
	cout << "Select Creature animateIn() center.x =  "<< center.x << endl;
	bAnimInComplete = false;
	bTransitioning = true;
	bAnimOutComplete = true;
	bButtonActivated = false;
	selectedCreatureIndex = -1;
	int i = 0;
	for (i = 0; i < 3; i++) {
		creatures[i]->hitPct = 0.f;
		//recordScene->setRandomPointsForCreature(creatures[i]->creature, center, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"),
		//										panel->getValueF("CREATURE_RAD_DIFF_MULT"), panel->getValueF("CREATURE_RAND_MAX_DIFF"));
		
		recordScene->setRandomPointsForCreature( creatures[i]->creature, center, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"),
												panel->getValueF("CREATURE_RAD_DIFF_MULT"), panel->getValueF("CREATURE_RAND_MAX_DIFF"));
		
		creatures[i]->creature->setLifeAtts( 1.f, 10000, 1000, .01f ); // keep these kids alive //
		//recordScene->setRandomNotesHitForCreature( renderScene->creatures[cIndex] );
		//recordScene->addRadiusForNotesHit( renderScene->creatures[cIndex], panel.getValueF("F_ADD_RADIUS_FOR_NOTE_HIT") );
		//creatures[i]->creature->setRandomRecordingSegmentLocations(panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"));
		//creatures[i]->creature->setMovementPtsRadius( center, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"), creatures[i]->creature->radDiffScalar); //creatures[i]->creature->radDiffScalar
	}
	setCreatureAttsFromPanel();
	
	float tarX = creatures[0]->center.x;
	creatures[0]->center.x = tarX - 200;
	Tweenzor::add( &creatures[0]->center.x, creatures[0]->center.x , tarX, 0.0f, 1.2f, EASE_OUT_QUART );
	Tweenzor::add( &creatures[1]->center.y, creatures[1]->center.y + 50, creatures[1]->center.y, 0.f, 1.f, EASE_OUT_QUART );
	tarX = creatures[2]->center.x;
	creatures[2]->center.x = tarX + 200;
	Tweenzor::add( &creatures[2]->center.x, creatures[2]->center.x, tarX, 0.0f, 1.2f, EASE_OUT_QUART );
	
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &SelectCreatureScene::onAnimInComplete);
}

void SelectCreatureScene::onAnimInComplete(float args) {
	cout << "Select Creature Scene animation complete " << endl;
	bAnimInComplete = true;
	bTransitioning = false;
}
// make sure to set selectedCreatureIndex before calling this function !!! //
void SelectCreatureScene::animateOut() {
	bTransitioning = true;
	bAnimInComplete = false; // so that the cv does not check the image //
	bAnimOutComplete = false;
	for (int i = 0; i < 3; i++) {
		if (i == selectedCreatureIndex) {
			Tweenzor::add( &creatures[i]->center.x, creatures[i]->center.x, center.x, 0.2f, 1.f, EASE_IN_OUT_QUART );
			Tweenzor::add( &creatures[i]->center.y, creatures[i]->center.y, center.y, 0.2f, 1.f, EASE_IN_OUT_QUART );
		} else {
			if (i == 0) {
				Tweenzor::add( &creatures[i]->center.x, creatures[i]->center.x, creatures[i]->center.x - 550, 0.f, .5f, EASE_IN_QUART );
			} else if (i == 1) {
				Tweenzor::add( &creatures[i]->center.y, creatures[i]->center.y, creatures[i]->center.y - 650, 0.f, .5f, EASE_IN_QUART );
			} else if(i == 2) {
				Tweenzor::add( &creatures[i]->center.x, creatures[i]->center.x, creatures[i]->center.x + 550, 0.f, .5f, EASE_IN_QUART );
			}
		}
	}
	Tweenzor::add( &creatures[selectedCreatureIndex]->rotX, creatures[selectedCreatureIndex]->rotX, 0, 1.2f, .3f, EASE_IN_OUT_QUART );
	Tweenzor::add( &creatures[selectedCreatureIndex]->rotY, creatures[selectedCreatureIndex]->rotY, -180, 1.2f, .6f, EASE_IN_OUT_QUART );
	Tweenzor::add( &creatures[selectedCreatureIndex]->center.z, creatures[selectedCreatureIndex]->center.z, 1000, 1.8f, .6f, EASE_IN_QUART );
	Tweenzor::getTween( &creatures[selectedCreatureIndex]->center.z )->addListener( Tween::COMPLETE, this, &SelectCreatureScene::onAnimOutComplete);
}

void SelectCreatureScene::onAnimOutComplete(float args) {
	cout << "Select Creature Scene :: animateOutComplete " << endl;
	bAnimOutComplete = true;
	bTransitioning = false;
}

void SelectCreatureScene::setCreatureAttsFromPanel() {
	float rotYpadding = 30.f;
	ofxVec3f dirNorm(0, 0, 1);
	creatures[0]->rotY = panel->getValueF("SC_CREATURE_0_ROT_X_Y", 0);
	creatures[0]->rotX = panel->getValueF("SC_CREATURE_0_ROT_X_Y", 1); // on y axis in panel, so rotation around x axis //
	creatures[0]->minRotY = creatures[0]->rotY - rotYpadding;
	creatures[0]->maxRotY = creatures[0]->rotY + rotYpadding;
	creatures[0]->rotYDir = 1;
	dirNorm.rotate(creatures[0]->rotY, up);
	dirNorm.rotate(creatures[0]->rotX, side);
	creatures[0]->dir.set(dirNorm.x, dirNorm.y, dirNorm.z);
	creatures[0]->center.set(panel->getValueF("SC_CREATURE_0_LOC", 0), panel->getValueF("SC_CREATURE_0_LOC", 1), panel->getValueF("SC_CREATURE_0_LOC_Z"));
	creatures[0]->creature->setCreatureAlongVector(creatures[0]->center, creatures[0]->dir);
	creatures[0]->particle.loc.set( creatures[0]->center.x, creatures[0]->center.y );
	creatures[0]->targetLoc.set( creatures[0]->center.x, creatures[0]->center.y, 0);
	
	dirNorm.set(0, 0, 1);
	creatures[1]->rotY = panel->getValueF("SC_CREATURE_1_ROT_X_Y", 0);
	creatures[1]->rotX = panel->getValueF("SC_CREATURE_1_ROT_X_Y", 1);
	creatures[1]->minRotY = creatures[1]->rotY - rotYpadding;
	creatures[1]->maxRotY = creatures[1]->rotY + rotYpadding;
	creatures[1]->rotYDir = -1;
	dirNorm.rotate(creatures[1]->rotY, up);
	dirNorm.rotate(creatures[1]->rotX, side);
	creatures[1]->dir.set(dirNorm.x, dirNorm.y, dirNorm.z);
	creatures[1]->center.set(panel->getValueF("SC_CREATURE_1_LOC", 0), panel->getValueF("SC_CREATURE_1_LOC", 1), panel->getValueF("SC_CREATURE_1_LOC_Z"));
	creatures[1]->creature->setCreatureAlongVector(creatures[1]->center, creatures[1]->dir);
	creatures[1]->particle.loc.set( creatures[1]->center.x, creatures[1]->center.y );
	creatures[1]->targetLoc.set( creatures[1]->center.x, creatures[1]->center.y, 0);
	
	dirNorm.set(0, 0, 1);
	creatures[2]->rotY = panel->getValueF("SC_CREATURE_2_ROT_X_Y", 0);
	creatures[2]->rotX = panel->getValueF("SC_CREATURE_2_ROT_X_Y", 1);
	creatures[2]->minRotY = creatures[2]->rotY - rotYpadding;
	creatures[2]->maxRotY = creatures[2]->rotY + rotYpadding;
	creatures[2]->rotYDir = 1;
	dirNorm.rotate(creatures[2]->rotY, up);
	dirNorm.rotate(creatures[2]->rotX, side);
	creatures[2]->dir.set(dirNorm.x, dirNorm.y, dirNorm.z);
	creatures[2]->center.set(panel->getValueF("SC_CREATURE_2_LOC", 0), panel->getValueF("SC_CREATURE_2_LOC", 1), panel->getValueF("SC_CREATURE_2_LOC_Z"));
	creatures[2]->creature->setCreatureAlongVector(creatures[2]->center, creatures[2]->dir);
	creatures[2]->particle.loc.set( creatures[2]->center.x, creatures[2]->center.y );
	creatures[2]->targetLoc.set( creatures[2]->center.x, creatures[2]->center.y, 0);
}

void SelectCreatureScene::setActiveCreature(int $index) {
	if ($index > -1 && $index < 3)
		creatures[$index]->hitPct = 1.f;
}

void SelectCreatureScene::update() {
	
	#ifdef SETUP_SCREEN_COORDS
		setCreatureAttsFromPanel();
	#endif
	
	// get the screen locs of the creatures, so that we can acurately draw things behind them in 2D space //
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	
	for (int i = 0; i < 3; i++) {
		gluProject(creatures[i]->center.x, creatures[i]->center.y, creatures[i]->center.z,
				   mvmatrix, projmatrix, viewport,
				   &screenLocs[i * 3], &screenLocs[i * 3 + 1], &screenLocs[i * 3 + 2]);
		screenLocs[i * 3 + 1] = ofGetHeight() - screenLocs[i * 3 + 1]; // flip since OF coords are reversed in y //
	}
	
	float halfRectW, halfRectH;
	for(int i = 0; i < 3; i++) {
		halfRectW = creatures[i]->rect.width * .5f;
		halfRectH = creatures[i]->rect.height * .5f;
		
		//creatures[i]->rect.x = creatures[i]->center.x - halfRectW;
		//creatures[i]->rect.y = creatures[i]->center.y - halfRectH;
		creatures[i]->rect.x = screenLocs[i * 3    ] - halfRectW;
		creatures[i]->rect.y = screenLocs[i * 3 + 1] - halfRectH;
	}
	
	if (!bTransitioning) {
		if (bAnimInComplete) {
			int nBlobsFoundThisFrame = tracker->contourFinder.nBlobs;
			float currBlobX, currBlobY, currBlobW, currBlobH;
			float currBlobPtX, currBlobPtY;
			int contourIndex = 0;
			for (int i = 0; i < 3; i++) {
				creatures[i]->bColliding = false;
				
				
				int numWhite = motionCVGrayscaleImage->countNonZeroInRegion(creatures[i]->rect.x * videoMinRatio,
																			creatures[i]->rect.y * videoMinRatio,
																			creatures[i]->rect.width * videoMinRatio,
																			creatures[i]->rect.height * videoMinRatio );
				if (numWhite > 400) {
					cout << "SelectCreatureScene :: update : " << i << " numWhite = " << numWhite << endl;
					creatures[i]->hitPct += .06f;
				}
			}
			
			
			for (int i = 0; i < 3; i++) {
				//if(!creatures[i]->bColliding) {
					creatures[i]->particle.addForce(creatures[i]->targetLoc.x, creatures[i]->targetLoc.y, ofGetWidth(), 2.9f);
				//}
				creatures[i]->particle.addDamping( creatures[i]->particle.DAMPING);
				creatures[i]->particle.vel += creatures[i]->particle.acc;
				creatures[i]->particle.loc += creatures[i]->particle.vel;
				creatures[i]->particle.acc.set(0, 0);
				
				creatures[i]->center.x = creatures[i]->particle.loc.x;
				creatures[i]->center.y = creatures[i]->particle.loc.y;
				
				creatures[i]->dir.set(0, 0, 1);
				float tarY = 90.f;
				if (i == 2) tarY = -90.f;
				float rotY = ((1 - creatures[i]->hitPct) * creatures[i]->rotY) + (creatures[i]->hitPct * tarY);
				creatures[i]->dir.rotate(rotY, up);
				creatures[i]->dir.rotate(creatures[i]->rotX, side);
				
				creatures[i]->rotY += (rotYinc * creatures[i]->rotYDir);
				//creatures[i]->rotY -= 5.f;
				if (creatures[i]->rotY > creatures[i]->maxRotY) {
					creatures[i]->rotY = creatures[i]->maxRotY;
					creatures[i]->rotYDir *= -1;
				}
				if (creatures[i]->rotY < creatures[i]->minRotY) {
					creatures[i]->rotY = creatures[i]->minRotY;
					creatures[i]->rotYDir *= -1;
				}
			}
		}
	} else {
		for (int i = 0; i < 3; i++) {
			creatures[i]->dir.set(0, 0, 1);
			creatures[i]->dir.rotate(creatures[i]->rotY, up);
			creatures[i]->dir.rotate(creatures[i]->rotX, side);
			
		}
	}
	
	for (int i = 0; i < 3; i++) {
		creatures[i]->hitPct -= .02f;
		
		if (creatures[i]->hitPct < 0) creatures[i]->hitPct = 0.f;
		if (creatures[i]->hitPct > 1) creatures[i]->hitPct = 1.f;
		
		if (creatures[i]->hitPct > .95) {
			creatures[i]->hitPct = .5f;
			selectedCreatureIndex = i;
			bButtonActivated = true;
		}
		
		creatures[i]->creature->setCreatureAlongVector(creatures[i]->center, creatures[i]->dir);
	}
	
	// check the buttons //
	#ifdef USING_OSC
		if (ofGetElapsedTimeMillis() - lastBtnChecktime > BUTTON_CHECK_TIME) {
			/*
			 #define BUTTON_ID_TANGLES		1
			 #define BUTTON_ID_SPIKY		2
			 #define BUTTON_ID_SLINKY		3 
			 */
			for(int i = 0; i < 3; i++) {
				osc->buttonOver(i + 1, creatures[i]->hitPct);
			}
			
			lastBtnChecktime = ofGetElapsedTimeMillis();
		}
		if(bButtonActivated) { // a button has been selected, so send OSC //
			osc->buttonClick(selectedCreatureIndex + 1);
		}
	#endif
	
	for (int i = 0; i < 3; i++) {
		creatures[i]->creature->update();
		creatures[i]->creature->setLifeAtts( 1.f, 10000, 1000, .01f ); // keep these kids alive //
	}
	 
}

void SelectCreatureScene::draw() {
	//ofSetColor(46, 200, 150);
	//ofRect(200, 200, 10, 10);
	// glDepthTest enabled and disabled in testApp //
	for (int i = 0; i < 3; i++) {
		//glDisable(GL_DEPTH_TEST);
		if (!bTransitioning) {
			//ofSetColor(50, 50, 50);
			//ofLine(creatures[i]->particle.loc.x, creatures[i]->particle.loc.y + creatures[i]->rect.height * .5, 
			//	   creatures[i]->targetLoc.x, creatures[i]->targetLoc.y + creatures[i]->rect.height * .5 + 20);
		}
		if (i != selectedCreatureIndex) {
			float xAdd, yAdd = 0.f;
			if (i == 0) {xAdd -= 30.f; yAdd = -10.f;}
			if (i == 2) {xAdd += 30.f; yAdd = -10.f;}
			
			ofSetColor(255, 255, 255, creatures[i]->hitPct * 200 + 50);
			//ofCircle(creatures[i]->center.x + xAdd, creatures[i]->center.y, creatures[i]->rect.width * .5);
			//ofCircle(creatures[i]->center.x, creatures[i]->center.y, creatures[i]->rect.width * .5);
			ofCircle(creatures[i]->rect.x + (creatures[i]->rect.width * .5), creatures[i]->rect.y + (creatures[i]->rect.height * .5), creatures[i]->rect.width * .5);
			
			/*
			ofSetColor(255, 255, 255);
			ofNoFill();
			ofSetLineWidth(5.f);
			ofSetColor(255, 0, 0);
			ofCircle(creatures[i]->center.x + xAdd, creatures[i]->center.y, creatures[i]->rect.width);
			ofFill();
			*/
			//ofSetColor(255, 0, 0, creatures[i]->hitPct * 200 + 50);
			//ofRect(creatures[i]->rect.x, creatures[i]->rect.y, creatures[i]->rect.width, creatures[i]->rect.height);
			
			ofSetLineWidth(1.f);
			
			ofSetRectMode(OF_RECTMODE_CENTER);
			glPushMatrix();
			glTranslatef(creatures[i]->rect.x + (creatures[i]->rect.width * .5), creatures[i]->rect.y + (creatures[i]->rect.height * .5), 1.f);
			ofSetColor(255, 255, 255);
			circleBtnImage->draw( 0, 0, creatures[i]->rect.width + 4, creatures[i]->rect.height + 4);
			glPopMatrix();
			ofSetRectMode(OF_RECTMODE_CORNER);
			/*
			float fillGrey = creatures[i]->hitPct * 150 + 50;
			ofSetColor(fillGrey, fillGrey, fillGrey);
			ofRect(creatures[i]->center.x - (creatures[i]->rect.width * .5), creatures[i]->center.y - (creatures[i]->rect.height * .5),
				   creatures[i]->rect.width, creatures[i]->rect.height);
			ofNoFill();
			ofSetColor(200, 200, 200);
			ofRect(creatures[i]->center.x - (creatures[i]->rect.width * .5), creatures[i]->center.y - (creatures[i]->rect.height * .5),
				   creatures[i]->rect.width, creatures[i]->rect.height);
			ofFill();
			 */
		}
		
		creatures[i]->creature->draw();
	}
}










