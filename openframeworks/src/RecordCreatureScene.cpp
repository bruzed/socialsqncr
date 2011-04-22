/*
 *  RecordCreatureScene.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RecordCreatureScene.h"

void RecordCreatureScene::setUpPanel(ofxControlPanel* $panel) {
	Scene::setUpPanel( $panel );
	cSegmentManager = new CircleSegmentManager();
	cSegmentManager->panel = $panel;
	
	cSegmentManager_f = new CircleSegmentManager();
	cSegmentManager_f->panel = $panel;
	
	cSegmentManager_b = new CircleSegmentManager();
	cSegmentManager_b->panel = $panel;
	
	panel->addPanel("Recording Settings", 1, false);
	
	panel->setWhichPanel("Recording Settings");
	panel->setWhichColumn(0);
	
	panel->addToggle("Show Debug Shortcuts", "B_SHOW_DEBUG_INFO", true);
	
	panel->addToggle("Show Bounding Boxes ", "B_SHOW_BOUNDING_BOXES", false);
	panel->addToggle("Show Triangles ", "B_SHOW_TRIANGLES", false);
	panel->addToggle("Show Vector Field", "B_SHOW_VECTOR_FIELD", true);
	panel->addSlider("Vector Field Multiplier ", "VF_MULTIPLIER", .45f, 0, 1.f, false);
	panel->addSlider("Creature Record Radius", "CREATURE_RECORDING_RESET_RADIUS", 450.f, 300.f, 500.f, false);
	panel->addSlider("Creature Rad Diff Mult", "CREATURE_RAD_DIFF_MULT", .1f, 0.f, .5f, false);
	panel->addSlider("Random Max Diff", "CREATURE_RAND_MAX_DIFF", 120.f, 20.f, 300.f, false);
	panel->addSlider("Max Movement Pt Radius", "F_MAX_MOVEMENT_PT_RADIUS", 200.f, 50.f, 500.f, false);
	panel->addSlider("Add Radius for Note Hit", "F_ADD_RADIUS_FOR_NOTE_HIT", 30.f, 0.f, 70.f, false);
	panel->addSlider("Note Hit Multiplier", "F_NOTE_HIT_MULT", 0.f, 2.f, 10.f, false);
	panel->addSlider("Max Motion Pct", "F_MAX_MOTION_RECORD_PCT", 0.12f, 0.0f, 0.2f, false);
	panel->addSlider("Min Motion Pct", "F_MIN_MOTION_RECORD_PCT", 0.02f, 0.f, 0.2f, false);
}

void RecordCreatureScene::initiate(int $numPts, int $numSegmentPts, float $maxRadius, ofxVec3f $center, float $videoMinRatio) {
	
	center.set($center.x, $center.y, $center.z);
	
	cSegmentManager->setup( center.x, center.y );
	cSegmentManager_f->setup(center.x, center.y);
	cSegmentManager_b->setup(center.x, center.y);
	
	cSegmentManager_f->visible = false;
	cSegmentManager_b->visible = false;
	
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		cSegmentManager_f->segments[i].bIsActive = true;
		cSegmentManager_b->segments[i].bIsActive = true;
	}
	
	numMovementPts	= $numPts; // stores the amount of recording points
	numSpinePts		= numMovementPts + 1;
	numSegmentPts	= $numSegmentPts;
	
	segmentIndex	= -1;
	segAngleInc		= TWO_PI / (float) numSegmentPts;
	
	videoMinRatio = $videoMinRatio;
	
	grayDiffSmall.allocate(60, 45);
	
	currentCreatureInt = -1;
	
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		frequencies[i] = i * 220 + 220;
	}
	
	for (int i = 0; i < numMovementPts + 1; i++) {
		RecordSegment rs;
		rs.loc.set( 0, 0, 0 );
		movementPts.push_back( rs );
		movementPts[movementPts.size()-1].initiate(numSegmentPts, $maxRadius, 0, 0, 0);
	}
	bAnimInComplete = false;
	bAnimOutComplete = false;
	bTransitioning = false;
	
	bRecording = false;
	bRecordingComplete = false;
	
	mainMessage_txt = new TextField( FontBook::getFontForSize(24) );
	countDown_txt = new TextField( FontBook::getFontForSize(120) );
	
	hitZones = new float[numSpinePts];
	
	bCreatureVisible = false;
	creature = 0;
}

void RecordCreatureScene::animateIn() {
	bAnimInComplete		= false;
	bTransitioning		= true;
	bRecording			= false;
	bRecordingComplete	= false;
	segmentIndex		= 0;
	bDrawCreature		= false;
	
	// set the notes to not active, so that we can animate in laters //
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		cSegmentManager->segments[i].bIsActive = false;
		cSegmentManager->segments[i].hitPct = 0.f;
	}
	
	mainMessage_txt->setText("Hit the triggers to build your creature");
	mainMessage_txt->visible = true;
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = center.y - 130.f;
	mainMessage_txt->color.a = 0;
	
	motion_totalWhitePixPoss = motion_whitePixHit = motionHitPct = 0;
	for(int i = 0; i < numSpinePts; i++) {
		hitZones[i] = 0.f;
	}
	
	// colorize this dirty piece //
	ofColor color = ColorPalette::getCurrentColorForCreature( status );
	cSegmentManager->colorize(color.r, color.g, color.b);
	cSegmentManager_f->colorize(color.r, color.g, color.b);
	cSegmentManager_b->colorize(color.r, color.g, color.b);
	
	bCreatureVisible = false;
	if (creature != 0) delete creature;
	// set up the creature // 
	switch (status) {
		case CREATURE_TYPE_RECTANGLES:
			creature = new Tangles();
			break;
		case CREATURE_TYPE_SPIKEY:
			creature = new Spiky();
			break;
		case CREATURE_TYPE_SLINKY:
			creature = new Slinky();
			break;
		default:
			break;
	}
	
	creature->initiate(panel->getValueI("I_NUM_CREATURE_SEGS"), CREATURE_NUM_BODY_PTS_PER_SEGMENT, panel->getValueF("F_SPINE_MAX_RAD"),
									 panel->getValueF("F_SPINE_LENGTH"), panel->getValueF("F_EXTREMETY_SPACING"), panel->getValueF("F_TENTACLE_DIV_LENGTH"));
	creature->setBodyRadiusFromMotion( .5f, .5f, panel->getValueF("F_SPINE_MIN_RAD"), panel->getValueF("F_SPINE_MAX_RAD") );
	ofColor highColor = ColorPalette::getWhiteHighlightColor();
	ofColor lowColor = ColorPalette::getCurrentColorForCreature( status );
	creature->setColors( highColor.r, highColor.g, highColor.b, highColor.a, lowColor.r, lowColor.g, lowColor.b, lowColor.a );
	creature->segmentIndex = -1;
	creature->bRenderWireFrame = true;
	
	//setRandomPointsForCreature( creature, center, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"),
	//										panel->getValueF("CREATURE_RAD_DIFF_MULT"), panel->getValueF("CREATURE_RAND_MAX_DIFF"));
	
	creature->setLifeAtts( 1.f, 10000, 1000, .01f );
	//creature->setCreatureAlongVector(ofxVec3f(ofGetWidth() * .5, ofGetHeight() * .5, 500), ofxVec3f(-1, 0, 0) );
	creatureTargetCenter.set(ofGetWidth() * .5, ofGetHeight() * .5 + 10, 500);
	creatureCenter.set(creatureTargetCenter.x, creatureTargetCenter.y + 70, creatureTargetCenter.z);
	
	//////////////////////////////////////////////////////////////////
	
	Tweenzor::add( &cSegRotZ, -70, 0, 0.f, 1.f, EASE_OUT_QUART );
	Tweenzor::add( &mainMessage_txt->color.a, mainMessage_txt->color.a, 255.f, .5f, .5f, EASE_IN_OUT_SINE );
	
	
	currActiveBtnIndex = 0;
	float totalNumNotesPerSeg = (float) CREATURE_NUM_NOTES_PER_SEGMENT - 1;
	Tweenzor::add( &currActiveBtnIndex, currActiveBtnIndex, totalNumNotesPerSeg, 1.5f, 1.0f, EASE_LINEAR );
	Tweenzor::getRecentTween()->addListener( Tween::UPDATE, this, &RecordCreatureScene::onBtnAnimInTweenUpdate);
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onAnimInComplete_1); // skip onAnimInComplete_1 //
}

void RecordCreatureScene::onBtnAnimInTweenUpdate(float $args ) {
	currActiveBtnIndex = round( $args );
	if (!cSegmentManager->segments[currActiveBtnIndex].bIsActive) {
		cSegmentManager->segments[currActiveBtnIndex].bIsActive = true;
		cSegmentManager->segments[currActiveBtnIndex].hitPct = 1.f;
	}
}

void RecordCreatureScene::onAnimInComplete_1(float args) {
	countDown_txt->setText("10");
	countDown_txt->visible = true;
	countDown_txt->x = center.x - (countDown_txt->width * .5);
	countDown_txt->y = center.y + 70.f;
	countDown_txt->color.a = 0;
	
	Tweenzor::add( &countDown_txt->color.a, countDown_txt->color.a, 255.f, .5f, 1.f, EASE_IN_OUT_SINE );
	
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onAnimInComplete_2);
}

void RecordCreatureScene::onAnimInComplete_2(float args) {
	mainMessage_txt->setText("GO!");
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = center.y - 130.f;
	Tweenzor::add( &countDown_txt->color.a, countDown_txt->color.a, 255.f, 0.f, .5f, EASE_IN_OUT_SINE );
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onAnimInComplete_3);
}

void RecordCreatureScene::onAnimInComplete_3(float args) {
	mainMessage_txt->visible = false;
	bDrawCreature = true;
	Tweenzor::add( &creatureCenter.y, creatureCenter.y, creatureTargetCenter.y, 0.f, .5, EASE_IN_OUT_SINE);
	Tweenzor::add( &countDown_txt->y, countDown_txt->y, center.y - 180.f, 0.f, .25f, EASE_IN_OUT_SINE );
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onAnimInComplete);
}

void RecordCreatureScene::onAnimInComplete(float args) {
	bAnimInComplete = true;
	bTransitioning = false;
	
	bRecording = true;
	bRecordingComplete = false;
	
	startTime = ofGetElapsedTimeMillis();
	
	setUpForRecording(center.x, center.y, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"));
	setCurrentRecordingSegmentLocation( segmentIndex, center.x, center.y, false );
	creature->setActiveSegment( segmentIndex );
	
	// start the Metronome !! //
	#ifdef USING_OSC
		osc->startMetronome();
	#endif
}

void RecordCreatureScene::animateOut() {
	#ifdef USING_OSC
		osc->stopMetronome();
	#endif
	
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		cSegmentManager->segments[i].bIsActive = false;
		cSegmentManager->segments[i].hitPct = 0.f;
	}
	bAnimOutComplete = false;
	bTransitioning = true;
	countDown_txt->visible = false;
	mainMessage_txt->visible = true;
	mainMessage_txt->setText("Creature Complete");
	mainMessage_txt->x = center.x - (mainMessage_txt->width * .5);
	mainMessage_txt->y = center.y - 180.f;
	
	// how much movement was there //
	calculateMotionPercentages();
	
	Tweenzor::add( &mainMessage_txt->color.a, mainMessage_txt->color.a, 0.f, 2.f, .5f, EASE_IN_OUT_SINE );
	Tweenzor::add( &cSegRotZ, cSegRotZ, 400.f, 2.1f, .5, EASE_IN_QUART);
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onAnimOutComplete);
}

void RecordCreatureScene::onAnimOutComplete(float args) {
	bAnimOutComplete = true;
	bTransitioning = false;
	bAnimInComplete = false;
}

void RecordCreatureScene::calculateMotionPercentages() {
	float maxHitPct = panel->getValueF("F_MAX_MOTION_RECORD_PCT");
	motion_whitePixHit = 0.f;
	float maxPosPctPixs = 0;
	for(int i = 0; i < numSpinePts; i++) {
		motion_whitePixHit += hitZones[i];
		if (hitZones[i] > maxPosPctPixs) {
			maxPosPctPixs = hitZones[i];
			motionPosPct = (float)i / (float)(numSpinePts - 1.f);
		}
	}
	
	motionHitPct = (float)motion_whitePixHit / (float)motion_totalWhitePixPoss;
	cout << "RecordCreatureScene :: calculateMotionPercentages : motionHitPct not mapped = " << motionHitPct << endl;
	if (motionHitPct > maxHitPct) motionHitPct = maxHitPct;
	if (motionHitPct < 0) motionHitPct = 0;
	float minMotionHitPct = panel->getValueF("F_MIN_MOTION_RECORD_PCT");
	if (motionHitPct < minMotionHitPct) motionHitPct = minMotionHitPct;
	motionHitPct = ofMap(motionHitPct, minMotionHitPct, maxHitPct, 0.f, 1.f);
	cout << "RecordCreatureScene :: calculateMotionPercentages : motionHitPct mapped = " << motionHitPct << " motionPosPct = " << motionPosPct << endl;
}

// Note Functions for the Creatures ///////////////////////////////////////
void RecordCreatureScene::setRandomNotesHitForCreature( Creature* $cr ) {
	setRandomRecordingNotesHit();
	for (int i = 0; i < numMovementPts; i++) {
		for(int j = 0; j < CREATURE_NUM_NOTES_PER_SEGMENT; j++) {
			$cr->movementPts[i].notes[j] = movementPts[i].notes[j];
		}
	}
}

void RecordCreatureScene::setRandomRecordingNotesHit() {
	for (int i = 0; i < numMovementPts; i++) {
		for(int j = 0; j < CREATURE_NUM_NOTES_PER_SEGMENT; j++) {
			bool bNoteOn = false;
			if (ofRandomuf() > .7) 
				bNoteOn = true;
			movementPts[i].notes[j] = bNoteOn;
		}
	}
}

void RecordCreatureScene::resetNotesHit() {
	for (int i = 0; i < numMovementPts; i++) {
		for(int j = 0; j < CREATURE_NUM_NOTES_PER_SEGMENT; j++) {
			movementPts[i].notes[j] = false;
		}
	}
}

void RecordCreatureScene::setNotesHitForCreature( Creature* $cr ) {
	for (int i = 0; i < numMovementPts; i++) {
		for(int j = 0; j < CREATURE_NUM_NOTES_PER_SEGMENT; j++) {
			$cr->movementPts[i].notes[j] = movementPts[i].notes[j];
		}
	}
}

// used for sending to SC through OSC, make sure to set notes in scene, those will be used, not in creature //
void RecordCreatureScene::buildCreature( unsigned int $creatureID, int $bpmIndex ) {
	int totes = numMovementPts * CREATURE_NUM_NOTES_PER_SEGMENT;
	int notesArray[ totes ];
	int tempzors;
	for (int i = 0; i < numMovementPts; i++) {
		for(int j = 0; j < CREATURE_NUM_NOTES_PER_SEGMENT; j++) {
			//$cr->movementPts[i].notes[j] = movementPts[i].notes[j];
			//notesArray[j * CREATURE_NUM_NOTES_PER_SEGMENT + i] = movementPts[i].notes[j];
			tempzors = 0;
			if (movementPts[i].notes[j] == true) tempzors = 1;
			
			notesArray[j * numMovementPts + i] = tempzors;
		}
	}
	#ifdef USING_OSC
		osc->buildCreature( $creatureID, notesArray, totes );
		osc->playSequence( $creatureID, $bpmIndex );
		osc->rampUp( $creatureID );
	#endif
}


// RECORDING FUNCTIONS ///////////////////////////////////////////////////
void RecordCreatureScene::setRandomPointsForCreature(Creature* $cr, ofxVec3f $center, float $targetRadius, float $radiusScalar, float $maxRandom) {
	setRandomRecordingSegmentLocations($targetRadius, $maxRandom);
	setMovementPtsRadius( $center, $targetRadius, $radiusScalar);
	setPointsForCreature( $cr );
}

void RecordCreatureScene::setMovementPtsRadius(ofxVec3f $center, float $targetRadius, float $radiusScalar) {
	float maxMovementPtRadius = panel->getValueF("F_MAX_MOVEMENT_PT_RADIUS");
	
	for (int i = 0; i < numMovementPts; i++) {
		for (int j = 0; j < movementPts[i].numPts; j++) {
			ofxVec2f pos;
			pos.set(movementPts[i].bodyPts[j].loc.x - $center.x, movementPts[i].bodyPts[j].loc.y - $center.y);
			
			float radDiff = (pos.length() - $targetRadius);
			radDiff = radDiff < 1 ? radDiff * -1 : radDiff;
			if (radDiff < 0) radDiff = 0.f;
			radDiff *= $radiusScalar;
			if (radDiff > maxMovementPtRadius) radDiff = maxMovementPtRadius;
			movementPts[i].bodyPts[j].radius = radDiff;
			//cout << "this is the pos length " << pos.length() << "this is the $tarRad = " << $targetRadius << " the diff = " << radDiff << endl;
		}
	}
}

void RecordCreatureScene::setPointsForCreature( Creature* $cr ) {
	for (int i = 0; i < numMovementPts; i++) {
		for (int j = 0; j < movementPts[i].numPts; j++) {
			$cr->movementPts[i].bodyPts[j].loc.x = movementPts[i].bodyPts[j].loc.x;
			$cr->movementPts[i].bodyPts[j].loc.y = movementPts[i].bodyPts[j].loc.y;
			$cr->movementPts[i].bodyPts[j].loc.z = movementPts[i].bodyPts[j].loc.z;
			$cr->movementPts[i].bodyPts[j].radius = movementPts[i].bodyPts[j].radius;
			if ( i == 4 || i == 6) {
				//cout << "Segment: " << i << ", bodyPt: " << j << ", radius: " << movementPts[i].bodyPts[j].radius << endl;
			}
		}
	}
}

void RecordCreatureScene::addRadiusForNotesHit( Creature* $cr, float $noteHitAdd ) {
	float segNotesBodyPtsRatio = (float)CREATURE_NUM_NOTES_PER_SEGMENT / (float)CREATURE_NUM_BODY_PTS_PER_SEGMENT; // mostly .5 //
	for (int i = 0; i < $cr->numMovementPts; i++) {
		for (int j = 0; j < $cr->movementPts[i].numPts; j++) {
			int noteIndex = ceil((float)j * segNotesBodyPtsRatio);
			// increase radius if corresponding note is active //
			if ($cr->movementPts[i].notes[noteIndex] == true) {
				//cout << "The note " << noteIndex << " has been hit, adding " << $noteHitAdd << " to the radius at " << j << endl;
				$cr->movementPts[i].bodyPts[j].radius += $noteHitAdd;
			}
		}
	}
}

void RecordCreatureScene::setUpForRecording(float $x, float $y, float $radius) {
	resetNotesHit();
	for (int i = 0; i < numMovementPts; i++) {
		setCurrentRecordingSegmentLocation(0, $x, $y, false);
		movementPts[i].reset( $radius );
	}
}

void RecordCreatureScene::setCurrentRecordingSegmentLocation(int $segmentIndex, float $x, float $y, bool $tween) {
	//segmentIndex = $segmentIndex;
	int currNum = numMovementPts - $segmentIndex;
	string numString = ofToString(currNum);
	if (currNum < 10) numString = "0"+numString;
	
	float tarZ = 0.f;
	countDown_txt->setText(numString);
	for (int i = 0; i < numMovementPts; i++) {
		//movementPts[i].loc.set($x, $y, (i - $segmentIndex) * 300);
		movementPts[i].loc.set($x, $y, movementPts[i].loc.z);
		tarZ = (i - $segmentIndex) * 300;
		if ($tween) Tweenzor::add( &movementPts[i].loc.z, movementPts[i].loc.z, tarZ, 0.f, .3f, EASE_IN_OUT_QUART);
		else movementPts[i].loc.z = tarZ;
		//for (int j = 0; j < movementPts[i].numPts; j++) {
			//movementPts[i].bodyPts[j].loc.z = movementPts[i].loc.z;
			//int currIndex = j * 4;
			//movementPts[i].drawingPts[currIndex    ].z = movementPts[i].loc.z;
			//movementPts[i].drawingPts[currIndex + 1].z = movementPts[i].loc.z;
			//movementPts[i].drawingPts[currIndex + 2].z = movementPts[i].loc.z;
			//movementPts[i].drawingPts[currIndex + 3].z = movementPts[i].loc.z;
		//}
	}
}

void RecordCreatureScene::updateSegmentPoints(int $segmentIndex, float $x, float $y, float $z) {
	for (int i = 0; i < numMovementPts; i++) {
		for (int j = 0; j < movementPts[i].numPts; j++) {
			movementPts[i].bodyPts[j].loc.z = movementPts[i].loc.z;
			int currIndex = j * 4;
			movementPts[i].drawingPts[currIndex    ].z = movementPts[i].loc.z;
			movementPts[i].drawingPts[currIndex + 1].z = movementPts[i].loc.z;
			movementPts[i].drawingPts[currIndex + 2].z = movementPts[i].loc.z;
			movementPts[i].drawingPts[currIndex + 3].z = movementPts[i].loc.z;
		}
	}
}

void RecordCreatureScene::setRandomRecordingSegmentLocations(float $radius, float $maxRandom) {
	for (int i = 0; i < numMovementPts; i++) {
		movementPts[i].loc.set(center.x, center.y, 0);
		movementPts[i].resetRandom($radius, $maxRandom);
	}
}



void RecordCreatureScene::advanceSegment( ) {
	segmentIndex += 1;
	cSegmentManager->visible = false;
	// add(float* $property, float $begin, float $end, int $delay, int $duration, int $easeType
	Tweenzor::add( &cSegmentManager_f->center.z, 0, -400.f, 0.f, 0.3f, EASE_IN_OUT_QUART);
	Tweenzor::add( &cSegmentManager_f->alpha, 255, 0, 0.f, 0.3f, EASE_IN_OUT_QUART);
	Tweenzor::add( &cSegmentManager_b->center.z, 300, 0.f, 0.f, 0.3f, EASE_IN_OUT_QUART);
	Tweenzor::add( &cSegmentManager_b->alpha, 0, 255.f, 0.f, 0.3f, EASE_IN_OUT_QUART);
	Tweenzor::getRecentTween()->addListener( Tween::COMPLETE, this, &RecordCreatureScene::onSegmentsAnimComplete);
	cSegmentManager_f->visible = true;
	cSegmentManager_b->visible = true;
	
	if (segmentIndex > numMovementPts - 1) {
		bRecordingComplete = true; // checked by testApp to see if it should be saved //
		bRecording = false;
		
	}
}

void RecordCreatureScene::onSegmentsAnimComplete(float args) {
	cSegmentManager_f->visible = false;
	cSegmentManager_b->visible = false;
	cSegmentManager->visible = true;
}

void RecordCreatureScene::addForceToMovementPtAlongAxis(int $movementPtIndex, int $bodyPtIndex, ofxVec2f $frc) {
	ofxVec2f frc, centerNormal, targetLoc, currLoc;
	frc.set($frc.x, $frc.y);
	// prevLoc stores the target loc, where it should be on the circle, set in reset in RecordSegment //
	targetLoc.set(movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].prevLoc.x, movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].prevLoc.y);
	currLoc.set(movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].loc.x, movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].loc.y);
	
	centerNormal = targetLoc - ofxVec2f(center.x, center.y);
	centerNormal.normalize();
	
	frc = frc.dot( centerNormal );
	frc = centerNormal * frc.length();
	
	movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].addForce(frc.x, frc.y, 0.f);
}

void RecordCreatureScene::addForceToMovementPtAlongCenterNormal(int $movementPtIndex, int $bodyPtIndex, float $strength) {
	ofxVec2f centerNormal, targetLoc;
	targetLoc.set(movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].prevLoc.x, movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].prevLoc.y);
	centerNormal = targetLoc - ofxVec2f(center.x, center.y);
	centerNormal.normalize();
	centerNormal *= $strength;
	
	movementPts[$movementPtIndex].bodyPts[$bodyPtIndex].addForce(centerNormal.x, centerNormal.y, 0.f);
}

void RecordCreatureScene::update() {
	//if (status == CREATURE_TYPE_RECTANGLES) cout << "RecordCreatureScene :: update : type is rectangles" << endl;
	
	creature->setCreatureAlongVector( creatureCenter, ofxVec3f(-1, 0, 0) );
	creature->setLifeAtts( 1.f, 10000, 1000, .01f );
	creature->update();
	
	
	if (bRecording && segmentIndex > -1 && segmentIndex < numMovementPts) {
		float VFStrength		= panel->getValueF("VF_MULTIPLIER");
		float centerRadius		= panel->getValueF("CREATURE_RECORDING_RESET_RADIUS");
		
		grayDiffSmall.scaleIntoMe( *motionCVGrayscaleImage );
		grayDiffSmall.blur(5); // really blur the image alot!
		VF->setFromPixels(grayDiffSmall.getPixels(), false, VFStrength);
		//ofxVec2f frc, centerNormal, targetLoc, currLoc;
		ofxVec2f frc;
		for (int i = 0; i < movementPts[segmentIndex].numPts; i++) {
			frc = VF->getForceFromPos(movementPts[segmentIndex].bodyPts[i].loc.x, movementPts[segmentIndex].bodyPts[i].loc.y);
			
			addForceToMovementPtAlongAxis( segmentIndex, i, frc);
			movementPts[segmentIndex].bodyPts[i].addForce(center.x, center.y, 0.f, centerRadius, -110.f);
		}
	}
	///
	
	unsigned int elapsedTime = ofGetElapsedTimeMillis() - startTime;
	if (bRecording == true) {
		if( elapsedTime > 0 ) {
			// set the percentages for the amount of movement //
			motion_totalWhitePixPoss += video_width * video_height;
			float checkW = video_width / (float)(numSpinePts+2.f);
			for (int i = 0; i < numSpinePts; i++) {
				hitZones[i] += motionCVGrayscaleImage->countNonZeroInRegion((float)(i + 1.f) * checkW, 0, checkW, video_height);
			}
			
			calculateMotionPercentages();
			//setBodyRadiusFromMotion( float $motionPct, float $midPct, float $minRadius, float $maxRadius );
			creature->setBodyRadiusFromMotion( 1 - motionHitPct, motionPosPct, panel->getValueF("F_SPINE_MIN_RAD"), panel->getValueF("F_SPINE_MAX_RAD") );
			
			bool bShouldAdvSegment = false;
			
			#ifdef USING_OSC
				if(elapsedTime > 1000) {
					if (osc->bHasMetronome) {
						bShouldAdvSegment = true;
						osc->bHasMetronome = false;
					}
				} else {
					osc->bHasMetronome = false;
				}
			#else
				//if(elapsedTime > 2300) {
			if (elapsedTime > 1000) {
					bShouldAdvSegment = true;
				}
			#endif
			
			if(bShouldAdvSegment) {
				setMovementPtsRadius( center, panel->getValueF("CREATURE_RECORDING_RESET_RADIUS"), panel->getValueF("CREATURE_RAD_DIFF_MULT"));
				setPointsForCreature( creature );
				if (status == CREATURE_TYPE_SLINKY) {
					creature->bFirstRun = true;
				}
				advanceSegment( );
				if (bRecording) {
					creature->setActiveSegment( segmentIndex );
					setCurrentRecordingSegmentLocation( segmentIndex, center.x, center.y, true );
					// only restart the start time if not sending osc //
					#ifndef USING_OSC 
						startTime = ofGetElapsedTimeMillis();
					#endif
					cout << "segment index is " << segmentIndex << endl;
				}
				
			}
			
		}
	}
	
	// check for the hit segments //
	
	float noteHitMult = panel->getValueF("F_NOTE_HIT_MULT");
	
	elapsedTime = ofGetElapsedTimeMillis();
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		//cSegmentManager->segments[i].bIsActive = false;
		cSegmentManager->segments[i].hitPct -= .02;
		if (cSegmentManager->segments[i].hitPct < 0.f) cSegmentManager->segments[i].hitPct = 0.f;
		if (bRecording && segmentIndex > -1 && segmentIndex < numMovementPts) {
			addForceToMovementPtAlongCenterNormal(segmentIndex, (i), cSegmentManager->segments[i].hitPct * noteHitMult);
			//addForceToMovementPtAlongCenterNormal(segmentIndex, (i * 2) + 1, cSegmentManager->segments[i].hitPct * 2.f);
		}
		
		if (cSegmentManager->segments[i].bIsActive) { // check to make sure the button is active and accepting input //
			int numWhite = motionCVGrayscaleImage->countNonZeroInRegion((cSegmentManager->center.x + cSegmentManager->segments[i].rect.x) * videoMinRatio,
																		(cSegmentManager->center.y + cSegmentManager->segments[i].rect.y) * videoMinRatio,
																		cSegmentManager->segments[i].rect.width * videoMinRatio,
																		cSegmentManager->segments[i].rect.height * videoMinRatio );
			if (numWhite > 400) {
				if (elapsedTime - cSegmentManager->segments[i].lastTimeTriggered > 700) {
					//cSegmentManager->segments[i].bIsActive = true;
					cSegmentManager->segments[i].hitPct = 1.f;
					
					if (bRecording == true) {
						// set the notes for playback laters //
						movementPts[segmentIndex].setNoteActivated(i);
					}
					#ifdef USING_OSC
						// OSC send the freq to SuperCollider //
						osc->sendNoteHit( currentCreatureInt, segmentIndex, i);
					#endif
					cSegmentManager->segments[i].lastTimeTriggered = elapsedTime;
				}
			}
		}
	}
	
	////////////////////////////////
	
	for (int i = 0; i < numMovementPts; i++) {
		updateSegmentPoints(i, movementPts[i].loc.x, movementPts[i].loc.y, movementPts[i].loc.z);
		for (int j = 0; j < movementPts[i].numPts; j++) {
			movementPts[i].bodyPts[j].addDamping( movementPts[i].bodyPts[j].DAMPING );
			movementPts[i].bodyPts[j].vel += movementPts[i].bodyPts[j].acc;
			movementPts[i].bodyPts[j].loc += movementPts[i].bodyPts[j].vel;
			
			movementPts[i].bodyPts[j].acc.set(0, 0, 0);
		}
	}
	
	
	if (bRecording && segmentIndex > -1 && segmentIndex < numMovementPts) {
		
		// 4 drawing points per movementPt //
		ofxVec2f currLoc, tempVec, targetLoc;
		for (int i = 0; i < movementPts[segmentIndex].numPts; i++) {
			int currIndex = i * 4;
			currLoc.set(movementPts[segmentIndex].bodyPts[i].loc.x, movementPts[segmentIndex].bodyPts[i].loc.y);
			// there are four drawing points per bodyPt, so the first and last drawingPt don't move, they stay on the circle //
			if (status == CREATURE_TYPE_RECTANGLES) {
				//ofxVec2f tempVec = movementPts[segmentIndex].drawingPts[currIndex] - movementPts[segmentIndex].drawingPts[currIndex + 3];
				tempVec = movementPts[segmentIndex].drawingPts[currIndex] - currLoc;
				float dist = tempVec.length() * .5;
				//float dist = 20.f;
				tempVec = ofxVec2f(center.x, center.y) - currLoc;
				tempVec.normalize();
				tempVec.rotate( -90.f );
				tempVec *= dist;
				movementPts[segmentIndex].drawingPts[currIndex+2].set(tempVec.x + currLoc.x, tempVec.y + currLoc.y, 0);
				tempVec *= -1;
				movementPts[segmentIndex].drawingPts[currIndex+1].set(tempVec.x + currLoc.x, tempVec.y + currLoc.y, 0);
			} else if (status == CREATURE_TYPE_SPIKEY) {
				movementPts[segmentIndex].drawingPts[currIndex+1].set(currLoc.x, currLoc.y, 0);
				movementPts[segmentIndex].drawingPts[currIndex+2].set(currLoc.x, currLoc.y, 0);
			} else if (status == CREATURE_TYPE_SLINKY) {
				//tempVec = movementPts[segmentIndex].drawingPts[currIndex] - currLoc;
				//float dist = tempVec.length() * .5;
				targetLoc.set(movementPts[segmentIndex].bodyPts[i].prevLoc.x, movementPts[segmentIndex].bodyPts[i].prevLoc.y);
				targetLoc = targetLoc - currLoc;
				targetLoc *= .5;
				float dist = 20.f;
				tempVec = ofxVec2f(center.x, center.y) - currLoc;
				tempVec.normalize();
				tempVec.rotate( -90.f );
				//tempVec *= (dist + ofRandom(-3, 3) );
				tempVec *= dist;
				movementPts[segmentIndex].drawingPts[currIndex+2].set(tempVec.x + currLoc.x + targetLoc.x, tempVec.y + currLoc.y + targetLoc.y, 0);
				tempVec.normalize();
				//tempVec *= -(dist + ofRandom(-3, 3) );
				tempVec *= -dist;
				movementPts[segmentIndex].drawingPts[currIndex+1].set(tempVec.x + currLoc.x + targetLoc.x, tempVec.y + currLoc.y + targetLoc.y, 0);
			}
		}
	}
}

void RecordCreatureScene::draw() {
	//ofSetColor(0, 0, 0, 150);
	//ofRect(10, 10, 240, 190);
	//ofNoFill();
	//ofSetColor(255, 255, 255, 150);
	//ofFill();
	//ofRect(8, 8, 244, 194);
	if (bDrawCreature) creature->draw();
	ofColor highColor = ColorPalette::getWhiteHighlightColor();
	ofColor lowColor = ColorPalette::getCurrentColorForCreature( status );
	
	if (bAnimInComplete) {
		if(!bTransitioning ) {
			for (int i = 0; i < numMovementPts; i++) {
				if ( i == segmentIndex) {
					float radius = 5;
					/*
					for (int j = 0; j < movementPts[i].numDrawingPts; j++) {
						if (j > 0) {
							if (j % 2 == 1) {
								ofSetColor(150, 150, 150);
								radius = 8;
							} else {
								ofSetColor(255, 100, 255);
								radius = 5;
							}
						}
						ofCircle(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y, radius);
					}
					 */
					ofSetColor(255, 255, 255);
					ofSetLineWidth(3.f);
					
					if (status == CREATURE_TYPE_RECTANGLES || status == CREATURE_TYPE_SPIKEY) {
						for (int j = 0; j < movementPts[i].numDrawingPts; j++) {
							if (j < movementPts[i].numDrawingPts - 1) {
								ofLine(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y, movementPts[i].drawingPts[j+1].x, movementPts[i].drawingPts[j+1].y);
							} else {
								ofLine(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y, movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
							}
						}
					} else if (status == CREATURE_TYPE_SLINKY) {
						ofNoFill();
						ofBeginShape();
						for (int j = 0; j < movementPts[i].numDrawingPts; j++) {
							ofCurveVertex(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y);
							if (j == 1) {
								ofCurveVertex(movementPts[i].bodyPts[0].loc.x, movementPts[i].bodyPts[0].loc.y);
							}
							if (j > 2) {
								if (j % 4 == 1) {
									int mpIndex = floor( (float)j / 4.f );
									ofCurveVertex(movementPts[i].bodyPts[mpIndex].loc.x, movementPts[i].bodyPts[mpIndex].loc.y);
								}
							}
						}
						ofCurveVertex(movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
						ofCurveVertex(movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
						ofEndShape(true);
						ofFill();
					}
					
					
					/*
					ofSetColor(255, 255, 255);
					ofSetLineWidth(3.f);
					for (int j = 0; j < movementPts[i].numPts; j++) {
						if (j < movementPts[i].numPts - 1) {
							ofLine(movementPts[i].bodyPts[j].loc.x, movementPts[i].bodyPts[j].loc.y, movementPts[i].bodyPts[j+1].loc.x, movementPts[i].bodyPts[j+1].loc.y);
						} else {
							ofLine(movementPts[i].bodyPts[j].loc.x, movementPts[i].bodyPts[j].loc.y, movementPts[i].bodyPts[0].loc.x, movementPts[i].bodyPts[0].loc.y);
						}
					}
					*/
					ofSetLineWidth(1.f);
					
					// draw the points that are being hit //
					//ofSetColor(250, 0, 0);
					//for (int j = 0; j < movementPts[i].numPts; j++) {
					//	ofCircle(movementPts[i].bodyPts[j].loc.x, movementPts[i].bodyPts[j].loc.y, 6);
					//}
				} else {
					
					if (movementPts[i].loc.z == 0) {
						ofSetColor(highColor.r, highColor.g, highColor.b, 255);
					} else if (movementPts[i].loc.z > 0) {
						ofSetColor(lowColor.r, lowColor.g, lowColor.b, 100);
					} else {
						ofSetColor(lowColor.r, lowColor.g, lowColor.b, 75);
					}
					
					float lineWidth = ((int)movementPts[i].loc.z * .002);
					lineWidth += 5;
					//lineWidth -= 5;
					if (lineWidth < .5f) lineWidth = .5f;
					if (lineWidth > 10.f) lineWidth = 10.f;
					
					glLineWidth ( lineWidth );
					glPushMatrix();
					glTranslatef(0, 0, movementPts[i].loc.z);
					if (status == CREATURE_TYPE_RECTANGLES || status == CREATURE_TYPE_SPIKEY) {
						for (int j = 0; j < movementPts[i].numDrawingPts; j++) {
							if (j < movementPts[i].numDrawingPts - 1) {
								ofLine(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y, movementPts[i].drawingPts[j+1].x, movementPts[i].drawingPts[j+1].y);
							} else {
								ofLine(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y, movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
							}
						}
					} else if (status == CREATURE_TYPE_SLINKY) {
						ofNoFill();
						ofBeginShape();
						for (int j = 0; j < movementPts[i].numDrawingPts; j++) {
							ofCurveVertex(movementPts[i].drawingPts[j].x, movementPts[i].drawingPts[j].y);
							if (j == 1) {
								ofCurveVertex(movementPts[i].bodyPts[0].loc.x, movementPts[i].bodyPts[0].loc.y);
							}
							if (j > 2) {
								if (j % 4 == 1) {
									int mpIndex = floor( (float)j / 4.f );
									ofCurveVertex(movementPts[i].bodyPts[mpIndex].loc.x, movementPts[i].bodyPts[mpIndex].loc.y);
								}
							}
						}
						ofCurveVertex(movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
						ofCurveVertex(movementPts[i].drawingPts[0].x, movementPts[i].drawingPts[0].y);
						ofEndShape(true);
						ofFill();
					}
					glPopMatrix();
					
					/*
					glBegin( GL_LINES );
					for (int j = 0; j < movementPts[i].numPts; j++) {
						if (j < movementPts[i].numPts - 1) {
							glVertex3f(movementPts[i].bodyPts[j].loc.x, movementPts[i].bodyPts[j].loc.y, movementPts[i].bodyPts[j].loc.z);
							glVertex3f(movementPts[i].bodyPts[j+1].loc.x, movementPts[i].bodyPts[j+1].loc.y, movementPts[i].bodyPts[j+1].loc.z);
						} else {
							glVertex3f(movementPts[i].bodyPts[j].loc.x, movementPts[i].bodyPts[j].loc.y, movementPts[i].bodyPts[j].loc.z);
							glVertex3f(movementPts[i].bodyPts[0].loc.x, movementPts[i].bodyPts[0].loc.y, movementPts[i].bodyPts[0].loc.z);
						}
					}
					glEnd();
					*/
					glLineWidth ( 1.f );
				}
			}
		}
	}
	if (mainMessage_txt->visible) {
		glPushMatrix();
		glTranslatef(mainMessage_txt->x, mainMessage_txt->y, 0);
		mainMessage_txt->draw();
		glPopMatrix();
	}
	if (countDown_txt->visible) {
		glPushMatrix();
		glTranslatef(countDown_txt->x, countDown_txt->y, 0);
		countDown_txt->draw();
		glPopMatrix();
	}
	
	if (cSegmentManager->visible) {
		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glRotatef(cSegRotZ, 0, 0, 1);
			cSegmentManager->draw();
		glPopMatrix();
	}
	if (cSegmentManager_f->visible) {
		glPushMatrix();
		glTranslatef(cSegmentManager_f->center.x, cSegmentManager_f->center.y, cSegmentManager_f->center.z);
			cSegmentManager_f->draw();
		glPopMatrix();
	}
	if (cSegmentManager_b->visible) {
		glPushMatrix();
		glTranslatef(cSegmentManager_b->center.x, cSegmentManager_b->center.y, cSegmentManager_b->center.z);
			cSegmentManager_b->draw();
		glPopMatrix();
	}
}




















