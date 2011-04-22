/*
 *  FontBook.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "FontBook.h"

FontBook* FontBook::__instance = 0;

FontBook* FontBook::Instance() {
	if (__instance == 0) {
		__instance = new FontBook();
	}
	return __instance;
}

void FontBook::addFont( string $url, int $fontSize, int $lineHeight) {
	Instance();
	Font* f = new Font();
	f->fontSize = $fontSize;
	ofTrueTypeFont ttf;
	f->font = ttf;
	f->font.loadFont($url, $fontSize);
	f->font.setLineHeight($lineHeight);
	__instance->_fonts.push_back( f );
}

ofTrueTypeFont* FontBook::getFontForSize(int $fontSize) {
	Instance();
	for(int i = 0; i < __instance->_fonts.size(); i++) {
		if (__instance->_fonts[i]->fontSize == $fontSize) {
			return &__instance->_fonts[i]->font;
		}
	}
	return NULL;
}

