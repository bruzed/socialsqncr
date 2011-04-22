/*
 *  TopMenu.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TOP_MENU_
#define _TOP_MENU_

#include "ofxTweenzor.h"
#include "ofMain.h"
#include "Constants.h"
#include "ofxOpenCv.h"
#include "FontBook.h"
#include "Particle2D.h"
#include "TextField.h"
#include "ofxBlobTracker.h"
#include "OSCManager.h"

typedef struct {
	ofPoint targetPoint;
	float width, height;
	TextField tf;
	bool bHit;
	float hitPct;
	float radius;
	unsigned int lastCheckTime;
} BubbleMenuItem;

class TopMenu {
public:
	
	bool bAnimInComplete;
	bool bAnimOutComplete;
	bool bTopMenuHit;
	bool bButtonActivated;
	bool bNewVideoFrame;
	int currScene;
	
	TextField mainMessage;
	
	int menuItemSelected;
	
	//int totalMenuItems;
	
	//vector < BubbleMenuItem > menuItems;
	BubbleMenuItem menuItem;
	
	ofColor			bgRecHighColor;
	ofColor			bgRecBaseColor;
	
#ifdef USING_OSC
	OSCManager* osc;
#endif
	
	// set from the test app //
	ofxCvGrayscaleImage* motionCVGrayscaleImage;
	ofxBlobTracker* tracker;
	
	ofImage* circleBtnImage; // assigned by testApp //
	
	float videoMinRatio, videoRatio;
	
	int fontSize;
	
	void initiate();
	void animateButtonIn();
	void animateButtonOut();
	void update();
	void draw();
	void onAnimInComplete(float args);
};

#endif