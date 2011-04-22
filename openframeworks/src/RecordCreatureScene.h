/*
 *  RecordCreatureScene.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RECORD_CREATURE_SCENE_
#define _RECORD_CREATURE_SCENE_

#include "ofxOpenCv.h"
#include "Scene.h"
#include "Constants.h"
#include "RecordSegment.h"
#include "CircleSegmentManager.h"
#include "Creatures.h"
#include "TextField.h"
#include "FontBook.h"
#include "OSCManager.h"
#include "vectorField.h"
#include "ofxBlobTracker.h"

class RecordCreatureScene : public Scene {
public:
	int status; // which type of creature are you recording? Change drawing of recording, accordingly //
	int numMovementPts, numSpinePts;
	int numSegmentPts;
	int segmentIndex;
	float segAngleInc;
	float videoMinRatio, videoRatio;
	float video_width, video_height;
	bool bRecording;
	bool bRecordingComplete;
	unsigned int startTime;
	vector <RecordSegment> movementPts;
	ofxVec3f center;
	
	bool bDrawCreature; // are we ready to draw the creature yet? //
	
	float currActiveBtnIndex; // for animating in the buttons //
	
	Creature* creature;
	bool bCreatureVisible;
	ofxVec3f creatureCenter;
	ofxVec3f creatureTargetCenter;
	
	unsigned int motion_totalWhitePixPoss, motion_whitePixHit;
	float motionHitPct, motionPosPct;
	float* hitZones;
	
	unsigned int currentCreatureInt;
	
	float cSegRotZ;
	
	float frequencies[CREATURE_NUM_NOTES_PER_SEGMENT];
	
	TextField* mainMessage_txt;
	TextField* countDown_txt;
	
	CircleSegmentManager*	cSegmentManager;
	CircleSegmentManager*	cSegmentManager_f;
	CircleSegmentManager*	cSegmentManager_b;
	
	ofxBlobTracker*			tracker;
	ofxCvGrayscaleImage*	motionCVGrayscaleImage;
	
	vectorField* VF;
	ofxCvGrayscaleImage	grayDiffSmall;
	
	void initiate( int $numPts, int $numSegmentPts, float $maxRadius, ofxVec3f $center, float $videoMinRatio);
	virtual void setUpPanel(ofxControlPanel* $panel);
	
	void setUpForRecording(float $x, float $y, float $radius);
	//void setCurrentRecordingSegmentLocation(int $segmentIndex, float $x, float $y);
	void setCurrentRecordingSegmentLocation(int $segmentIndex, float $x, float $y, bool $tween);
	void setRandomRecordingSegmentLocations(float $radius, float $maxRandom);
	void updateSegmentPoints(int $segmentIndex, float $x, float $y, float $z);
	
	void setRandomPointsForCreature(Creature* $cr, ofxVec3f $center, float $targetRadius, float $radiusScalar, float $maxRandom);
	void setPointsForCreature( Creature* $cr );
	void addRadiusForNotesHit( Creature* $cr, float $noteHitAdd );
	void addForceToMovementPtAlongAxis(int $movementPtIndex, int $bodyPtIndex, ofxVec2f $frc);
	void addForceToMovementPtAlongCenterNormal(int $movementPtIndex, int $bodyPtIndex, float $strength);
	void setRandomRecordingNotesHit();
	void resetNotesHit(); 
	void setNotesHitForCreature( Creature* $cr );
	void setRandomNotesHitForCreature( Creature* $cr );
	void buildCreature( unsigned int $creatureID, int $bpmIndex );
	void setMovementPtsRadius(ofxVec3f $center, float $targetRadius, float $radiusScalar);
	//void setNoteActivated(int $segmentIndex, int $noteIndex);
	void advanceSegment( );
	void onSegmentsAnimComplete(float args);
	
	virtual void animateIn();
	void onAnimInComplete(float args);
	void onAnimInComplete_1(float args);
	void onBtnAnimInTweenUpdate(float $args );
	void onAnimInComplete_2(float args);
	void onAnimInComplete_3(float args);
	virtual void animateOut();
	void onAnimOutComplete(float args);
	
	void calculateMotionPercentages();
	
	virtual void update();
	virtual void draw();
	
#ifdef USING_OSC
	OSCManager* osc;
#endif
	
};

#endif