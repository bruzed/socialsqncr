/*
 *  TopMenu.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "TopMenu.h"


void TopMenu::initiate() {
	bAnimInComplete =	true;
	bAnimOutComplete =	false;
	bTopMenuHit =		false;
	bButtonActivated =	false;
	bNewVideoFrame =	false; // set from testApp //
	
	// fontSize set from outside, in testApp //
	fontSize = 24; // default //
	
	bgRecHighColor.r = 200; bgRecHighColor.g = 255; bgRecHighColor.b = 50; bgRecHighColor.a = 255;
	bgRecBaseColor.r = 50; bgRecBaseColor.g = 50; bgRecBaseColor.b = 50; bgRecBaseColor.a = 20;
	
	mainMessage.font = FontBook::getFontForSize( fontSize );
	
	// create the single button instance //
	menuItem.hitPct = 0.f;
	menuItem.tf.font = FontBook::getFontForSize( 16 );
	menuItem.tf.setText( "CREATE" );
	menuItem.radius = 60.f;
	menuItem.targetPoint.x = ofGetWidth() * .5;
}

void TopMenu::animateButtonIn() {
	bButtonActivated = false;
	bAnimInComplete = false;
	
	menuItem.hitPct			= 0.f;
	//menuItem.radius			= 0.f;
	menuItem.tf.color.a		= 0.f;
	
	menuItem.lastCheckTime = ofGetElapsedTimeMillis();
	
	// set up bubbles in scenes //
	if (currScene == SCENE_RENDER_ALL_CREATURES) {
		mainMessage.setText("");
	}
	
	if (currScene == SCENE_SELECT_CREATURE) {
		mainMessage.setText("SELECT YOUR CREATURE!");
	}
	if (currScene == SCENE_ENTER_NAME) {
		//mainMessage.setText("TAG YOUR CREATURE");
		mainMessage.setText(" ");
	}
	mainMessage.y = -50;
	mainMessage.x = ofGetWidth() * .5 - mainMessage.width * .5;
	
	Tweenzor::add( &mainMessage.y, -50, 40.f, 0.f, 1.f, EASE_OUT_QUART);
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &TopMenu::onAnimInComplete);
}

void TopMenu::onAnimInComplete(float args) {
	cout << "The animation in is complete" << endl;
	bAnimInComplete = true;
}

void TopMenu::animateButtonOut() {
	bButtonActivated = false;
	bAnimInComplete = false;
	
	Tweenzor::add( &mainMessage.y, mainMessage.y, -50.f, 0.f, 1.f, EASE_IN_QUART);
}

void TopMenu::update() {
	
	if (currScene == SCENE_RENDER_ALL_CREATURES) {
		if (bNewVideoFrame) {
			
			bool bFoundThisFrame = tracker->contourFinder.nBlobs > 0;
			
			if (bFoundThisFrame) {
				//menuItem.radius += ( 25.f - menuItem.radius) * .2f;
				menuItem.tf.color.a += (250.f - menuItem.tf.color.a) * .05f;
			} else {
				//menuItem.radius += ( -menuItem.radius) * .005f;
				menuItem.tf.color.a += (50.f - menuItem.tf.color.a) * .025f;
			}
			
			//if (menuItem.radius > 25) menuItem.radius = 25.f;
			//if (menuItem.radius < 0) menuItem.radius = 0.f;
			
			if (menuItem.tf.color.a > 250) menuItem.tf.color.a = 250.f;
			if (menuItem.tf.color.a < 50) menuItem.tf.color.a = 50;
			
			if (menuItem.targetPoint.x < menuItem.radius) menuItem.targetPoint.x = menuItem.radius;
			if (menuItem.targetPoint.x > ofGetWidth() - menuItem.radius) menuItem.targetPoint.x = ofGetWidth() - menuItem.radius;
			if (menuItem.targetPoint.y < menuItem.radius) menuItem.targetPoint.y = menuItem.radius;
			if (menuItem.targetPoint.y > ofGetHeight() - menuItem.radius) menuItem.targetPoint.y = ofGetHeight() - menuItem.radius;
			
			int numWhite = 0;
			 numWhite = motionCVGrayscaleImage->countNonZeroInRegion((menuItem.targetPoint.x - menuItem.radius) * videoMinRatio,
																	 (menuItem.targetPoint.y - menuItem.radius) * videoMinRatio,
																	 (menuItem.radius * 2) * videoMinRatio,
																	 (menuItem.radius * 2) * videoMinRatio );
			if (numWhite > 100) {
				cout << "TopMenu :: update : " << " numWhite = " << numWhite << endl;
				menuItem.hitPct += .1f;
			}
			
			#ifdef USING_OSC
				if (ofGetElapsedTimeMillis() - menuItem.lastCheckTime > BUTTON_CHECK_TIME) {
					osc->buttonOver(BUTTON_ID_CREATE, menuItem.hitPct);
					menuItem.lastCheckTime = ofGetElapsedTimeMillis();
				}
			#endif
			
			
			if (menuItem.hitPct > .9) {
				bButtonActivated = true; // now send the buttonClick event //
				#ifdef USING_OSC
					osc->buttonClick( BUTTON_ID_CREATE );
				#endif
			}
			menuItem.hitPct -= .02f;
			if (menuItem.hitPct < 0) menuItem.hitPct = 0;
			if (menuItem.hitPct > 1) menuItem.hitPct = 1;
			
			
			float xenoTar = .97f;
			float totalBlobs = (float)tracker->contourFinder.nBlobs;
			float minY = 9000;
			float tarX = ofGetWidth() * .5;
			float tarY;
			if (totalBlobs > 0) {
				float totalX = 0.f;
				for (int i = 0; i < totalBlobs; i++) {
					tarX = tracker->contourFinder.blobs[i].boundingRect.x + ( tracker->contourFinder.blobs[i].boundingRect.width * .5f );
					tarY = tracker->contourFinder.blobs[i].boundingRect.y;
					
					tarX *= videoRatio;
					tarY *= videoRatio;
					
					totalX += tarX;
					
					if (tarY < minY) {
						minY = tarY;
					}
				}
				totalX /= totalBlobs;
				//tarX = ofMap(totalX, 0, ofGetWidth(), 0, ofGetWidth() * .3);
				//tarX = (ofGetWidth() * .5) - (tarX * .5);
				tarX = totalX;
				tarY = minY - menuItem.radius - 10;
			} else {
				tarY = menuItem.radius + 20;
				tarX = (float) ofGetWidth() * .5f;
			}
			
			if( tarY < menuItem.radius + 20) {
				tarY = menuItem.radius + 20;
			} else if (tarY > 200) {
				tarY = 200;
			}
			
			menuItem.targetPoint.x = menuItem.targetPoint.x * xenoTar + tarX * (1 - xenoTar);
			menuItem.targetPoint.y = menuItem.targetPoint.y * xenoTar + tarY * (1 - xenoTar);
		
		}
		
		menuItem.tf.x = menuItem.targetPoint.x - (menuItem.tf.width * .5f);
		menuItem.tf.y = menuItem.targetPoint.y + 4;
			
		
	}
}

void TopMenu::draw() {
	ofFill();
	
	if (currScene == SCENE_RENDER_ALL_CREATURES) {
		ofSetColor(255, 255, 255, menuItem.hitPct * 200.f + 20);
		ofCircle(menuItem.targetPoint.x, menuItem.targetPoint.y, menuItem.radius);
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(255, 255, 255, 255);
		circleBtnImage->draw( menuItem.targetPoint.x, menuItem.targetPoint.y, menuItem.radius * 2 + 2, menuItem.radius * 2 + 2);
		ofSetRectMode(OF_RECTMODE_CORNER);
		menuItem.tf.color.r = menuItem.tf.color.g = menuItem.tf.color.b = (1 - menuItem.hitPct) * 255;
		
		ofSetLineWidth(1.f);
		
		glPushMatrix();
		glTranslatef(menuItem.tf.x, menuItem.tf.y, 0);
		menuItem.tf.draw();
		glPopMatrix();
	}
		
	
	glPushMatrix();
	glTranslatef(mainMessage.x, mainMessage.y, 0);
	mainMessage.draw();
	glPopMatrix();
	
}













