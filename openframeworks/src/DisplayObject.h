/*
 *  DisplayObject.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DISPLAY_OBJECT_
#define _DISPLAY_OBJECT_

#include "ofxVec3f.h"

class DisplayObject : public ofxVec3f {
public:
	ofColor color;
	
	float width, height;
	bool visible;
	
	DisplayObject() {
		visible = true;
		color.r = 255; color.g = 255; color.b = 255; color.a = 255;
	}
};

#endif