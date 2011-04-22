/*
 *  ColorPalette.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLOR_PALETTE_
#define _COLOR_PALETTE_

#include "Constants.h"
#include "ofMain.h"

typedef struct {
	ofColor hsvColor;
	float hueMax, hueMin;
	float hueInc;
} CreatureColor;

class ColorPalette {
public:
	static ColorPalette* Instance();
	
	static void advanceColorForCreature( int $creatureType );
	
	static ofColor getCurrentColorForCreature( int $creatureType );
	static void initiate();
	
	static ofColor convertRgbToHsv(float $r, float $g, float $b);
	static ofColor convertHsvToRgb(float $h, float $s, float $v);
	
	static ofColor getBaseBodyColor();
	static ofColor getWhiteHighlightColor();
	
	static ofColor getInactiveCreatureColor();
	
	
private:
	int NUM_COLORS;
	static ColorPalette* __instance;
	vector < CreatureColor* > _colors;
	
	void incrementCreatureColor( int $index );
};

#endif