#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxPoint2f.h"
#include "vectorField.h"
#include "Particle2D.h"
#include "ofxBlobTracker.h"
#include "ofxTweenzor.h"

#include "Constants.h"

#include "ofxCamera.h"

#include "Scenes.h"

#include "FontBook.h"
#include "TopMenu.h"

#include "ofxTileSaver.h"

#include "OSCManager.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	// the tracker creates an instance of a video player and grabber, which is clumsy and wasteful
	// but then we only have to set live video once here #define //
	ofxBlobTracker	tracker;
	ofxControlPanel	panel;
	bool bShowPanel;
	
	bool bFullscreen;
	
	vectorField VF;
	
	float video_width;
	float video_height;
	float videoRatio;
	float videoMinRatio;
	
	ofxTileSaver tileSaver; // save out hi res mofos
	
	ofImage bgImage;
	
	ofxVec3f center; // included in Constants.h
	
	bool bSaveScreenImage; // should we save the screen image? //
	
	// camera man //
	// this is managed by the render scene, and reset for all other scenes in testApp,
	// so make sure if you are in render scene, you leave this cat alone //
	ofxCamera cam;
	
	ofImage circleBtnImage;
	
	ofxCvGrayscaleImage motionCVGrayscaleImage;
	ofxCvGrayscaleImage	grayDiffSmall;
	
	TopMenu* topMenu;
	
	bool bMousePressed;
	
	bool bSwitchScenes; // we need to switch, initiates the transition of scenes
	bool bTransitioning; // set to true on bSwitchScenes and false when scenes have been switched 
	
	// all of the scenes //
	SelectCreatureScene*	selectScene;
	RecordCreatureScene*	recordScene;
	ClaimCreatureScene*		claimScene;
	RenderCreaturesScene*	renderScene;
	
	int status;
	
	// Function Declarations //
	void addRandomCreature( int $creatureType );
	
#ifdef USING_OSC
	OSCManager osc;
#else
	unsigned int beatTimer;
#endif
	

};

#endif
