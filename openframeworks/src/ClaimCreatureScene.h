/*
 *  ClaimCreatureScene.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CLAIM_CREATURE_SCENE_
#define _CLAIM_CREATURE_SCENE_

#include "Scene.h"
#include "Creature.h"
#include "FontBook.h"
#include "ofxOpenCv.h"
#include "TextField.h"
#include "Constants.h"
#include "OSCManager.h"

#ifndef _CLAIM_BUTTON_
#define _CLAIM_BUTTON_
class ClaimButton : public DisplayObject {
public:
	TextField*		tf;
	float hitPct;
	bool selected;
};
#endif

class ClaimCreatureScene : public Scene {
public:
	bool bShowLiveVideo;
	bool bGimmeLiveVideo;
	bool bChooseImage;
	bool bIsFrameNew; // set from testApp //
	bool bTakePhoto;
	bool bFollowTarget;
	
	bool bGrabCreatureImage;
	
	ClaimButton* leftBtn;
	ClaimButton* rightBtn;
	
	DisplayObject* flash;
	
	ofxVec3f center;
	ofxCvColorImage		videoCVImage;
	ofxCvColorImage		smVideoCVImage;
	int smV_width, smV_height;
	float video_width, video_height; // actual dimensions of the videoCVImage //
	float video_draw_w, video_draw_h; // 
	float videoMinRatio;
	Creature* creature;
	
	unsigned int startTime; // photo timer
	unsigned int lastBtnChecktime;
	
	// set from the test app //
	ofxCvGrayscaleImage* motionCVGrayscaleImage;
	
	ofImage* circleBtnImage; // assigned by testApp //
	
	ofxVec3f target;
	ofxVec3f dest;
	
	TextField* mainMessage_txt;
	DisplayObject* imageBorder;
	float imageBorderLineLength;
	TextField* num_txt;
	
#ifdef USING_OSC
	OSCManager* osc;
#endif
	
	void initiate( const ofxVec3f &$center, float $videoMinRatio);
	void animateIn(Creature* $creature);
	void onAnimInComplete(float args);
	void onAnimInComplete_1(float args);
	virtual void animateOut();
	void onAnimOutComplete(float args);
	
	void advanceCreatureSegment();
	void setImagesFromPixels( unsigned char* $pixels, float $w, float $h);
	void startTakingPhoto();
	void takePhoto();
	virtual void reset();
	virtual void update();
	virtual void draw();
};

#endif