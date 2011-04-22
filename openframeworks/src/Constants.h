/*
 *  Constants.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CONSTANTS_
#define _CONSTANTS_

#define USING_OSC
//#define SETUP_SCREEN_COORDS

#define SCENE_ENTER_NAME				10
#define SCENE_RENDER_ALL_CREATURES		11
#define SCENE_SELECT_CREATURE			12
#define SCENE_RECORDING_CREATURE		13

#define CREATURE_NUM_NOTES_PER_SEGMENT	8
#define CREATURE_NUM_BODY_PTS_PER_SEGMENT 8
// Tangles == Rock, Slinky == Paper, Spiky == Scissors //
#define CREATURE_TYPE_RECTANGLES		1
#define CREATURE_TYPE_SPIKEY			2
#define CREATURE_TYPE_SLINKY			3

#define CREATURE_THUMB_WIDTH			70
#define CREATURE_THUMB_HEIGHT			70

#define TOTAL_CREATURE_COL0RS			12

// Button ID constants //
#define BUTTON_ID_CREATE		0
#define BUTTON_ID_TANGLES		1
#define BUTTON_ID_SPIKY			2
#define BUTTON_ID_SLINKY		3
#define BUTTON_ID_TAKE_PHOTO	4
#define BUTTON_ID_KEEP_PHOTO	5

#define BUTTON_CHECK_TIME		350 // if the last time check of the button is over this, then send value //

#include "ofxVec3f.h"
#include "ofMain.h"
#include "ColorPalette.h"

typedef struct  {
	ofxVec3f up;
	ofxVec3f forward;
	ofxVec3f side;
} Orientation;

typedef struct {
	ofxVec3f loc;
	float radius;
} MovementPoint;

typedef struct {
	unsigned int creatureIndex;
	int segmentIndex;
} OSCAdvanceSegmentMessage;




#endif