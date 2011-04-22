/*
 *  ParticleManager.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleManager.h"

void ParticleManager::initiate( int $numParticles, float $minX, float $maxX, float $minY, float $maxY, float $minZ, float $maxZ ) {
	// panel is set by RenderCreatureScene in setPanel function //
	cout << "ParticleManager :: initiate : numParticles = " << $numParticles << endl;
	NUM_PARTICLES = $numParticles;
	
	ofDisableArbTex();
	texture.loadImage("particle.png");
	ofEnableArbTex();
	//texture.setImageType(OF_IMAGE_COLOR_ALPHA);
	
	texW = 30;
	texH = texW;
	
	float r, g, b, a;
	r = g = b = a = 1.f;
	r = 1.f;
	g = 1.f;
	b = 1;
	a = 1.f;
	
	pos			= new PVert[NUM_PARTICLES];
	posQuads	= new PVert[NUM_PARTICLES * 4];
	dim			= new float[NUM_PARTICLES];
	texcords	= new PTexture[NUM_PARTICLES * 4];
	colors		= new PColor[NUM_PARTICLES * 4];
	screenLocs	= new double[NUM_PARTICLES * 3];
	
	visibles = new bool[NUM_PARTICLES];
	
	float px, py, pz;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		
		//pt.x = 200.f;
		px = ofRandom($minX, $maxX);
		py = ofRandom($minY, $maxY);
		pz = ofRandom($minZ, $maxZ);
		
		//px = ofRandom(0, ofGetWidth());
		//py = ofRandom(0, ofGetHeight());
		//pz = ofRandom(0, 1);
		
		//px = 100;
		//py = 100;
		//pz = 0;
		
		dim[i] = texW * ofRandom(.5, 1.f);
		
		setParticleColor( i, r, g, b, a);
		
		pos[i].x = px;
		pos[i].y = py;
		pos[i].z = pz;
		
		setParticlePosition( i, px, py, pz, dim[i]);
		
		// P1
		texcords[(i*4)+0].u = 0;//(cellWidth * row)		/ texW;
		texcords[(i*4)+0].v = 0;//(cellHeight * col)	/ texH;
		
		// P2
		texcords[(i*4)+1].u = .9;//((cellWidth * row)	+ cellWidth)	/ texW;
		texcords[(i*4)+1].v = 0;//(cellHeight * col)	/ texH;
		
		// P2
		texcords[(i*4)+2].u = .9;//((cellWidth * row) + cellWidth)		/ texW;
		texcords[(i*4)+2].v = .9;//((cellHeight * col) + cellHeight)	/ texH;	
		
		// P2
		texcords[(i*4)+3].u = 0;//(cellWidth * row)		/ texW;
		texcords[(i*4)+3].v = .9;//((cellHeight * col)+cellHeight)	/ texH;	
		
		
		
	}
	
	glGenBuffersARB(3, &particleVBO[0]);
	
	// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (NUM_PARTICLES*4)*4*sizeof(float), colors, GL_STREAM_DRAW_ARB);
	
	// vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (NUM_PARTICLES*4)*3*sizeof(float), posQuads, GL_STREAM_DRAW_ARB);
	
	// texture coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (NUM_PARTICLES*4)*2*sizeof(float), texcords, GL_STREAM_DRAW_ARB);
	
	
}

void ParticleManager::setParticlePosition(int $i, float $px, float $py, float $pz, float $pDimension) {
	float halfPDim = $pDimension * .5f;
	
	// P1
	posQuads[($i*4)+0].x = $px - halfPDim;
	posQuads[($i*4)+0].y = $py - halfPDim;
	posQuads[($i*4)+0].z = $pz - halfPDim;
	
	// P2
	posQuads[($i*4)+1].x = $px + halfPDim;
	posQuads[($i*4)+1].y = $py - halfPDim;
	posQuads[($i*4)+1].z = $pz - halfPDim;
	
	// P3
	posQuads[($i*4)+2].x = $px + halfPDim;
	posQuads[($i*4)+2].y = $py + halfPDim;
	posQuads[($i*4)+2].z = $pz - halfPDim;
	
	// P4
	posQuads[($i*4)+3].x = $px - halfPDim;
	posQuads[($i*4)+3].y = $py + halfPDim;
	posQuads[($i*4)+3].z = $pz - halfPDim;
	
}

void ParticleManager::setParticleColor(int $i, float $r, float $g, float $b, float $a) {
	if ($r < 0) $r = 0;
	if ($r > 1) $r = 1.f;
	
	if ($g < 0) $g = 0;
	if ($g > 1) $g = 1.f;
	
	if ($b < 0) $b = 0;
	if ($b > 1) $b = 1.f;
	
	if ($a < 0) $a = 0;
	if ($a > 1) $a = 1.f;
	
	// Color 1
	colors[($i*4)+0].r = $r;
	colors[($i*4)+0].g = $g;
	colors[($i*4)+0].b = $b;
	colors[($i*4)+0].a = $a;
	
	// Color 2
	colors[($i*4)+1].r = $r;
	colors[($i*4)+1].g = $g;
	colors[($i*4)+1].b = $b;
	colors[($i*4)+1].a = $a;
	
	// Color 3
	colors[($i*4)+2].r = $r;
	colors[($i*4)+2].g = $g;
	colors[($i*4)+2].b = $b;
	colors[($i*4)+2].a = $a;
	
	// Color 4
	colors[($i*4)+3].r = $r;
	colors[($i*4)+3].g = $g;
	colors[($i*4)+3].b = $b;
	colors[($i*4)+3].a = $a;
	
}

void ParticleManager::onBeat(float $pct) {
	float r, g, b, a;
	r = g = b = a = 1.f;
	a = $pct;
	
	float pDimension = 1.f * (texW);
	
	for (int i = 0; i < NUM_PARTICLES; i++) {
		
		dim[i] = texW;
		
		setParticleColor( i, r * ofRandom(0, 1), g, b, a * ofRandom(0, 1) );
		
		setParticlePosition(i, pos[i].x, pos[i].y, pos[i].z, dim[i]);
	}
}

void ParticleManager::update() {
	float minDimension		= panel->getValueF("PARTICLE_MIN_DIMEN");
	float dimenDec			= panel->getValueF("PARTICLE_DEC");
	
	float redMin			= panel->getValueF("PARTICLE_RED_MIN");
	float redDec			= panel->getValueF("PARTICLE_RED_DEC");
	float greenMin			= panel->getValueF("PARTICLE_GREEN_MIN");
	float greenDec			= panel->getValueF("PARTICLE_GREEN_DEC");
	float blueMin			= panel->getValueF("PARTICLE_BLUE_MIN");
	float blueDec			= panel->getValueF("PARTICLE_BLUE_DEC");
	float alphaMin			= panel->getValueF("PARTICLE_ALPHA_MIN");
	float alphaDec			= panel->getValueF("PARTICLE_ALPHA_DEC");
	
	float r, g, b, a;
	float adjX, adjY, pixPct;
	int pixIndex;
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	
	unsigned char* pixels = motionCVGrayscaleImage->getPixels();
	*/
	bool bOnScreen;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		/*bOnScreen = true;
		gluProject(pos[i].x, pos[i].y, pos[i].z,
				   mvmatrix, projmatrix, viewport,
				   &screenLocs[i * 3], &screenLocs[i * 3 + 1], &screenLocs[i * 3 + 2]);
		
		screenLocs[i * 3 + 1] = ofGetHeight() - screenLocs[i * 3 + 1];
		
		if (screenLocs[i * 3] > 0) {
			if (screenLocs[i * 3] < ofGetWidth()) {
				if (screenLocs[i * 3 + 1] > 0) {
					if (screenLocs[i * 3 + 1] < ofGetHeight()) {
						bOnScreen = true;
					}
				}
			}
		}
		if (bOnScreen) {
			//r = 1.f;
			//cout << "videoMinRatio: " << videoMinRatio << endl;
			// check for brightess in cvImage //
			//adjX = screenLocs[i * 3    ] * videoMinRatio;
			adjX = screenLocs[i * 3    ] * videoMinRatio;
			adjY = screenLocs[i * 3 + 1] * videoMinRatio;
			//adjY = 0;
			//adjY = screenLocs[i * 3 + 1] * videoMinRatio;
			adjX = adjX < 0 ? 0 : adjX;
			adjX = adjX > video_width ? video_width : adjX;
			adjY = adjY < 0 ? 0 : adjY;
			adjY = adjY > video_height ? video_height : adjY;
			
			//cout << "screenLocs : x, y " << screenLocs[i * 3] << ", " << screenLocs[i * 3 + 1] << " adjusted: x, y " << adjX << ", " << adjY << endl;
			pixPct = (adjY) * 320.f + (adjX);
			pixIndex = pixPct;
			pixPct = motionCVGrayscaleImage->getPixels()[pixIndex];
			//pixPct = pixels[pixIndex] / 255.f;
			//cout << " this is the pixPct " << pixPct << endl;
			if (pixPct > 100.0) {
				//colors[i * 4].r = 1.f;
				colors[i * 4].a = 1;
				visibles[i] = true;
			} else {
				//g = colors[i * 4].g = 1.f;
				//a = alphaMin;
				visibles[i] = false;
			}
			//r = pixPct;
			
		} else {
			//a = alphaMin;
			visibles[i] = false;
		}
		*/
		//setParticleColor( i, r, g, b, a );
		
		//dim[i] -= dimenDec;
		if (dim[i] < minDimension) dim[i] = minDimension;
		
		//setParticlePosition(i, pos[i].x, pos[i].y, pos[i].z, dim[i]);
		
		r = colors[i * 4].r -= redDec;
		g = colors[i * 4].g -= greenDec;
		b = colors[i * 4].b -= blueDec;
		a = colors[i * 4].a -= alphaDec;
		
		r = r < redMin ? redMin : r;
		g = g < greenMin ? greenMin : g;
		b = b < blueMin ? blueMin : b;
		a = a < alphaMin ? alphaMin : a;
		
		setParticleColor( i, r, g, b, a );
	}
}

void ParticleManager::draw() {
	ofSetColor(255, 255, 255);
	//motionCVGrayscaleImage->draw(0, 0, ofGetWidth(), ofGetHeight());
	//glEnable( GL_BLEND );
	// Define the blend mode
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
	//ofEnableAlphaBlending();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	// bind tex coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (NUM_PARTICLES*4)*2*sizeof(float), texcords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	
	// bind color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (NUM_PARTICLES*4)*4*sizeof(float), colors);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
	// bind vertices [these are quads]
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (NUM_PARTICLES*4)*3*sizeof(float), posQuads);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	
	// draw the vbo
	//glDisable(GL_DEPTH_TEST);
	ofEnableArbTex();	
	
	
	texture.getTextureReference().bind();
	
	glDrawArrays(GL_QUADS, 0, NUM_PARTICLES * 4);
	
	texture.getTextureReference().unbind();
	//ofDisableAlphaBlending();
	
	ofDisableArbTex();
	//glEnable(GL_DEPTH_TEST);
	//glDisable( GL_BLEND );
	//glutSwapBuffers();
	glDepthMask(GL_TRUE);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	
	ofSetColor(100, 100, 100);
	//ofDrawBitmapString(ofToString(mouseX, 0)+", "+ofToString(mouseY, 0), mouseX + 2, mouseY - 2);
	/*
	ofSetColor(255, 255, 255);
	for(int i = 0; i < NUM_PARTICLES; i++) {
		if (visibles[i]) {
			ofSetColor(0, 255, 0);
		} else {
			ofSetColor(255, 0, 0);
		}
		//ofSetColor(visibles[i] * 250, 255, colors[i] * 250);
		ofCircle(pos[i].x, pos[i].y, 5);
	}
	 */
	//for (int i = 0; i < NUM_PARTICLES; i++) {
	//	ofDrawBitmapString( ofToString(screenLocs[i * 3], 0) +", "+ofToString(screenLocs[i * 3 + 1], 0),
	//					   screenLocs[i * 3] + 2, screenLocs[i * 3 + 1] + 2);
	//}
}