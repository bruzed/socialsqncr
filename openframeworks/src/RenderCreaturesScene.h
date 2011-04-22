/*
 *  RenderCreaturesScene.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDER_CREATURES_SCENE_
#define _RENDER_CREATURES_SCENE_

#include "Constants.h"
#include "Scene.h"
#include "Creatures.h"
#include "ofxPerlin.h"
#include "OSCManager.h"
#include "ParticleManager.h"
#include "ofxCamera.h"
#include "ofxCvGrayscaleImage.h"
#include "vectorField.h"

class RenderCreaturesScene : public Scene {
public:
	vector <Creature*> creatures;
	int totalCreatures;
	int creaturesLastID; // stored for sending to SC //
	int numCreatureSegmets;
	bool bMousePressed;
	float mouseX, mouseY;
	
	ParticleManager pManager;
	
	ofxVec3f minCreatureBounds, maxCreatureBounds;
	ofxVec3f avgCreatureLoc;
	
	ofxCamera* cam;
	ofxVec3f camMinBounds, camMaxBounds;
	float camEasing;
	
	// get the screen locs of the creature to add VF forces //
	double* screenLocs;
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	OSCManager* osc;
	
	vectorField*			VF;
	ofxCvGrayscaleImage*	motionCVGrayscaleImage;
	ofxCvGrayscaleImage		grayDiffSmall;
	
	ofxPerlin* perlin;
	float counter;
	unsigned int lastSentOscTime, lastSentOscTime2; // when to send various messages via OSC //
	
	virtual void setUpPanel(ofxControlPanel* $panel);
	void setMotionCVImage( ofxCvGrayscaleImage* $motionCVGrayscaleImage );
	void setVideoAtts( float $video_width, float $video_height, float $videoMinRatio, float $videoRatio );
	void initiate();
	void addCreature( int $creatureType );
	void clearAllCreatures();
	void deleteCreatureAtIndex( int $creatureIndex );
	void initLastCreature();
	void advanceCreatureSegments();
	bool advanceCreatureSegment( unsigned int $creatureIndex, int $segmentIndex );
	
	void resetCamera();
	void checkCameraBounds();
	
	virtual void update();
	virtual void draw();
};

#endif