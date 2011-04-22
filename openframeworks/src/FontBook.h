/*
 *  FontBook.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _FONT_BOOK_
#define _FONT_BOOK_

#include "ofMain.h"

typedef struct {
	ofTrueTypeFont font;
	int fontSize;
} Font;

class FontBook {
public:
	
	static FontBook* Instance();
	
	static void addFont( string $url, int $fontSize, int $lineHeight);
	static ofTrueTypeFont* getFontForSize(int $fontSize);
	
private:
	//ofTrueTypeFont  helveticaMed;
	// Font struct is located in Constants //
	static FontBook* __instance;
	vector <Font*> _fonts;
};

#endif