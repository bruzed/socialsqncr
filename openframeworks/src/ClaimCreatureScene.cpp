/*
 *  ClaimCreatureScene.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClaimCreatureScene.h"

void ClaimCreatureScene::initiate( const ofxVec3f &$center, float $videoMinRatio) {
	
	center.set($center.x, $center.y, $center.z);
	videoMinRatio = $videoMinRatio;
	
	video_width = 140.f;
	video_height = 140.f;
	
	video_draw_w = 400.f;
	video_draw_h = 400.f;
	
	videoCVImage.allocate(video_width, video_height);
	
	smV_width = (float)CREATURE_THUMB_WIDTH;
	smV_height = (float)CREATURE_THUMB_HEIGHT;
	smVideoCVImage.allocate(smV_width, smV_height);
	
	bShowLiveVideo = false;
	bGimmeLiveVideo = false;
	bIsFrameNew = false;
	bTakePhoto = false;
	
	mainMessage_txt = new TextField( FontBook::getFontForSize(24) );
	imageBorder = new DisplayObject();
	imageBorderLineLength = 20.f;
	
	num_txt = new TextField( FontBook::getFontForSize(24) );
	num_txt->setText("3");
	num_txt->x = center.x - (num_txt->width * .5);
	num_txt->y = 150;
	num_txt->visible = true;
	
	leftBtn = new ClaimButton();
	leftBtn->visible = true;
	leftBtn->width = 200;
	leftBtn->height = 200;
	leftBtn->x = 90;
	leftBtn->y = ofGetHeight() * .35 - leftBtn->height * .5;
	leftBtn->tf = new TextField( FontBook::getFontForSize(16) );
	leftBtn->tf->setText("Take Another");
	leftBtn->tf->x = leftBtn->x + (leftBtn->width * .5) - (leftBtn->tf->width * .5);
	leftBtn->tf->y = ofGetHeight() * .35;
	leftBtn->selected = false;
	
	rightBtn = new ClaimButton();
	rightBtn->visible = true;
	rightBtn->width = 200;
	rightBtn->height = 200;
	rightBtn->x = ofGetWidth() - rightBtn->width - 90;
	rightBtn->y = ofGetHeight() * .35 - rightBtn->height * .5;
	rightBtn->tf = new TextField( FontBook::getFontForSize(16) );
	rightBtn->tf->setText("Keep Image");
	rightBtn->tf->x = rightBtn->x + (rightBtn->width * .5) - (rightBtn->tf->width * .5);
	rightBtn->tf->y = ofGetHeight() * .35;
	rightBtn->selected = false;
	
	leftBtn->hitPct = 0.f;
	rightBtn->hitPct = 0.f;
	
	
	flash = new DisplayObject();
	flash->x = center.x - (video_draw_w * .5);
	flash->y = center.y - (video_draw_h * .5) - 100;
	flash->visible = true;
	flash->width = video_draw_w;
	flash->height = video_draw_h;
	flash->color.a = 0;
	
}

void ClaimCreatureScene::animateIn(Creature* $creature) {
	cout << "ClaimCreatureScene :: animateIn : start animation " << endl;
	creature = $creature;
	bFollowTarget = true;
	
	creature->setCreatureAlongVector(ofxVec3f(center.x, center.y + 115, 400), ofxVec3f(1, 0, 0) );
	// set him up to get target locations //
	dest.set(creature->spine[0].loc.x, creature->spine[0].loc.y, creature->spine[0].loc.z);
	
	creature->setCreatureAlongVector( ofxVec3f(center.x, center.y, 800), ofxVec3f(0, 0, -1) );
	target.set(creature->spine[0].loc.x, creature->spine[0].loc.y, creature->spine[0].loc.z);
	
	//leftBtn->displayObj->visible = false;
	//rightBtn->displayObj->visible = false;
	
	num_txt->visible = false;
	leftBtn->visible = false;
	rightBtn->visible = false;
	imageBorder->visible = false;
	
	bAnimInComplete = false;
	bAnimOutComplete = false;
	bTransitioning = true;
	bShowLiveVideo = false;
	bGimmeLiveVideo = false;
	bTakePhoto = false;
	bGrabCreatureImage = false;
	
	Tweenzor::add( &target.z, target.z, dest.z, 0.f, .8f, EASE_IN_OUT_SINE);
	Tweenzor::add( &target.y, target.y, dest.y + ofRandom(-50, 50), .5f, .9f, EASE_OUT_SINE);
	Tweenzor::add( &target.x, target.x, dest.x -ofRandom(300, 500), .5f, 1.3f, EASE_OUT_SINE);
	
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &ClaimCreatureScene::onAnimInComplete_1);
}

void ClaimCreatureScene::onAnimInComplete_1(float args) {
	
	imageBorder->visible = true;
	imageBorder->width = 50;
	imageBorder->height = 50;
	imageBorder->x = center.x - (imageBorder->width * .5);
	imageBorder->y = center.y - (imageBorder->height * .5) + 70;
	
	float padding = 10.f;
	float tarX = center.x - ((video_draw_w + padding) * .5);
	float tarY = center.y - ((video_draw_h + padding) * .5) - 100;
	
	Tweenzor::add( &imageBorder->x, imageBorder->x, tarX, 0.3f, .5f, EASE_OUT_SINE);
	Tweenzor::add( &imageBorder->y, imageBorder->y, tarY, 0.3f, .5f, EASE_OUT_SINE);
	Tweenzor::add( &imageBorder->width, imageBorder->width, video_draw_w + padding, 0.3f, .5f, EASE_OUT_SINE);
	Tweenzor::add( &imageBorder->height, imageBorder->height, video_draw_h + padding, 0.3f, .5f, EASE_OUT_SINE);
	
	//Tweenzor::add( &target.z, target.z, dest.z, 0.f, .3f, EASE_OUT_SINE);
	Tweenzor::add( &target.y, target.y, dest.y, 0.f, .5f, EASE_OUT_SINE);
	Tweenzor::add( &target.x, target.x, dest.x, 0.f, 1.f, EASE_OUT_SINE);
	
	mainMessage_txt->visible = true;
	mainMessage_txt->setText("Smile");
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = 250;
	mainMessage_txt->color.a = 0;
	
	bGimmeLiveVideo = true; // tells testApp to give us the pixels from the tracker video grabber //
	
	Tweenzor::add( &mainMessage_txt->color.a, mainMessage_txt->color.a, 255, 1.f, .8f, EASE_OUT_QUART);
	Tweenzor::add( &mainMessage_txt->y, mainMessage_txt->y, 150, 1.f, 1.f, EASE_OUT_QUART);
	
	Tweenzor::add( &mainMessage_txt->x, mainMessage_txt->x, mainMessage_txt->x, 1.f, 2.f, EASE_OUT_QUART); // just for timing //
	
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &ClaimCreatureScene::onAnimInComplete);
	
}

void ClaimCreatureScene::onAnimInComplete(float args) {
	bAnimInComplete = true;
	bTransitioning = false;
	//bShowLiveVideo = true; // testApp is going to give you the image you need, this tells it to do so //
	bChooseImage = true;
	bTakePhoto = false;
	
	startTime = ofGetElapsedTimeMillis(); // this starts the timer for someone to do something //
	
	lastBtnChecktime = startTime;
	
	takePhoto();
}


void ClaimCreatureScene::animateOut() {
	cout << "ClaimCreatureScene :: animateOut : animating creature out, need functionality here" << endl;
	bTakePhoto = false;
	bGimmeLiveVideo = false;
	bChooseImage = false;
	
	bAnimOutComplete = false;
	
	bTransitioning = true;
	leftBtn->visible = false;
	rightBtn->visible = false;
	mainMessage_txt->visible = false;
	imageBorder->visible = false;
	
	bGrabCreatureImage = true;
	
	bFollowTarget = false;
	
	target.set(dest.x, dest.y, dest.z);
	Tweenzor::add( &target.x, target.x, center.x, 0.f, 1.f, EASE_IN_OUT_SINE);
	Tweenzor::add( &target.y, target.y, center.y, 0.f, 1.f, EASE_IN_OUT_SINE);
	Tweenzor::add( &target.z, target.z, 600.f, 0.f, 1.f, EASE_IN_OUT_SINE);
	
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &ClaimCreatureScene::onAnimOutComplete);
}


void ClaimCreatureScene::onAnimOutComplete(float args) {
	bAnimOutComplete = true;
}


void ClaimCreatureScene::advanceCreatureSegment() {
	creature->advanceSegment();
}


void ClaimCreatureScene::startTakingPhoto() {
	bTakePhoto = true;
	startTime = ofGetElapsedTimeMillis();
	num_txt->visible = true;
	bGimmeLiveVideo = true;
	bChooseImage = false;
	
	rightBtn->visible = false;
	leftBtn->visible = false;
	
	mainMessage_txt->visible = true;
	mainMessage_txt->setText("Taking photo in:");
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = 100;
	mainMessage_txt->color.a = 255;
}

void ClaimCreatureScene::takePhoto() {
	bChooseImage = true;
	bGimmeLiveVideo = false;
	bTakePhoto = false;
	num_txt->visible = false;
	
	rightBtn->visible = true;
	leftBtn->visible = true;
	
	mainMessage_txt->setText("");
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = 150;
	
	flash->color.a = 255;
}

// 320 x 240 //
void ClaimCreatureScene::setImagesFromPixels(unsigned char* $pixels, float $w, float $h) {
	unsigned char* pixels = new unsigned char[(int)video_width * (int)video_height * 3];
	
	for (int x = 0; x < video_width; x++) {
		for (int y = 0; y < video_height; y++) {
			int pixIndex = (y * video_width + x) * 3;
			int pixIndex2 = ( (y+15) * 320 + (x+55) ) * 3;
			//pixels[pixIndex] = $pixels[pixIndex];
			pixels[pixIndex] = $pixels[pixIndex2];
			pixels[pixIndex + 1] = $pixels[pixIndex2 + 1];
			pixels[pixIndex + 2] = $pixels[pixIndex2 + 2];
		}
	}
	videoCVImage.setFromPixels(pixels, (int)video_width, (int)video_height);
	//videoCVImage.mirror(false, true);
	smVideoCVImage.scaleIntoMe(videoCVImage);
}

void ClaimCreatureScene::reset() {
	
}

void ClaimCreatureScene::update() {
	flash->color.a -= 2;
	if (flash->color.a < 0) flash->color.a = 0;
	
	leftBtn->hitPct -= .01;
	rightBtn->hitPct -= .01;
	if (leftBtn->hitPct < 0) leftBtn->hitPct = 0;
	if (rightBtn->hitPct < 0) rightBtn->hitPct = 0;
	leftBtn->selected = false;
	rightBtn->selected = false;
	
	if (bChooseImage) {
		if (bIsFrameNew) {
			int numWhite = motionCVGrayscaleImage->countNonZeroInRegion(leftBtn->x * videoMinRatio, leftBtn->y * videoMinRatio, 
																				   leftBtn->width * videoMinRatio, leftBtn->height * videoMinRatio);
			
			//float pctWhite = (leftBtn->width * leftBtn->height) / (float)numWhite;
			if (numWhite > 300) {
				leftBtn->hitPct += .12;
			}
			
			numWhite = motionCVGrayscaleImage->countNonZeroInRegion(rightBtn->x * videoMinRatio, rightBtn->y * videoMinRatio, 
																		rightBtn->width * videoMinRatio, rightBtn->height * videoMinRatio);
			
			if (numWhite > 300) {
				rightBtn->hitPct += .12;
			}
		}
		
		if (leftBtn->hitPct > .9) {
			leftBtn->selected = true;
		}
		if (rightBtn->hitPct > .9) {
			rightBtn->selected = true;
		}
		
		#ifdef USING_OSC
			if (ofGetElapsedTimeMillis() - lastBtnChecktime > BUTTON_CHECK_TIME) {
				osc->buttonOver(BUTTON_ID_TAKE_PHOTO, leftBtn->hitPct);
				osc->buttonOver(BUTTON_ID_KEEP_PHOTO, rightBtn->hitPct);
				lastBtnChecktime = ofGetElapsedTimeMillis();
			}
		#endif
		
	}
	if (leftBtn->hitPct > 1) leftBtn->hitPct = 1;
	if (rightBtn->hitPct > 1) rightBtn->hitPct = 1;
	
	if (leftBtn->selected) {
		startTakingPhoto();
		#ifdef USING_OSC
			osc->buttonClick(BUTTON_ID_TAKE_PHOTO);	
		#endif
	}
	if (rightBtn->selected) {
		animateOut(); //
		#ifdef USING_OSC
			osc->buttonClick(BUTTON_ID_KEEP_PHOTO);	
		#endif
	}
	
	if (bTakePhoto) {
		int timezors = ofGetElapsedTimeMillis() - startTime;
		int elapsed = timezors / 1000;
		num_txt->setText(ofToString( 2 - elapsed + 1, 0));
		if (elapsed > 2) {
			takePhoto();
		}
	}
	
	if (bAnimInComplete && !bTransitioning) {
		if (ofGetElapsedTimeMillis() - startTime > 20000) {
			animateOut(); //
			startTime = ofGetElapsedTimeMillis();
		}
	}
	
	#ifdef SETUP_SCREEN_COORDS
		
	#else
		if (bFollowTarget) { // don't update on animate out, the creature will be picked up by testApp and then animated out :)
			creature->spine[0].addForce( (target - creature->spine[0].loc) * .15f );
		} else {
			creature->setCreatureAlongVector( target, ofxVec3f(1, 0, 0) );
		}
		creature->update();
	#endif
}

void ClaimCreatureScene::draw() {
	if (!bTransitioning) {
		ofSetColor(255, 255, 255);
		videoCVImage.draw(center.x - (video_draw_w * .5), center.y - (video_draw_h * .5) - 100, video_draw_w, video_draw_h);
	}
	
	if (imageBorder->visible) {
		ofSetColor(200, 200, 200);
		glBegin(GL_LINES);
		
		glVertex2f(imageBorder->x, imageBorder->y);
		glVertex2f(imageBorder->x + imageBorderLineLength, imageBorder->y);
		
		glVertex2f(imageBorder->x, imageBorder->y);
		glVertex2f(imageBorder->x, imageBorder->y + imageBorderLineLength);
		
		glVertex2f(imageBorder->x, imageBorder->y + imageBorder->height);
		glVertex2f(imageBorder->x, imageBorder->y + imageBorder->height - imageBorderLineLength);
		
		glVertex2f(imageBorder->x, imageBorder->y + imageBorder->height);
		glVertex2f(imageBorder->x + imageBorderLineLength, imageBorder->y + imageBorder->height);
		
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y);
		glVertex2f(imageBorder->x + imageBorder->width - imageBorderLineLength, imageBorder->y);
		
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y);
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y + imageBorderLineLength);
		
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y + imageBorder->height);
		glVertex2f(imageBorder->x + imageBorder->width - imageBorderLineLength, imageBorder->y + imageBorder->height);
		
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y + imageBorder->height);
		glVertex2f(imageBorder->x + imageBorder->width, imageBorder->y + imageBorder->height - imageBorderLineLength);
		
		glEnd();
	}
	
	if (num_txt->visible) {
		glPushMatrix();
		glTranslatef(num_txt->x, num_txt->y, 0);
		num_txt->draw();
		glPopMatrix();
	}
	
	if (mainMessage_txt->visible) {
		glPushMatrix();
		glTranslatef(mainMessage_txt->x, mainMessage_txt->y, 0);
		mainMessage_txt->draw();
		glPopMatrix();
	}
	if (leftBtn->visible) {
		//ofSetColor(255, 255, 255, 30 + leftBtn->hitPct * 150);
		//ofRect(leftBtn->x, leftBtn->y, leftBtn->width, leftBtn->height);
		
		ofSetColor(255, 255, 255, leftBtn->hitPct * 200.f + 50);
		ofCircle(leftBtn->x + leftBtn->width * .5, leftBtn->y + leftBtn->width * .5, leftBtn->width * .5); // since it is recs
		
		/*
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofSetLineWidth(5.f);
		ofCircle(leftBtn->x + leftBtn->width * .5, leftBtn->y + leftBtn->width * .5, leftBtn->width * .5); // since it is recs
		ofFill();
		 */
		ofSetRectMode(OF_RECTMODE_CENTER);
		glPushMatrix();
		glTranslatef(leftBtn->x + leftBtn->width * .5, leftBtn->y + leftBtn->width * .5, 1.f);
		ofSetColor(255, 255, 255);
		circleBtnImage->draw( 0, 0, leftBtn->width + 4, leftBtn->width + 4);
		glPopMatrix();
		ofSetRectMode(OF_RECTMODE_CORNER);
		
		leftBtn->tf->color.r = leftBtn->tf->color.g = leftBtn->tf->color.b = (1 - leftBtn->hitPct) * 255;
		
		ofSetLineWidth(1.f);
		
		glPushMatrix();
		glTranslatef(leftBtn->tf->x, leftBtn->tf->y, 0);
		leftBtn->tf->draw();
		glPopMatrix();
	}
	
	if (rightBtn->visible) {
		//ofSetColor(255, 255, 255, 30 + rightBtn->hitPct * 150);
		//ofRect(rightBtn->x, rightBtn->y, rightBtn->width, rightBtn->height);
		
		ofSetColor(255, 255, 255, rightBtn->hitPct * 200.f + 50);
		ofCircle(rightBtn->x + rightBtn->width * .5, rightBtn->y + rightBtn->width * .5, rightBtn->width * .5); // since it is recs
		
		/*
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofSetLineWidth(5.f);
		ofCircle(rightBtn->x + rightBtn->width * .5, rightBtn->y + rightBtn->width * .5, rightBtn->width * .5); // since it is recs
		ofFill();
		*/
		
		ofSetRectMode(OF_RECTMODE_CENTER);
		glPushMatrix();
		glTranslatef(rightBtn->x + rightBtn->width * .5, rightBtn->y + rightBtn->width * .5, 1.f);
		ofSetColor(255, 255, 255);
		circleBtnImage->draw( 0, 0, rightBtn->width + 4, rightBtn->width + 4);
		glPopMatrix();
		ofSetRectMode(OF_RECTMODE_CORNER);
		
		rightBtn->tf->color.r = rightBtn->tf->color.g = rightBtn->tf->color.b = (1 - rightBtn->hitPct) * 255;
		
		ofSetLineWidth(1.f);
		
		glPushMatrix();
		glTranslatef(rightBtn->tf->x, rightBtn->tf->y, 0);
		rightBtn->tf->draw();
		glPopMatrix();
	}
	
	if (flash->visible) {
		ofSetColor(255, 255, 255, flash->color.a);
		ofRect(flash->x, flash->y, flash->width, flash->height);
	}
	
	//ofSetColor(255, 40, 200, 150);
	//ofRect(150, 150, 10, 10);
	glEnable(GL_DEPTH_TEST);
		creature->draw();
	glDisable(GL_DEPTH_TEST);
}






