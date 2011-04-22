/*
 *  RenderCreaturesScene.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 4/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RenderCreaturesScene.h"

void RenderCreaturesScene::setUpPanel(ofxControlPanel* $panel) {
	Scene::setUpPanel( $panel );
	
	panel->addPanel("Creature Render Settings", 1, false);
	panel->setWhichPanel("Creature Render Settings");
	panel->setWhichColumn(0);
	
	panel->addSlider("Spine Length", "F_SPINE_LENGTH", 12.f, 3.f, 40.f, false);
	panel->addSlider("Spine Max Rad", "F_SPINE_MAX_RAD", 20.f, 5.f, 50.f, false);
	panel->addSlider("Spine Min Rad", "F_SPINE_MIN_RAD", 10.f, 5.f, 30.f, false);
	panel->addSlider("Num Segments", "I_NUM_CREATURE_SEGS", 10, 4, 20, true);
	panel->addSlider("Extremety Spacing", "F_EXTREMETY_SPACING", 10.f, 2.f, 30.f, false);
	panel->addSlider("Tentacle Div Length", "F_TENTACLE_DIV_LENGTH", 10.f, 2.f, 40.f, false);
	panel->addSlider("Avg Life Span (Sec)", "I_AVG_LIFE_SPAN", 40, 00, 1800, true);
	panel->addSlider("Life Span Diff (Sec)", "I_LIFE_SPAN_DIFF", 20, 0, 300, true);
	panel->addSlider("Death Time Start (Pct)", "F_DEATH_TIME_START_PCT", .1f, 0.f, 1.f, false);
	
	
	panel->addPanel("Creature - Creature Settings", 2, false);
	panel->setWhichPanel("Creature - Creature Settings");
	panel->setWhichColumn(0);
	
	panel->addSlider("Perlin Force Mult", "PERLIN_FRC_MULT", 2, 0, 20, false);
	panel->addToggle("Add Perlin Noise", "B_ADD_PERLIN", true);
	
	panel->addSlider("Align Min Dist", "ALIGN_MIN_DIST", 80, 0, 700, false);
	panel->addSlider("Align Strength ", "ALIGN_STRENGTH", .015, 0, 10, false);
	
	panel->addSlider("Sep Min Dist ", "SEP_MIN_DIST", 35, 0, 700, false);
	panel->addSlider("Sep Strength ", "SEP_STRENGTH", .03, 0, 10, false);
	
	panel->addSlider("Cohesion Min Dist ", "COH_MIN_DIST", 90, 0, 700, false);
	panel->addSlider("Cohesion Strength ", "COH_STRENGTH", .015, 0, 10, false);
	
	panel->addSlider("Max Steer ", "MAX_STEER", .09, 0, 1, false);
	panel->addSlider("Max Speed ", "MAX_SPEED", 15, 0, 30, false);
	panel->addSlider("Damping ", "HEAD_DAMPING", .01, 0, 1, false);
	
	panel->setWhichColumn(1);
	panel->addSlider("Equal Pct Add", "EQUAL_PCT_ADD", .2f, 0.f, 1.f, false);
	panel->addSlider("Less Pct Add", "LESS_PCT_ADD", .2f, 0.f, 1.f, false);
	panel->addSlider("Greater Pct Add", "GREAT_PCT_ADD", .2f, 0.f, 1.f, false);
	
	panel->addSlider("Less Frc Str", "LESS_FRC_STR", -3.f, -15.f, 15.f, false);
	panel->addSlider("Greater Frc Str", "GREATER_FRC_STR", 1.1f, -15.f, 15.f, false);
	
	panel->addSlider("Pct Decrement", "F_CREATURE_PCT_DEC", .005f, .0f, .3f, false);
	
	panel->addSlider("Diff Creature Min Dist", "DIFF_CREATURE_MIN_DIST", 500.f, 100.f, 1000.f, false);
	
	panel->addSlider("Creature Min X", "CREATURE_MIN_X", -ofGetWidth(), -5000, 0, false);
	panel->addSlider("Creature Max X", "CREATURE_MAX_X", ofGetWidth() * 2, ofGetWidth() + 100, 5000 + ofGetWidth(), false);
	panel->addSlider("Creature Min Y", "CREATURE_MIN_Y", -ofGetHeight() * .5, -1500, 0, false);
	panel->addSlider("Creature Max Y", "CREATURE_MAX_Y",  ofGetHeight() * 1.5, ofGetHeight(), ofGetHeight() * 2, false);
	panel->addSlider("Creature Min Z", "CREATURE_MIN_Z", -ofGetHeight(), -ofGetHeight() * 2, 0, false);
	panel->addSlider("Creature Max Z", "CREATURE_MAX_Z", 0, -300, ofGetHeight(), false);
	
	panel->addSlider("Creature Spring Center", "CREATURE_SPRING_CENTER", .002f, 0.0f, .05f, false);
	
	panel->addPanel("Environment Settings", 2, false);
	panel->setWhichPanel( "Environment Settings" );
	panel->setWhichColumn( 0 );
	panel->addSlider("Num Particles", "I_NUM_PARTICLES", 3000, 200, 20000, true);
	panel->addSlider("Particle Min X", "PARTICLE_MIN_X", -ofGetWidth(), -5000, 0, false);
	panel->addSlider("Particle Max X", "PARTICLE_MAX_X", ofGetWidth() * 2, ofGetWidth() + 100, 5000 + ofGetWidth(), false);
	panel->addSlider("Particle Min Y", "PARTICLE_MIN_Y", -ofGetHeight() * .5, -1500, 0, false);
	panel->addSlider("Particle Max Y", "PARTICLE_MAX_Y", ofGetHeight() * 1.5, ofGetHeight(), ofGetHeight() * 2, false);
	panel->addSlider("Particle Min Z", "PARTICLE_MIN_Z", -ofGetHeight(), -ofGetHeight() * 2, 0, false);
	panel->addSlider("Particle Max Z", "PARTICLE_MAX_Z", 0, -300, ofGetHeight(), false);
	
	panel->addSlider("Particle Min W", "PARTICLE_MIN_DIMEN", 10.f, 2.f, 30.f, false);
	panel->addSlider("Particle Dim Dec", "PARTICLE_DEC", .1f, 0.0f, 3.f, false);
	
	panel->addSlider("Particle Red Min", "PARTICLE_RED_MIN", .1f, 0.0f, 1.f, false);
	panel->addSlider("Particle Red DEC", "PARTICLE_RED_DEC", .01f, 0.0f, .05f, false);
	panel->addSlider("Particle Green Min", "PARTICLE_GREEN_MIN", .1f, 0.0f, 1.f, false);
	panel->addSlider("Particle Green DEC", "PARTICLE_GREEN_DEC", .01f, 0.0f, .05f, false);
	panel->addSlider("Particle Blue Min", "PARTICLE_BLUE_MIN", .1f, 0.0f, 1.f, false);
	panel->addSlider("Particle Blue DEC", "PARTICLE_BLUE_DEC", .01f, 0.0f, .05f, false);
	panel->setWhichColumn( 1 );
	panel->addSlider("Particle Alpha Min", "PARTICLE_ALPHA_MIN", .1f, 0.0f, 1.f, false);
	panel->addSlider("Particle Alpha DEC", "PARTICLE_ALPHA_DEC", .01f, 0.0f, .05f, false);
	
	
	panel->addToggle("B Creature Avg Loc", "B_SHOW_CREATURE_AVG_LOC", true);
	panel->addSlider("Camera Min X", "CAMERA_MIN_X", -ofGetWidth(), -5000, 0, false);
	panel->addSlider("Camera Max X", "CAMERA_MAX_X", ofGetWidth() * 2, ofGetWidth() + 100, 5000 + ofGetWidth(), false);
	panel->addSlider("Camera Min Y", "CAMERA_MIN_Y", 0, -ofGetHeight() * .5, 0, false);
	panel->addSlider("Camera Max Y", "CAMERA_MAX_Y", ofGetHeight(), ofGetHeight(), ofGetHeight() * 2, false);
	panel->addSlider("Camera Min Z", "CAMERA_MIN_Z", -ofGetHeight(), -ofGetHeight() * 2, 0, false);
	panel->addSlider("Camera Max Z", "CAMERA_MAX_Z", 0, -300, ofGetHeight(), false);
	
	panel->addSlider("Cam Easing", "CAMERA_EASING", .2f, 0.f, 1.f, false);
	
	pManager.panel = panel;
	
}

void RenderCreaturesScene::setMotionCVImage( ofxCvGrayscaleImage* $motionCVGrayscaleImage ) {
	pManager.motionCVGrayscaleImage = $motionCVGrayscaleImage; // we wasnt to trace out some motion in this piece for particle interaction //
	motionCVGrayscaleImage = $motionCVGrayscaleImage;
}

void RenderCreaturesScene::setVideoAtts( float $video_width, float $video_height, float $videoMinRatio, float $videoRatio ) {
	pManager.video_width = $video_width;
	pManager.video_height = $video_height;
	pManager.videoMinRatio = $videoMinRatio;
	pManager.videoRatio = $videoRatio;
}

void RenderCreaturesScene::resetCamera() {
	// setup the camera, according to OF default camera settings // 
	float w = ofGetWidth();
	float h = ofGetHeight();
	
	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;
	
	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;
	
	cam->perspective( 60.f, aspect, nearDist, farDist);
	cam->position( eyeX, eyeY, dist);
	cam->eye( eyeX, eyeY, 0);
	
	cam->up(0, 1, 0);
}
// check to make sure that the avgCreatureLoc is not outside the bounds of the camera //
void RenderCreaturesScene::checkCameraBounds() {
	camMinBounds.set( panel->getValueF("CAMERA_MIN_X"), panel->getValueF("CAMERA_MIN_Y"), panel->getValueF("CAMERA_MIN_Z") );
	camMaxBounds.set( panel->getValueF("CAMERA_MAX_X"), panel->getValueF("CAMERA_MAX_Y"), panel->getValueF("CAMERA_MAX_Z") );
	
	if (avgCreatureLoc.x < camMinBounds.x) avgCreatureLoc.x = camMinBounds.x;
	if (avgCreatureLoc.x > camMaxBounds.x) avgCreatureLoc.x = camMaxBounds.x;
	
	if (avgCreatureLoc.y < camMinBounds.y) avgCreatureLoc.y = camMinBounds.y;
	if (avgCreatureLoc.y > camMaxBounds.y) avgCreatureLoc.y = camMaxBounds.y;
	
	if (avgCreatureLoc.z < camMinBounds.z) avgCreatureLoc.z = camMinBounds.z;
	if (avgCreatureLoc.z > camMaxBounds.z) avgCreatureLoc.z = camMaxBounds.z;
}

// don't forget to set the panel first //
void RenderCreaturesScene::initiate() {
	screenLocs	= new double[50 * 3];
	grayDiffSmall.allocate(60, 45);
	
	totalCreatures		= 0;
	
	perlin				= new ofxPerlin();
	counter				= 0.f;
	bMousePressed		= false;
	
	creaturesLastID		= 0;
	
	numCreatureSegmets = panel->getValueI("I_NUM_CREATURE_SEGS");
	pManager.initiate(panel->getValueI("I_NUM_PARTICLES"), panel->getValueF("PARTICLE_MIN_X"), panel->getValueF("PARTICLE_MAX_X"),
					  panel->getValueF("PARTICLE_MIN_Y"), panel->getValueF("PARTICLE_MAX_Y"), 
					  panel->getValueF("PARTICLE_MIN_Z"), panel->getValueF("PARTICLE_MAX_Z"));
	resetCamera();
	lastSentOscTime = ofGetElapsedTimeMillis();
}

void RenderCreaturesScene::addCreature( int $creatureType ) {
	switch ($creatureType) {
		case CREATURE_TYPE_RECTANGLES:
			creatures.push_back( new Tangles() );
			break;
		case CREATURE_TYPE_SPIKEY:
			creatures.push_back( new Spiky() );
			break;
		case CREATURE_TYPE_SLINKY:
			creatures.push_back( new Slinky() );
			break;
		default:
			break;
	}
	
	totalCreatures = creatures.size();
	initLastCreature();
	//ofColor color = ColorPalette::getCurrentColorForCreature( $creatureType );
	//ofColor baseColor = ColorPalette::getBaseBodyColor();
	ofColor highColor = ColorPalette::getWhiteHighlightColor();
	ofColor lowColor = ColorPalette::getCurrentColorForCreature( $creatureType );
	creatures[totalCreatures - 1]->setColors( highColor.r, highColor.g, highColor.b, highColor.a,
											  lowColor.r, lowColor.g, lowColor.b, lowColor.a );
	#ifdef USING_OSC
		osc->createNewCreature( $creatureType, totalCreatures - 1 );
	#endif
	ColorPalette::advanceColorForCreature( $creatureType );
	creatures[totalCreatures - 1]->spine[0].addForce( ofRandom(-2, 2), ofRandom(-2, 2), ofRandom(-1, 1) );
	//creaturesLastID++;
}

void RenderCreaturesScene::initLastCreature() {
	creatures[totalCreatures - 1]->initiate(numCreatureSegmets, CREATURE_NUM_BODY_PTS_PER_SEGMENT, panel->getValueF("F_SPINE_MAX_RAD"),
								panel->getValueF("F_SPINE_LENGTH"), panel->getValueF("F_EXTREMETY_SPACING"), panel->getValueF("F_TENTACLE_DIV_LENGTH"));
	//creatures[totalCreatures - 1]->ID = creaturesLastID;
}

void RenderCreaturesScene::advanceCreatureSegments() {
	for(int i = 0; i < totalCreatures; i++) {
		creatures[i]->advanceSegment();
	}
}

bool RenderCreaturesScene::advanceCreatureSegment( unsigned int $creatureIndex, int $segmentIndex ) {
	//cout << "RenderCreaturesScene :: advanceCreatureSegment : id = " << $creatureIndex << " segmentIndex = " << $segmentIndex << endl;
	//if ($creatureID > -1) {
	//	cout << "RenderCreaturesScene :: advanceCreatureSegment : 1" << endl;
	//	if ($creatureID < totalCreatures) {
			//cout << "RenderCreaturesScene :: advanceCreatureSegment : 2" << endl;
			
	
			creatures[$creatureIndex]->setActiveSegment( $segmentIndex );
			return true;
	//	}
	//}
	//return false;
}

void RenderCreaturesScene::deleteCreatureAtIndex( int $creatureIndex ) {
	if (creatures.size() > 0) {
		#ifdef USING_OSC
			osc->deleteCreature( $creatureIndex );
		#endif
		delete creatures[$creatureIndex];
		creatures.erase( creatures.begin() + $creatureIndex );
		totalCreatures = creatures.size();
	}
}

void RenderCreaturesScene::clearAllCreatures() {
	//cout << "RenderCreaturesScene :: clearAllCreatures : totalCreatures = " << totalCreatures << endl;
	#ifdef USING_OSC
		osc->deleteAllCreatures( );
	#endif
	for(int i = 0; i < totalCreatures; i++) {
		delete creatures[i];
	}
	creatures.clear();
	totalCreatures = creatures.size();
	
	//creaturesLastID = 0;
	
	cout << "RenderCreaturesScene :: clearAllCreatures : totalCreatures = " << totalCreatures << endl;
}

void RenderCreaturesScene::update() {
	minCreatureBounds.set( panel->getValueF("CREATURE_MIN_X"), panel->getValueF("CREATURE_MIN_Y"), panel->getValueF("CREATURE_MIN_Z") );
	maxCreatureBounds.set( panel->getValueF("CREATURE_MAX_X"), panel->getValueF("CREATURE_MAX_Y"), panel->getValueF("CREATURE_MAX_Z") );
	pManager.update();
	
	// set Flocking vars //
	float alignMinDist		= panel->getValueF("ALIGN_MIN_DIST");
	float alignStrength		= panel->getValueF("ALIGN_STRENGTH");
	
	float sepMinDist		= panel->getValueF("SEP_MIN_DIST");
	float sepStrength		= panel->getValueF("SEP_STRENGTH");
	
	float cohMinDist		= panel->getValueF("COH_MIN_DIST");
	float cohStrength		= panel->getValueF("COH_STRENGTH");
	
	float maxSteer			= panel->getValueF("MAX_STEER");
	float maxSpeed			= panel->getValueF("MAX_SPEED");
	float headDamping		= panel->getValueF("HEAD_DAMPING");
	
	float perlinFrcMult		= panel->getValueF("PERLIN_FRC_MULT");
	bool bAddPerlin			= panel->getValueB("B_ADD_PERLIN");
	
	float nonDec			= panel->getValueF("F_CREATURE_PCT_DEC"); // distance not less than minDist //
	
	for(int i = 0; i < totalCreatures; i++) {
		creatures[i]->lesserPct		-= nonDec;
		creatures[i]->greaterPct	-= nonDec;
		creatures[i]->equalPct		-= nonDec;
	}
	
	if (totalCreatures > 1) {
		ofxVec3f diff;
		
		float equalAdd		= panel->getValueF("EQUAL_PCT_ADD");
		float lessAdd		= panel->getValueF("LESS_PCT_ADD");
		float greaterAdd	= panel->getValueF("GREAT_PCT_ADD");
		float minDist		= panel->getValueF("DIFF_CREATURE_MIN_DIST");
		
		float lessFrcStr	= panel->getValueF("LESS_FRC_STR");
		float greaterFrcStr	= panel->getValueF("GREATER_FRC_STR");
		
		float dist			= 0.f;
		float distPct		= 0.f;
		
		// creatures->lesserPct, greaterPct, equalPct;
		
		for(int i = 0; i < totalCreatures - 1; i++) {
			for(int j = i + 1; j < totalCreatures; j++) {
				diff = creatures[i]->spine[0].loc - creatures[j]->spine[0].loc;
				dist = diff.length();
				if (dist < minDist) {
					distPct = 1 - (dist / minDist);
					if (creatures[i]->type == creatures[j]->type) { // EQUALS //
						creatures[i]->equalPct += distPct * equalAdd;
						creatures[j]->equalPct += distPct * equalAdd;
						// Flock your shit..
						creatures[i]->addForFlocking( creatures[j] );
					} else {
							
						if (creatures[i]->type == CREATURE_TYPE_RECTANGLES) {
							if (creatures[j]->type == CREATURE_TYPE_SLINKY) { // i < j
								creatures[i]->lesserPct += distPct * lessAdd;
								creatures[j]->greaterPct += distPct * greaterAdd;
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, lessFrcStr, greaterFrcStr);
							} else if (creatures[j]->type == CREATURE_TYPE_SPIKEY) { // i > j
								creatures[i]->greaterPct += distPct * greaterAdd;
								creatures[j]->lesserPct += distPct * lessAdd;
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, greaterFrcStr, lessFrcStr);
							}
						} else if (creatures[i]->type == CREATURE_TYPE_SLINKY) { 
							if (creatures[j]->type == CREATURE_TYPE_RECTANGLES) { // i > j 
								creatures[i]->greaterPct += distPct * greaterAdd;
								creatures[j]->lesserPct += distPct * lessAdd;
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, greaterFrcStr, lessFrcStr);
							} else if (creatures[j]->type == CREATURE_TYPE_SPIKEY) { // i < j
								creatures[i]->lesserPct += distPct * lessAdd;
								creatures[j]->greaterPct += distPct * greaterAdd;
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, lessFrcStr, greaterFrcStr);
							}
						} else if (creatures[i]->type == CREATURE_TYPE_SPIKEY) {
							if (creatures[j]->type == CREATURE_TYPE_RECTANGLES) { // i < j
								creatures[i]->lesserPct += distPct * lessAdd;
								creatures[j]->greaterPct += distPct * greaterAdd;
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, lessFrcStr, greaterFrcStr);
							} else if (creatures[j]->type == CREATURE_TYPE_SLINKY) { // i > j
								creatures[i]->greaterPct += distPct * greaterAdd;
								creatures[j]->lesserPct += distPct * lessAdd; 
								creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist, greaterFrcStr, lessFrcStr);
							}
						}
						// addForce(Particle3D& $p, float $minDist, float $scale) //
						creatures[i]->spine[0].addForce(creatures[j]->spine[0], minDist * .45, -50 * (1 - creatures[i]->motionPct) );
					}
				}
				
				// addForce(Particle3D& $p, float $minDist, float $scale)
				// creatures[i]->spine[0].addForce( creatures[j]->spine[0], 400.f, -3.5f);
			}
		}
	}
	
	bool bSendCreatureMessagesOSC = false;
	bool bSendCreatureMessagesOSC2 = false;
	#ifdef USING_OSC
		if (ofGetElapsedTimeMillis() - lastSentOscTime > 750) {
			bSendCreatureMessagesOSC = true;
			lastSentOscTime = ofGetElapsedTimeMillis();
		}
		if (ofGetElapsedTimeMillis() - lastSentOscTime2 > 1000) {
			bSendCreatureMessagesOSC2 = true;
			lastSentOscTime2 = ofGetElapsedTimeMillis();
		}
	#endif
	
	avgCreatureLoc.set(0, 0, 0);
	for(int i = 0; i < totalCreatures; i++) {
		creatures[i]->lesserPct = creatures[i]->lesserPct < 0 ? 0 : creatures[i]->lesserPct;
		creatures[i]->lesserPct = creatures[i]->lesserPct > 1 ? 1 : creatures[i]->lesserPct;
		
		creatures[i]->greaterPct = creatures[i]->greaterPct < 0 ? 0 : creatures[i]->greaterPct;
		creatures[i]->greaterPct = creatures[i]->greaterPct > 1 ? 1 : creatures[i]->greaterPct;
		
		creatures[i]->equalPct = creatures[i]->equalPct < 0 ? 0 : creatures[i]->equalPct;
		creatures[i]->equalPct = creatures[i]->equalPct > 1 ? 1 : creatures[i]->equalPct;
		
		
		
		//cout << "RenderCreatureScene :: update : creatures[i]->spine[0].loc = " << creatures[i]->spine[0].loc.x << ", " << creatures[i]->spine[0].loc.y
		//<< ", " << creatures[i]->spine[0].loc.z << endl;
		
		if (isnan(creatures[i]->spine[0].loc.x) || isnan(creatures[i]->spine[0].loc.y) || isnan(creatures[i]->spine[0].loc.z)) {
			//creatures[i]->spine[0].loc.set( avgCreatureLoc.x, avgCreatureLoc.y, avgCreatureLoc.z );
			deleteCreatureAtIndex( i );
			break;
		}
		
		avgCreatureLoc += creatures[i]->spine[0].loc;
		
		//cout << "RenderCreatureScene :: update : creature " << i << ": equal = " << creatures[i]->equalPct << 
		//" lesser = " << creatures[i]->lesserPct << " greater = " << creatures[i]->greaterPct << endl;
		
		if (bSendCreatureMessagesOSC) {
			//cout << "RenderCreatureScene :: update : creature " << i << ": equal = " << creatures[i]->equalPct << 
			//" lesser = " << creatures[i]->lesserPct << " greater = " << creatures[i]->greaterPct << endl;
			osc->sendCreatureBehaviors(creatures[i]->type - 1, i, creatures[i]->greaterPct, creatures[i]->lesserPct, creatures[i]->equalPct);
			if (creatures[i]->bDying) osc->creatureAmp( i, creatures[i]->deathPct );
			//cout << "RenderCreatureScene :: update : creatureIndex = " << i << endl;
		}
		if (bSendCreatureMessagesOSC2) {
			float panX = ofMap(screenLocs[i * 3], 0, ofGetWidth(), -1, 1);
			osc->setPan(i, panX);
		}
	}
	
	//cout << "RenderCreatureScene :: update : totalCreatures = " << totalCreatures << ", avgCreatureLoc before = " << avgCreatureLoc.x << ", " << avgCreatureLoc.y << endl;
	
	avgCreatureLoc /= (float)totalCreatures;
	
	//isnan(x) ((x) != (x))
	
	//checkCameraBounds(); // adjusts avgCreatureLoc to be within cam bounds //
	
	//cout << "RenderCreatureScene :: update : totalCreatures = " << totalCreatures << ", avgCreatureLoc before = " << avgCreatureLoc.x << ", " << avgCreatureLoc.y << endl;
	//if (totalCreatures < 3 && totalCreatures > 0) {
	//	cout << "RenderCreatureScene :: update : creatures[0].spine[0].loc = " << creatures[0]->spine[0].loc.x << ", " 
	//	<< creatures[0]->spine[0].loc.y << ", " << creatures[0]->spine[0].loc.z << endl;
	//}
	
	//cout << "RenderCreatureScene :: update : avgCreatureLoc = " << avgCreatureLoc << endl;
	
	//cout << "RenderCreatureScene :: update : avgCreatureLoc = " << avgCreatureLoc.x << ", " << avgCreatureLoc.y << endl;
	
	bool bIsNan = false;
	if (isnan(avgCreatureLoc.x) || isnan(avgCreatureLoc.y) || isnan(avgCreatureLoc.z)) {
		bIsNan = true;
	}
	
	if ( totalCreatures < 1 || bIsNan) {
		avgCreatureLoc.x = (float)ofGetWidth() * .5f;
		avgCreatureLoc.y = (float)ofGetHeight() * .5f;
		avgCreatureLoc.z = 0.f;
	}
	
	//cout << "RenderCreatureScene :: update : totalCreatures = " << totalCreatures << ", avgCreatureLoc after = " << avgCreatureLoc.x << ", " << avgCreatureLoc.y << endl;
	
	cam->place();
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	ofSetupScreen();
	
	// add user input to the motion of the creatures //
	if (totalCreatures > 0) {
		float VFStrength		= panel->getValueF("VF_MULTIPLIER");
		grayDiffSmall.scaleIntoMe( *motionCVGrayscaleImage );
		grayDiffSmall.blur(5); // really blur the image alot!
		//cout << "VF fieldSize = " << VF << endl;
		VF->setFromPixels(grayDiffSmall.getPixels(), false, VFStrength);
		ofxVec2f frc;
		bool bOnScreen;
		for (int i = 0; i < totalCreatures; i++) {
			bOnScreen = false;
			gluProject(creatures[i]->spine[0].loc.x, creatures[i]->spine[0].loc.y, creatures[i]->spine[0].loc.z,
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
				frc = VF->getForceFromPos(screenLocs[i * 3], screenLocs[i * 3 + 1]);
				frc *= 8;
				creatures[i]->spine[0].addForce(frc.x, frc.y, (frc.x + frc.y) * .002f);
			}
		}
	}
	
	float creatureSpringCenter = panel->getValueF("CREATURE_SPRING_CENTER");
	ofxVec3f perlinFrc;
	for(int i = 0; i < totalCreatures; i++) {
		if (bAddPerlin) {
			float xyRads      = perlin->getRads( creatures[i]->spine[0].loc.x, creatures[i]->spine[0].loc.z, counter, 10.0, .0125 );
			float yRads       = perlin->getRads( creatures[i]->spine[0].loc.x, creatures[i]->spine[0].loc.y, counter,  10.0, .0125 );
			perlinFrc.set( cos(xyRads), -sin(yRads), sin(xyRads) );
			//perlinFrc.set( cos(xyRads), -sin(yRads), 0 );
			perlinFrc *= perlinFrcMult;
			perlinFrc *= perlinFrc.normalized().dot( creatures[i]->spine[0].vel.normalized() );
			if (!bMousePressed) creatures[i]->spine[0].addForce( perlinFrc );
		}
		
		if (bMousePressed) {
			creatures[i]->spine[0].addForce( (ofxVec3f(mouseX, mouseY, 400) - creatures[i]->spine[0].loc) * .09f );
		}
		// steer(ofxVec3f $target, bool $slowdown, float $scale)
		//creatures[i]->spine[0].steer( avgCreatureLoc, false, .8f);
		if (totalCreatures > 1) creatures[i]->spine[0].spring( avgCreatureLoc, creatureSpringCenter );
		
		creatures[i]->addFlockingForces();
		
		creatures[i]->update();
		creatures[i]->constrain3D(minCreatureBounds, maxCreatureBounds);
		creatures[i]->resetFlockingForces();
		
		creatures[i]->alignment.minDist		= alignMinDist;
		creatures[i]->alignment.strength	= alignStrength;
		
		creatures[i]->seperation.minDist	= sepMinDist;
		creatures[i]->seperation.strength	= sepStrength;
		
		creatures[i]->cohesion.minDist		= cohMinDist;
		creatures[i]->cohesion.strength		= cohStrength;
		
		//creatures[i]->spine[0].DAMPING		= headDamping;
	}
	
	// check if the creatures should die or not //
	//int lifeSpan = panel->getValueI("I_AVG_LIFE_SPAN") * 1000;
	//unsigned int elapsedTime = ofGetElapsedTimeMillis();
	
	for(int i = 0; i < totalCreatures; i++) {
		if (totalCreatures > 10) {
			if (creatures[i]->bDying) {
				break;
			}
			if (!creatures[i]->bDying) {
				//creatures[i]->deathTime = ofGetElapsedTimeMillis() + 2000;
				creatures[i]->startDying( 2000.f );
				break;
			}
		}
	}
	
	for(int i = 0; i < totalCreatures; i++) {
		if (totalCreatures == 3) {
			if(i > 0) {
				if(creatures[i]->bDying) {
					creatures[i]->deathTime = ofGetElapsedTimeMillis() + 10000;
					creatures[i]->spawnTime = ofGetElapsedTimeMillis();
				}
			}
		} else if (totalCreatures < 3) {
			if(creatures[i]->bDying) {
				creatures[i]->deathTime = ofGetElapsedTimeMillis() + 10000;
				creatures[i]->spawnTime = ofGetElapsedTimeMillis();
			}
		}
		if (creatures[i]->bRemoveMe) {
			deleteCreatureAtIndex( i );
			break; // so we don't get confused in the loop //
		}
	}
	
	checkCameraBounds(); // adjusts avgCreatureLoc to be within cam bounds //
	
	//float diffX = avgPt.x - cam.getPosition().x;
	//float diffY = (ofGetHeight() - avgPt.y) - cam.getPosition().y;
	camEasing = panel->getValueF("CAMERA_EASING");
	float distX = (avgCreatureLoc.x - cam->getPosition().x) * camEasing;
	float distY = ( (camMaxBounds.y - avgCreatureLoc.y) - cam->getPosition().y ) * camEasing;
	
	cam->track( distX, distY );
	
	counter += .0009f;
}

void RenderCreaturesScene::draw() {
	cam->place();
	pManager.draw();
	
	if (panel->getValueB("B_SHOW_CREATURE_AVG_LOC")) {
		ofSetColor(0, 255, 0);
		glPushMatrix();
		glTranslatef(avgCreatureLoc.x, avgCreatureLoc.y, avgCreatureLoc.z);
		ofCircle(0, 0, 5);
		glPopMatrix();
	}
	
	if (bMousePressed) {
		for (int i = 0; i < totalCreatures; i++) {
			//glBegin(GL_LINES);
			//glVertex3f(mouseX, mouseY, 0);
			//glVertex3f(creatures[i]->spine[0].loc.x, creatures[i]->spine[0].loc.y, creatures[i]->spine[0].loc.z);
			//glEnd();
		}
	}
	
	for (int i = 0; i < totalCreatures; i++) {
		creatures[i]->draw();
	}	
	
	//ofSetColor(255, 255, 255);
	//for (int i = 0; i < totalCreatures; i++) {
	//	ofDrawBitmapString(ofToString(screenLocs[i * 3], 0)+", "+ofToString(screenLocs[i * 3 + 1], 0), screenLocs[i * 3], screenLocs[i * 3 + 1] - 20);
	//}
}


