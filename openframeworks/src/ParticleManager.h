/*
 *  ParticleManager.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PARTICLE_MANAGER_
#define _PARTICLE_MANAGER_

#include "ofxVec3f.h"
#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxCvGrayscaleImage.h"

class PVert {
public:
	float x, y, z;
	PVert() {
		x=0.0; y=0.0; z=0.0;
	}
};

class PColor {
public:
	float r, g, b, a;
	PColor() {
		r=0.0; g=1.0; b=1.0; a=1.0;
	}
};

class PTexture {
public:
	float u, v;
	PTexture() {
		u = 0.0; v = 0.0;
	}
};

class ParticleManager {
public:
	int NUM_PARTICLES;
	
	ofxControlPanel* panel;
	
	// via Todd Vanderlin //////////////////////////
	// Texture
	ofImage		texture;
	float		texW, texH;
	float video_width, video_height, videoMinRatio, videoRatio;
	
	GLuint		particleVBO[3];
	
	double* screenLocs;
	bool*	visibles;
	
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	ofxCvGrayscaleImage* motionCVGrayscaleImage;
	
	// Values
	PVert*		pos;		// vertex (quad) of particle
	PVert*		posQuads;	// vertex (quad) of particle
	float*		dim;		// particle size (w/h)
	PTexture*	texcords;	// texture coords
	PColor*	colors;			// particle color rgba
	
	//////////////////////////////////////////////////////////////////////////////
	
	void initiate( int $numParticles, float $minX, float $maxX, float $minY, float $maxY, float $minZ, float $maxZ );
	void onBeat(float $pct);
	void setParticlePosition(int $i, float $px, float $py, float $pz, float $pDimension);
	void setParticleColor(int $i, float $r, float $g, float $b, float $a);
	void update();
	void draw();
};

#endif