#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	//ofSetVerticalSync(true); //
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetWindowTitle("social sqncr");
	Tweenzor::init();
	
	#ifdef USING_OSC
		cout << "yes we are using osc" << endl;
		//osc.setup( "169.254.156.174", 12000, 12009);
		osc.setup("127.0.0.1", 57120, 12009);
		//osc.setup("192.168.1.2", 57120, 12009);
	#else
		beatTimer = ofGetElapsedTimeMillis();
	#endif
	
	video_width = 320;
	video_height = 240;
	
	videoRatio = (float) ofGetWidth() / (float) video_width;
	videoMinRatio = (float) video_width / (float) ofGetWidth();
	
	VF.setupField(60, 45, ofGetWidth(), ofGetHeight());
	
	circleBtnImage.loadImage("circleBtnOutline.png");
	
	bShowPanel = false;
	panel.setup("Settings", ofGetWidth() - 620, 20, 600, 748);
	
	tracker.setup(&panel, video_width, video_height);
	
	motionCVGrayscaleImage.allocate( video_width, video_height);
	grayDiffSmall.allocate(60, 45);
	
	FontBook::addFont("HelveticaNeueMed.ttf", 16, 16);
	FontBook::addFont("HelveticaNeueMed.ttf", 24, 24);
	FontBook::addFont("HelveticaNeueMed.ttf", 120, 120);
	
	center.set(ofGetWidth() * .5, ofGetHeight() * .5, 0.f);
	
	bMousePressed		= false;
	bFullscreen			= false;
	
	//bSaveScreenImage	= false;
	//tileSaver.init( 7, 0, true );
	tileSaver.init( 7, 0, true );
	
	bgImage.loadImage("bgClouds.jpg");
	bgImage.setImageType(OF_IMAGE_GRAYSCALE);
	
	// Panel Configuration locatied in setUpPanel Funcs in scenes ////////////////////////////////////////////
	
	// create the scenes //
	// must set up before the panel loadSettings function is called //
	
	recordScene = new RecordCreatureScene();
	recordScene->setUpPanel( &panel );
	recordScene->motionCVGrayscaleImage = &motionCVGrayscaleImage;
	recordScene->VF = &VF;
	#ifdef USING_OSC
		recordScene->osc = &osc;
	#endif
	
	selectScene = new SelectCreatureScene();
	selectScene->setUpPanel(&panel);
	selectScene->motionCVGrayscaleImage = &motionCVGrayscaleImage;
	
	claimScene = new ClaimCreatureScene();
	claimScene->setUpPanel( &panel );
	claimScene->motionCVGrayscaleImage = &motionCVGrayscaleImage;
	
	renderScene = new RenderCreaturesScene();
	renderScene->setUpPanel( &panel );
	#ifdef USING_OSC
		renderScene->osc = &osc;
	#endif
	renderScene->cam = &cam;
	renderScene->setVideoAtts( video_width, video_height, videoMinRatio, videoRatio );
	renderScene->setMotionCVImage( &motionCVGrayscaleImage ); // sends it over to the ParticleManager to check for particle interaction //
	renderScene->VF = &VF;
	
	// TO DO:
	// move osc into a static class, so it doesn't have to be passed around everywhere //
	
	////////////////////////////////////////////////////////////
	
	//Load Panel Settings /////////////////////
	panel.loadSettings("cvSettings.xml");
	
	// allow the panel to load the settings so that the scenes can use the saved settings ///////////////////////
	recordScene->initiate(panel.getValueI("I_NUM_CREATURE_SEGS"), CREATURE_NUM_BODY_PTS_PER_SEGMENT,
						  panel.getValueF("F_SPINE_MAX_RAD"), center, videoMinRatio);
	recordScene->videoRatio = videoRatio;
	recordScene->video_width = video_width;
	recordScene->video_height = video_height;
	
	selectScene->recordScene = recordScene;
	selectScene->tracker = &tracker;
	selectScene->videoMinRatio = videoMinRatio;
	selectScene->videoRatio = videoRatio;
	selectScene->circleBtnImage = &circleBtnImage;
#ifdef USING_OSC
	selectScene->osc = &osc;
#endif
	selectScene->initiate();
	
	claimScene->initiate( center, videoMinRatio );
	claimScene->circleBtnImage = &circleBtnImage;
#ifdef USING_OSC
	claimScene->osc = &osc;
#endif
	
	renderScene->initiate();
	// GUI ELEMENTS ///////////////////////////////////////////
	// this is now only used in the render all creatures scene //
	topMenu = new TopMenu();
	topMenu->initiate();
	topMenu->motionCVGrayscaleImage = &motionCVGrayscaleImage;
	topMenu->videoMinRatio = videoMinRatio;
	topMenu->videoRatio = videoRatio;
	topMenu->tracker = &tracker;
	topMenu->circleBtnImage = &circleBtnImage;
#ifdef USING_OSC
	topMenu->osc = &osc;
#endif
	
	topMenu->fontSize = 16;
	topMenu->currScene = SCENE_RENDER_ALL_CREATURES;
	topMenu->animateButtonIn();
	
	// Initial Scene Settings //////////////////////////////////////////
	status = SCENE_RENDER_ALL_CREATURES;
	bSwitchScenes = false;
	bTransitioning = false;
	
	///////////////////////////////////////////////////////////
	
	//bSwitchScenes = true; // just for testing next scene //
	/*
	claimScene->animateIn(creatures[totalCreatures - 1]);
	
	topMenu->currScene = SCENE_ENTER_NAME;
	topMenu->animateButtonIn();
	
	status = SCENE_ENTER_NAME;
	*/
}

//--------------------------------------------------------------
void testApp::update() {
	if( tileSaver.bGoTiling ) {
		cout << "saving file: " << tileSaver.currentRow << endl;
		return;
	}
	Tweenzor::update();
	tracker.update();
	motionCVGrayscaleImage = tracker.getCvMotionHistoryImage();
	
	if(bShowPanel) panel.update();
	
	#ifdef USING_OSC
		osc.update();
	#endif
	
	if (status == SCENE_RENDER_ALL_CREATURES) {
		topMenu->bNewVideoFrame = tracker.bIsFrameNew;
		//topMenu->update();
		if (topMenu->bButtonActivated) {
			topMenu->bButtonActivated = false;
			bSwitchScenes = true;
			bTransitioning = true;
		}
		// integrate this code with the render creature scene, work with Bruce on this ////
		#ifdef USING_OSC
			if (osc.bBeatReceived) {
				osc.bBeatReceived = false;
			}
			if (osc.bHasAdvMessages) {
				for(int i = 0; i < osc.segmentAdvMessages.size(); i++) {
					//cout << "testApp :: osc has adv Messages : creatureID: " << osc.segmentAdvMessages[i].creatureID << ", segmentIndex: " << osc.segmentAdvMessages[i].segmentIndex << endl;
					renderScene->advanceCreatureSegment( osc.segmentAdvMessages[i].creatureIndex, osc.segmentAdvMessages[i].segmentIndex );
				}
				osc.clearAdvMessages(); // also sets osc.bHasAdvMessages to false //
			}
		#endif
		
		#ifndef USING_OSC
		if(ofGetElapsedTimeMillis() - beatTimer > 1000) {
			renderScene->advanceCreatureSegments();
			//renderScene->pManager.onBeat( ofRandom(0, 1) );
			beatTimer = ofGetElapsedTimeMillis();
		}	
		#endif
		
		if (bMousePressed) {
			renderScene->mouseX = mouseX;
			renderScene->mouseY = mouseY;
		}
		
		renderScene->bMousePressed = bMousePressed;
		renderScene->update();
		
		// right now we don't have a scene for this guy //
		if (bSwitchScenes) {
			#ifdef USING_OSC
				osc.ampDownAllCreatures(); // turn off the creatures for the interactive elements //
			#endif
			bTransitioning = false;
			bSwitchScenes = false;
			topMenu->currScene = SCENE_SELECT_CREATURE;
			topMenu->animateButtonIn();
			selectScene->reset();
			selectScene->animateIn();
			status = SCENE_SELECT_CREATURE;
		}
	} else if (status == SCENE_SELECT_CREATURE) {
		#ifndef USING_OSC
		if(ofGetElapsedTimeMillis() - beatTimer > 1000) {
			selectScene->advanceSegments();
			beatTimer = ofGetElapsedTimeMillis();
		}
		#endif
		
		if (selectScene->bButtonActivated) {
			bSwitchScenes = true;
			bTransitioning = true;
			selectScene->bButtonActivated = false;
		}
		selectScene->update();
		
		if (bSwitchScenes) {
			bTransitioning = true;
			bSwitchScenes = false;
			//selectScene->selectedCreatureIndex = topMenu->menuItemSelected;
			selectScene->animateOut();
			topMenu->animateButtonOut();
		}
		if (bTransitioning) {
			if (selectScene->bAnimOutComplete) {
				status = SCENE_RECORDING_CREATURE;
				cout << "testApp :: the transition from Select Creature to Record creature is complete" << endl;
				bTransitioning = false;
				int currentCreatureType = CREATURE_TYPE_SLINKY;
				// add creature here so that the ID can be used for the osc //////////////////
				if (selectScene->selectedCreatureIndex == CREATURE_TYPE_RECTANGLES - 1) {
					renderScene->addCreature( CREATURE_TYPE_RECTANGLES );
					currentCreatureType = CREATURE_TYPE_RECTANGLES;
				} else if (selectScene->selectedCreatureIndex == CREATURE_TYPE_SPIKEY - 1) {
					renderScene->addCreature( CREATURE_TYPE_SPIKEY );
					currentCreatureType = CREATURE_TYPE_SPIKEY;
				} else {
					renderScene->addCreature( CREATURE_TYPE_SLINKY );
					currentCreatureType = CREATURE_TYPE_SLINKY;
				}
				// is this correct?
				// recordScene->currentCreatureInt = renderScene->creatures[renderScene->totalCreatures - 1]->ID;
				recordScene->currentCreatureInt = renderScene->totalCreatures - 1;
				recordScene->status = currentCreatureType;
				recordScene->animateIn();
			}
		}
	} else if (status == SCENE_RECORDING_CREATURE) {
		//recordScene->bNewVideoFrame = tracker.bIsFrameNew;
		// the starting of the metronome is performed after the recordScene has completely animated in,
		// within the recordScene
		// the OSC - Metronome functionality is located in the recordScene->update function //
		recordScene->update();
		
		if (recordScene->bRecordingComplete) {
			cout << "Should call the function from the creature to set the points for entering name" << endl;
			recordScene->bRecordingComplete = false;
			bSwitchScenes = true;
		}
		
		if (bSwitchScenes) {
			bTransitioning = true;
			bSwitchScenes = false;
			recordScene->animateOut();
		}
		if (bTransitioning) {
			if (recordScene->bAnimOutComplete) {
				cout << "testApp :: the transition from Record Creature to render all creatures is complete" << endl;
				bTransitioning = false;
				recordScene->setMovementPtsRadius( center, panel.getValueF("CREATURE_RECORDING_RESET_RADIUS"), panel.getValueF("CREATURE_RAD_DIFF_MULT"));
				
				recordScene->setPointsForCreature( renderScene->creatures[renderScene->totalCreatures - 1] );
				recordScene->setNotesHitForCreature( renderScene->creatures[renderScene->totalCreatures - 1] );
				// now inside recordeScene, when segment hit, acc added
				//recordScene->addRadiusForNotesHit( renderScene->creatures[renderScene->totalCreatures - 1], panel.getValueF("F_ADD_RADIUS_FOR_NOTE_HIT") );
				//recordScene->buildCreature( renderScene->creatures[renderScene->totalCreatures - 1]->ID ); // send the notes to SC from the recordScene notes saved
				#ifdef USING_OSC
					//osc.creatureAmp( renderScene->totalCreatures - 1, 0 );
				#endif
				
				renderScene->creatures[renderScene->totalCreatures - 1]->setBodyRadiusFromMotion( 1 - recordScene->motionHitPct, recordScene->motionPosPct, 
																								 panel.getValueF("F_SPINE_MIN_RAD"), panel.getValueF("F_SPINE_MAX_RAD") );
				renderScene->creatures[renderScene->totalCreatures - 1]->setMovementAtts( recordScene->motionHitPct );
				renderScene->creatures[renderScene->totalCreatures - 1]->setLifeAtts( recordScene->motionHitPct, panel.getValueI("I_AVG_LIFE_SPAN")*1000, 
																					  panel.getValueI("I_LIFE_SPAN_DIFF")*1000, panel.getValueF("F_DEATH_TIME_START_PCT") );
				
				// recordScene->motionHitPct from panel.getValueF("F_MIN_MOTION_RECORD_PCT") to 1.f. must pass in 1 - 10
				//int bpmIndex = ofMap(recordScene->motionHitPct, panel.getValueF("F_MIN_MOTION_RECORD_PCT"), 1.f, 4, 10);
				int bpmIndex = ofMap(recordScene->motionHitPct, 0.f, 1.f, 4, 10);
				recordScene->buildCreature( renderScene->totalCreatures - 1, bpmIndex ); // send the notes to SC from the recordScene notes saved
				
				//renderScene->creatures[renderScene->totalCreatures - 1]->setCreatureAlongVector( renderScene->avgCreatureLoc, ofxVec3f(0, 0, 1) ); // put this creature where avg loc is
				ofxVec3f tempCenter(renderScene->avgCreatureLoc.x, renderScene->avgCreatureLoc.y, renderScene->avgCreatureLoc.z);
				tempCenter.x += ofRandom(-200, 200);
				tempCenter.y += ofRandom(-200, 200);
				tempCenter.z += ofRandom(-100, 100);
				renderScene->creatures[renderScene->totalCreatures - 1]->setCreatureAlongVector( tempCenter, ofxVec3f(0, 0, 1) );
				
				claimScene->animateIn( renderScene->creatures[renderScene->totalCreatures - 1] );
				#ifdef USING_OSC
					//osc.creatureAmp( renderScene->totalCreatures - 1, 1.f );
				#endif
				
				topMenu->currScene = SCENE_ENTER_NAME;
				topMenu->animateButtonIn();
				
				status = SCENE_ENTER_NAME;
			}
		}
		
	} else if (status == SCENE_ENTER_NAME) {
		
		claimScene->bIsFrameNew = tracker.bIsFrameNew;
		if (claimScene->bGimmeLiveVideo) {
			if (tracker.bIsFrameNew) {
				claimScene->setImagesFromPixels(tracker.videoColorCvImage.getPixels(), video_width, video_height);
			}
		}
		if (claimScene->bGrabCreatureImage) {
			renderScene->creatures[renderScene->totalCreatures-1]->setTextureFromPixels( claimScene->smVideoCVImage.getPixels() );
			claimScene->bGrabCreatureImage = false;
		}
		
		#ifdef USING_OSC
			if (osc.bHasAdvMessages) {
				//for(int i = 0; i < osc.segmentAdvMessages.size(); i++) {
					//cout << "testApp :: osc has adv Messages : creatureID: " << osc.segmentAdvMessages[i].creatureID << ", segmentIndex: " << osc.segmentAdvMessages[i].segmentIndex << endl;
					//renderScene->advanceCreatureSegment( osc.segmentAdvMessages[i].creatureIndex, osc.segmentAdvMessages[i].segmentIndex );
				//}
				//osc.clearAdvMessages(); // also sets osc.bHasAdvMessages to false //
				
				for(int i = 0; i < osc.segmentAdvMessages.size(); i++) {
					//cout << "testApp :: osc has adv Messages : creatureID: " << osc.segmentAdvMessages[i].creatureID << ", segmentIndex: " << osc.segmentAdvMessages[i].segmentIndex << endl;
					if (osc.segmentAdvMessages[i].creatureIndex == renderScene->totalCreatures - 1) {
						claimScene->advanceCreatureSegment(  );
					}
				}
				osc.clearAdvMessages(); // also sets osc.bHasAdvMessages to false //
			}
		#else
			if(ofGetElapsedTimeMillis() - beatTimer > 1000) {
				claimScene->advanceCreatureSegment();
				beatTimer = ofGetElapsedTimeMillis();
			}	
		#endif
		
		//cout << "The status of the creature is recording, from the TestApp" << endl;
		if (renderScene->totalCreatures > 0) {
			claimScene->update();
		}
		if (claimScene->bAnimOutComplete) {
			#ifdef USING_OSC
				osc.ampUpAllCreatures();
			#endif
			status = SCENE_RENDER_ALL_CREATURES;
			
			topMenu->currScene = SCENE_RENDER_ALL_CREATURES;
			topMenu->animateButtonIn();
		}
	} 
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	// just to check if it is working //
	//ofSetupScreen();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	ofSetColor(255, 255, 255);
	bgImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	motionCVGrayscaleImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	glDisable(GL_BLEND);
	tileSaver.begin(); // can not have any camera transformations happening, so comment out camera->place in renderScene !!
	if (status == SCENE_RECORDING_CREATURE) {
		ofEnableAlphaBlending();
		glEnable(GL_DEPTH_TEST);
			recordScene->draw();
		glDisable(GL_DEPTH_TEST);
		ofDisableAlphaBlending();
		
	} else if (status == SCENE_ENTER_NAME) {
		//cout << "TestApp :: draw : in the claim scene" << endl;
		ofEnableAlphaBlending();
		//glEnable(GL_DEPTH_TEST); // called inside class
			claimScene->draw();
		//glDisable(GL_DEPTH_TEST);
		ofDisableAlphaBlending();
	} else if (status == SCENE_RENDER_ALL_CREATURES) {
		ofEnableAlphaBlending();
		glEnable(GL_DEPTH_TEST);
			renderScene->draw();
		glDisable(GL_DEPTH_TEST);
		ofDisableAlphaBlending();
	} else if(status == SCENE_SELECT_CREATURE) {
		ofEnableAlphaBlending();
		glEnable(GL_DEPTH_TEST);
			selectScene->draw();
		glDisable(GL_DEPTH_TEST);
		ofDisableAlphaBlending();
	}
	
	tileSaver.end();
	ofSetupScreen();
	if (panel.getValueB("B_SHOW_VECTOR_FIELD")) {
		ofSetColor(0, 130, 130);
		VF.draw();
	}
	
	if (panel.getValueB("B_SHOW_DEBUG_INFO")) {
	
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 80);
		
		if (recordScene->bRecordingComplete) {
			ofSetColor(0, 255, 0);
			ofRect(10, ofGetHeight() - 70, 150, 16);
			ofSetColor(255, 255, 255);
			ofDrawBitmapString("CREATURE COMPLETE!", 14, ofGetHeight() - 57);
		} else if (recordScene->bRecording) {
			ofSetColor(255, 0, 0);
			ofRect(10, ofGetHeight() - 70, 150, 16);
			ofSetColor(255, 255, 255);
			ofDrawBitmapString("RECORDING CREATURE", 14, ofGetHeight() - 57);
		} else {
			ofSetColor(255, 255, 255);
			ofRect(10, ofGetHeight() - 70, 150, 16);
			ofSetColor(0, 0, 0);
			ofDrawBitmapString("WAITING", 14, ofGetHeight() - 57);
		}
		
		ofSetColor(50, 50, 50);
		ofRect( 10, ofGetHeight() - 44, 170, 40);
		ofSetColor(150, 150, 150);
		ofDrawBitmapString("'r' -> new Tangles()\n's' -> new Spiky()\n'l' -> new Slinky()", 14, ofGetHeight() - 32);
		
	}
	ofEnableAlphaBlending();
	topMenu->draw();
	ofDisableAlphaBlending();
	
	if (panel.getValueB("B_DRAW_DIAGNOSTIC")) {
		tracker.draw();
	}
	
	if (panel.getValueB("B_DRAW_CONTOURS")) {
		ofSetColor(255, 0, 0);
		//cout << "ofxBlobTracker :: draw : bDrawContours = " << bDrawContours << endl;
		for (int i = 0; i < tracker.contourFinder.nBlobs; i++) {
			//contourFinder.blobs[i].draw(20, 40 + video_height);
			tracker.contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	}
	ofFill();
	if(bShowPanel) panel.draw();
	
	//ofSetColor(100, 100, 100);
	//ofDrawBitmapString(ofToString(mouseX, 0)+", "+ofToString(mouseY, 0), mouseX + 2, mouseY - 2);
}



//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
		case ' ':
			tracker.learnBg();
			break;
		case 'd':
			tracker.toggleDiagnostic();
			break;
		case 'p':
			bShowPanel = !bShowPanel;
			break;
		case 'f':
			bFullscreen = !bFullscreen;
			
			if(!bFullscreen){
				ofSetFullscreen(false);
			} else if(bFullscreen == 1){
				ofSetFullscreen(true);
			}
			break;
		case 'r':
			addRandomCreature( CREATURE_TYPE_RECTANGLES );
			break;
		case 's':
			addRandomCreature( CREATURE_TYPE_SPIKEY );
			break;
		case 'l':
			addRandomCreature( CREATURE_TYPE_SLINKY );
			break;
		case 'x':
			tracker.listDevices();
			break;
		case OF_KEY_BACKSPACE:
			renderScene->clearAllCreatures();
			break;
		case '0':
			renderScene->deleteCreatureAtIndex( 0 );
			break;
		case '1':
			renderScene->deleteCreatureAtIndex( 1 );
			break;
		case '2':
			renderScene->deleteCreatureAtIndex( 2 );
			break;
		case 'z':
			char str[255];
			sprintf( str, "synthetures_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
			tileSaver.finish( str, false );
			break;
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(bShowPanel) panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(bShowPanel) panel.mousePressed(x,y,button);
	bMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(bShowPanel) panel.mouseReleased();
	bMousePressed = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

void testApp::addRandomCreature( int $creatureType ) {
	if (status == SCENE_RENDER_ALL_CREATURES) {
		renderScene->addCreature( $creatureType );
		
		int cIndex = renderScene->totalCreatures - 1;
		
		recordScene->setRandomPointsForCreature(renderScene->creatures[cIndex], center, panel.getValueF("CREATURE_RECORDING_RESET_RADIUS"),
												panel.getValueF("CREATURE_RAD_DIFF_MULT"), panel.getValueF("CREATURE_RAND_MAX_DIFF"));
		recordScene->setRandomNotesHitForCreature( renderScene->creatures[cIndex] );
		recordScene->addRadiusForNotesHit( renderScene->creatures[cIndex], panel.getValueF("F_ADD_RADIUS_FOR_NOTE_HIT") );
		//recordScene->buildCreature( renderScene->creatures[renderScene->totalCreatures - 1]->ID ); // send to SC from saved notes in recordScene //
		
		float tempMotionPct = ofRandom( 0, 1.f );
		//tempMotionPct = panel.getValueF("F_MIN_MOTION_RECORD_PCT");
		//tempMotionPct = 1.f;
		renderScene->creatures[renderScene->totalCreatures - 1]->setBodyRadiusFromMotion( 1 - tempMotionPct, ofRandom(0, 1.f),
																						 panel.getValueF("F_SPINE_MIN_RAD"), panel.getValueF("F_SPINE_MAX_RAD") );
		renderScene->creatures[renderScene->totalCreatures - 1]->setMovementAtts( tempMotionPct );
		renderScene->creatures[renderScene->totalCreatures - 1]->setLifeAtts( tempMotionPct, panel.getValueI("I_AVG_LIFE_SPAN")*1000,
																			 panel.getValueI("I_LIFE_SPAN_DIFF")*1000, panel.getValueF("F_DEATH_TIME_START_PCT") );
		
		// send atts to OSC to build that mofo //
		// from panel.getValueF("F_MIN_MOTION_RECORD_PCT") to 1.f. must pass in 1 - 10
		//int bpmIndex = ofMap( tempMotionPct, panel.getValueF("F_MIN_MOTION_RECORD_PCT"), 1.f, 4, 10);
		int bpmIndex = ofMap(recordScene->motionHitPct, 0.f, 1.f, 4, 10);
		recordScene->buildCreature( renderScene->totalCreatures - 1, bpmIndex );
		ofxVec3f dirNormal( ofRandomf(), ofRandomf(), ofRandomf() ); // ofRandomf() returns a number between -1 and 1 //
		//ofxVec3f tempCenter(center.x + ofRandom(-center.x * .5, center.x * .5), center.y + ofRandom(-center.y * .5, center.y * .5), ofRandom(0, 300.f) );
		ofxVec3f tempCenter(renderScene->avgCreatureLoc.x, renderScene->avgCreatureLoc.y, renderScene->avgCreatureLoc.z);
		tempCenter.x += ofRandom(-200, 200);
		tempCenter.y += ofRandom(-200, 200);
		tempCenter.z += ofRandom(-100, 100);
		renderScene->creatures[cIndex]->setCreatureAlongVector( tempCenter, dirNormal );
		
		claimScene->setImagesFromPixels(tracker.videoColorCvImage.getPixels(), video_width, video_height);
		renderScene->creatures[cIndex]->setTextureFromPixels( claimScene->smVideoCVImage.getPixels() );
		
	} else {
		cout << "scene is not SCENE_RENDER_ALL_CREATURES !!! <--------" << endl;
	}
}

