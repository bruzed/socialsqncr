/*
 *  SelectCreatureScene.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SELECT_CREATURE_SCENE_
#define _SELECT_CREATURE_SCENE_

#include "Scene.h"
#include "Tangles.h"
#include "Spiky.h"
#include "Slinky.h"
#include "Creature.h"
#include "RecordCreatureScene.h"
#include "ofxBlobTracker.h"
#include "Constants.h"
#include "Particle2D.h"

typedef struct {
	Creature* creature;
	ofxVec3f center, targetLoc;
	ofxVec3f dir;
	ofRectangle rect;
	Particle2D particle;
	bool bColliding;
	float rotY;
	float rotX;
	float minRotY, maxRotY;
	float rotYDir;
	float hitPct;
} SelectCreature;

class SelectCreatureScene : public Scene {
public:
	SelectCreature* creatures[3]; // Tangles, Spiky, Slinky //
	ofxVec3f up;
	ofxVec3f side;
	ofxVec3f center;
	float rotYinc;
	int selectedCreatureIndex;
	bool bButtonActivated;
	
	float videoMinRatio, videoRatio;
	
	ofxCvGrayscaleImage*	motionCVGrayscaleImage;
	
	ofImage* circleBtnImage; // assigned by testApp //
	
	unsigned int lastBtnChecktime; // store the last time that the buttons were checked //
	
	// prob should have put this in a singleton or something, as I keep adding it to all of the classes :/
	double* screenLocs;
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	RecordCreatureScene* recordScene; // used to set the points to random //
	
	ofxBlobTracker* tracker; // updated in testApp //
	
#ifdef USING_OSC
	OSCManager* osc;
#endif
	
	virtual void setUpPanel(ofxControlPanel* $panel);
	void initiate();
	void advanceSegments();
	virtual void animateIn();
	void onAnimInComplete(float args);
	virtual void animateOut();
	void onAnimOutComplete(float args);
	void setActiveCreature(int $index);
	void setCreatureAttsFromPanel();
	virtual void update();
	virtual void draw();
};

#endif