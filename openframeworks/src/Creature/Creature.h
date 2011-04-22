/*
 *  Creature.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CREATURE_
#define _CREATURE_

#include "Constants.h"
#include "CreatureSegment.h"
#include "MovementSegment.h"
#include "ofx3DUtils.h"
#include "ofxVec3f.h"
#include "ColorPalette.h"

// flocking code based on Zach Lieberman's
typedef struct{
	int			count;
	ofxVec3f	sum;
	float		minDist;
	float		strength;
} flockingForce;

class Creature {
public:
	flockingForce cohesion;
	flockingForce seperation;
	flockingForce alignment;
	
	int numMovementPts;
	int numBodyPts;
	int numSegmentPts;
	float spineLength; // length between pts //
	//int status;
	int type; // what type of creature is it? ie CREATURE_TYPE_RECTANGLES. located in Constants, set in derived class constructor //
	int segmentIndex; // current active segment //
	float length; // the length of the creature //
	float motionPct; // used to dampen forces in update // 0 - 1
	bool bRemoveMe; // should I be removed? //
	bool bDying;
	
	bool bRenderWireFrame;
	
	//unsigned int ID;
	
	unsigned int spawnTime; // when was this cat created? we need to know so that we can kill the mofo later //
	unsigned int deathTime; // know when this cat is going to die so that we can interpolate and add color fades to grey and shit //
	float deathPct; // calculated in Main Creature class update function so that it can be applied to sub classes //
	float deathStartPct; // at what pct should the creature begin to die 1 - 0 // 0 is completely deadsky //
	
	float tentacleDivLength;
	
	float segAngleInc;
	
	bool bHasTexture;
	
	bool bFirstRun; // set atts only on first run //
	
	Particle3D tagParticle; // used to position the texture //
	
	// rock, paper + scissors rules, if close enough, apply rules //
	float lesserPct, greaterPct, equalPct; // used to send to SC //
	
	vector <CreatureSegment>	spine; // used to render worm //
	vector <MovementSegment>	movementPts; // used to store movement of user //
	
	vector <ofxVec3f*> planeNormals;
	vector <ofxVec3f*> planeCenters;
	int totalPlaneNormals;
	
	vector <ofxVec3f> extrusionPts; // used for storing calcs of rec and spike extrusions for tangles and spiky //
	
	//string name;
	ofRectangle nameRect;
	ofTexture texture;
	
	// colors for highlighting of segments and extremeties //
	ofColor segmentHighColor;
	ofColor segmentBaseColor;
	
	ofColor extremetyHighColor; // tendrils, tangles, spikes //
	ofColor extremetyBaseColor;
	
	bool bNewSegmentSelected; // keep track of when segments are entered so that we know when to send out events //
	
	~Creature();
	
	virtual void initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength);
	void setUpForRecording(float $x, float $y, float $radius);
	void setCurrentRecordingSegmentLocation(int $segmentIndex, float $x, float $y);
	void reset( float $radius );
	void setRandomRecordingSegmentLocations(float $radius);
	void setMovementPtsRadius(ofxVec3f $center, float $targetRadius, float $radiusScalar);
	void setRandomRecordingNotesHit();
	void setBodyRadiusFromMotion( float $motionPct, float $midPct, float $minRadius, float $maxRadius );
	void setMovementAtts( float $motionPct );
	void setLifeAtts( float $healthPct, int $lifeSpan, int $lifeSpanDiff, float $deathStartPct );
	void startDying( float $lifeSpan );
	
	void setSegmentColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a );
	void setExtremetyColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a );
	void setColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a );
	
	void addForFlocking( Creature* $cr );
	void addFlockingForces();
	void resetFlockingForces();
	
	void setCreatureAlongVector(ofxVec3f $centerAxis, ofxVec3f $directionNormal);
	
	void setTextureFromPixels(unsigned char* $pixels);
	void advanceSegment( );
	void setActiveSegment(int $segmentIndex);
	void getNotesForSegment( int* $array, int $segmentIndex);
	
	void constrain3D(float $width, float $height, float $depth);
	void constrain3D( const ofxVec3f& $minBounds, const ofxVec3f& $maxBounds );
	
	virtual void update();
	virtual void draw(); // going to override in each creature, so should be virtual so that we can call it the same in creature //
};



#endif