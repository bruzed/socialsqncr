/*
 *  ColorPalette.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorPalette.h"

ColorPalette* ColorPalette::__instance = 0;

ColorPalette* ColorPalette::Instance() {
	if (__instance == 0) {
		__instance = new ColorPalette();
		__instance->NUM_COLORS = 3;
		__instance->initiate();
	}
	return __instance;
}

void ColorPalette::initiate() {
	
	for (int i = 0; i < __instance->NUM_COLORS; i++) {
		CreatureColor* col = new CreatureColor();
		__instance->_colors.push_back( col );
	}
	
	// Blue HSV //
	__instance->_colors[0]->hsvColor.r = (212.f/360.f) * 255.f; // Hue, since PhotoShop does 0 - 360 degrees, we need 0 - 255
	__instance->_colors[0]->hsvColor.g = .67f * 255.f; // Saturation, since PhotoShop does 0 - 100 %
	__instance->_colors[0]->hsvColor.b = .86f * 255.f; // Value / Brightness, since PhotoShop does 0 - 100 %
	__instance->_colors[0]->hsvColor.a = 100; // Alpha
	__instance->_colors[0]->hueMin = __instance->_colors[0]->hsvColor.r - 25;
	__instance->_colors[0]->hueMax = __instance->_colors[0]->hsvColor.r + 25;
	__instance->_colors[0]->hueInc = 5; // total 50 / 10, since we want 10 diff colors 
	
	// Yellow HSV //
	__instance->_colors[1]->hsvColor.r = (70.f/360.f) * 255.f;
	__instance->_colors[1]->hsvColor.g = .64f * 255.f;
	__instance->_colors[1]->hsvColor.b = .95f * 255.f;
	__instance->_colors[1]->hsvColor.a = 100;
	__instance->_colors[1]->hueMin = __instance->_colors[1]->hsvColor.r - 25;
	__instance->_colors[1]->hueMax = __instance->_colors[1]->hsvColor.r + 25;
	__instance->_colors[1]->hueInc = 5; // total 50 / 10, since we want 10 diff colors 
	
	// Red HSV //
	__instance->_colors[2]->hsvColor.r = 0;
	__instance->_colors[2]->hsvColor.g = .85f * 255.f;
	__instance->_colors[2]->hsvColor.b = .90f * 255.f;
	__instance->_colors[2]->hsvColor.a = 100;
	__instance->_colors[2]->hueMin = __instance->_colors[2]->hsvColor.r - 25;
	__instance->_colors[2]->hueMax = __instance->_colors[2]->hsvColor.r + 25;
	__instance->_colors[2]->hueInc = 5; // total 50 / 10, since we want 10 diff colors 
	
}

void ColorPalette::advanceColorForCreature( int $creatureType ) {
	Instance();
	__instance->incrementCreatureColor($creatureType - 1);// since creature types start at 1, located in Constants //
}

void ColorPalette::incrementCreatureColor( int $index ) {
	__instance->_colors[$index]->hsvColor.r += __instance->_colors[$index]->hueInc;
	if (__instance->_colors[$index]->hsvColor.r > __instance->_colors[$index]->hueMax) {
		__instance->_colors[$index]->hsvColor.r = __instance->_colors[$index]->hueMin;
	}
}

ofColor ColorPalette::getCurrentColorForCreature( int $creatureType ) {
	Instance();
	float hue = __instance->_colors[$creatureType - 1]->hsvColor.r; // since creature types start at 1 //
	//cout << "ColorPalette :: getCurrentColorForCreature : hue before adding " << hue << endl;
	if (hue < 0) hue += 255; // the red min is negative //
	//cout << "ColorPalette :: getCurrentColorForCreature : hue after adding " << hue << endl;
	ofColor tempColor = convertHsvToRgb(hue, __instance->_colors[$creatureType - 1]->hsvColor.g, __instance->_colors[$creatureType - 1]->hsvColor.b);
	tempColor.a = __instance->_colors[1]->hsvColor.a;
	return tempColor;
}

ofColor ColorPalette::getBaseBodyColor() { // the same for all of the creatures //
	return ofColor(255, 255, 255, 100);
}

ofColor ColorPalette::getWhiteHighlightColor() {
	return ofColor(255, 255, 255, 255);
}

ofColor ColorPalette::getInactiveCreatureColor() {
	return ofColor(200, 200, 200, 150);
}

// courtesy Zach Lieberman ///////////////////////////////////////////////////////////////
ofColor ColorPalette::convertRgbToHsv(float $r, float $g, float $b) {
	
	const double &x = $r / 255.0f;
    const double &y = $g / 255.0f;
    const double &z = $b / 255.0f;
	
    double max	 = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    double min	 = (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
    double range = max - min;
    float val	 = max;
    float sat   = 0;
    float hue   = 0;
    
    if (max != 0)   sat = range/max;
    
    if (sat != 0) {
		double h;
		
		if      (x == max)	h =     (y - z) / range;
		else if (y == max)	h = 2 + (z - x) / range;
		else		h = 4 + (x - y) / range;
		
		hue = h/6.;
	    
		if (hue < 0.)
			hue += 1.0;
    }
	ofColor tempColor;
	tempColor.r = hue * 255.0f; tempColor.g = sat * 255.0f; tempColor.b = val * 255.0f;
    return tempColor;
}

ofColor ColorPalette::convertHsvToRgb(float $h, float $s, float $v) {
	
	double hue = $h / 255.0f;
    double sat = $s / 255.0f;
    double val = $v / 255.0f;
	
    float x = 0.0, y = 0.0, z = 0.0;
    
    if (hue == 1) hue = 0;
    else hue *= 6;
	
    int i = int(floor(hue));
    double f = hue-i;
    double p = val*(1-sat);
    double q = val*(1-(sat*f));
    double t = val*(1-(sat*(1-f)));
	
    switch (i) {
		case 0: x = val; y = t; z = p; break;
		case 1: x = q; y = val; z = p; break;
		case 2: x = p; y = val; z = t; break;
		case 3: x = p; y = q; z = val; break;
		case 4: x = t; y = p; z = val; break;
		case 5: x = val; y = p; z = q; break;
    }
	ofColor tempColor;
	tempColor.r = x * 255.0f; tempColor.g = y * 255.0f; tempColor.b = z * 255.0f;
    return tempColor;
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////



