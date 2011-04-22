/*
 *  Scene.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SCENE_
#define _SCENE_

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "Constants.h"
#include "ofxTweenzor.h"

class Scene {
public:
	bool bAnimInComplete;
	bool bAnimOutComplete;
	bool bTransitioning;
	
	ofxControlPanel* panel;
	
	Scene();
	virtual void setUpPanel(ofxControlPanel* $panel);
	// we are doing quick cuts and quick animate in //
	virtual void animateIn();
	virtual void animateOut();
	virtual void reset();
	virtual void update();
	virtual void draw();
};

#endif