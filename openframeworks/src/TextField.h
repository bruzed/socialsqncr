/*
 *  TextField.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TEXT_FIELD_
#define _TEXT_FIELD_

#include "DisplayObject.h"

class TextField : public DisplayObject {
public:
	string text;
	ofTrueTypeFont* font;
	
	TextField() {
		text = "";
	}
	
	TextField(ofTrueTypeFont* $font) {
		DisplayObject::DisplayObject();
		font = $font;
	}
	
	void setFont( ofTrueTypeFont * $font ) {
		font = $font;
	}
	
	void setText( const string $str ) {
		text = $str;
		ofRectangle rec = font->getStringBoundingBox(text, 0, 0);
		width = rec.width;
		height = rec.height;
	}
	
	void draw() {
		ofSetColor(color.r, color.g, color.b, color.a);
		font->drawString(text, 0, 0);
	}
	
};

#endif