/*
 *  ofxBlobTracker.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 12/9/09.
 // Original Code from Zachery Lieberman //
 *
 */

#include "ofxBlobTracker.h"

bool notFountThisFrame(BlobTracker t){
	return !t.bFoundThisFrame;
}

void ofxBlobTracker::setup(ofxControlPanel * $panel, float $w, float $h) {
	LIVE_VIDEO = true;
	videoGrabber.setVerbose(false);
	videoGrabber.initGrabber($w, $h);
	video_width = $w;
	video_height = $h;
	panel = $panel;
	
	setupPanel();
	init();
}

void ofxBlobTracker::setup(ofxControlPanel * $panel, string $loc, bool $autoStart) {
	LIVE_VIDEO = false;
	videoPlayer.loadMovie( $loc );
	if ($autoStart) {
		videoPlayer.play();
		bPaused = false;
	} else {
		bPaused = true;
	}
	video_width = videoPlayer.width;
	video_height = videoPlayer.height;
	panel = $panel;
	
	setupPanel();
	init();
}

void ofxBlobTracker::setup(ofxControlPanel * $panel, ofVideoGrabber& $grabber, float $w, float $h ) {
	LIVE_VIDEO = true;
	
	videoGrabber = $grabber;
	video_width = $w;
	video_height = $h;
	panel = $panel;
	
	setupPanel();
	init();
}

void ofxBlobTracker::setup(ofxControlPanel * $panel, ofVideoPlayer& $player, float $w, float $h) {
	LIVE_VIDEO = false;
	bPaused = false;
	videoPlayer = $player;
	video_width = $w;
	video_height = $h;
	panel = $panel;
	
	setupPanel();
	init();
}

void ofxBlobTracker::init() {
	videoColorCvImage.allocate(video_width, video_height);
	videoGrayScaleCvImage.allocate(video_width, video_height);
	videoBgCvImage.allocate(video_width, video_height);
	videoDiffCvImage.allocate(video_width, video_height);
	videoPrevFrameCvImage.allocate(video_width, video_height);
	videoMotionCvImage.allocate(video_width, video_height);
	videoMotionHistoryCvImage.allocate(video_width, video_height);
	
	videoGrayscaleCvImagePreWarp.allocate(video_width, video_height);
	
	srcPts[0].set( 0, 0 );
	srcPts[1].set( video_width, 0 );
	srcPts[2].set( video_width, video_height );
	srcPts[3].set( 0, video_height );
	
	dstPts[0].set( 0, 0 );
	dstPts[1].set( video_width, 0 );
	dstPts[2].set( video_width, video_height );
	dstPts[3].set( 0, video_height );
	
	trackerIndex = 0;
	nBlobsFound	= 0;
	
	bLearnBg =		false;
	threshold =		80;
	motionThreshold = 100;
	nDilations =	2;
	nErosions =		2;
	maxOperations = 4;
	minBlobSize =	200;
	maxBlobSize =	5000;
	nBlobsTofind =	10;
	
	motionImageDecrement = 20;
	
	bDrawDiagnostic = false;
	bDrawContours	= false;
	bTrackMotion	= false;
	bVideoSettings	= false;
	bDrawShowVideo	= false;
}

void ofxBlobTracker::videoSettings() {
	if (LIVE_VIDEO) {
		videoGrabber.videoSettings();
		//videoGrabber.saveSettings();
	}
}

void ofxBlobTracker::listDevices() {
	if(LIVE_VIDEO) {
		cout << "asking to list the devices" << endl;
		videoGrabber.listDevices();
	}
}

// only for rendered movies //
void ofxBlobTracker::togglePlayPause() {
	if (!LIVE_VIDEO) {
		bPaused = !bPaused;
		videoPlayer.setPaused(bPaused);
		
	}
}

void ofxBlobTracker::toggleDiagnostic() {
	panel->setValueB("B_DRAW_DIAGNOSTIC", !panel->getValueB("B_DRAW_DIAGNOSTIC"));
}


// make sure that init() is called first, before using get functions //
ofxCvContourFinder* ofxBlobTracker::getContourFinder() {
	return &contourFinder;
}

ofxCvGrayscaleImage* ofxBlobTracker::getCvMotionImage() {
	return &videoMotionCvImage;
}

ofxCvGrayscaleImage* ofxBlobTracker::getCvMotionHistoryImagePointer() {
	return &videoMotionHistoryCvImage;
}

ofxCvGrayscaleImage& ofxBlobTracker::getCvMotionHistoryImage() {
	return videoMotionHistoryCvImage;
}

ofxCvColorImage* ofxBlobTracker::getCvColorImage() {
	return &videoColorCvImage;
}

void ofxBlobTracker::update() {
	
	
	//Do CV Stuff
	bIsFrameNew = false;
	if (LIVE_VIDEO) {
		videoGrabber.grabFrame();
		bIsFrameNew = videoGrabber.isFrameNew();
	} else {
		videoPlayer.idleMovie();
		bIsFrameNew = videoPlayer.isFrameNew();
	}
	
	if (bIsFrameNew) {
		//Update from the panel
		updatePanel();
		
		if (LIVE_VIDEO) {
			videoColorCvImage.setFromPixels(videoGrabber.getPixels(), video_width, video_height);
			videoColorCvImage.mirror(false, true);
		} else {
			videoColorCvImage.setFromPixels(videoPlayer.getPixels(), video_width, video_height);
		}
		
		//videoGrayScaleCvImage = videoColorCvImage;
		videoGrayscaleCvImagePreWarp = videoColorCvImage;
		videoGrayScaleCvImage.warpIntoMe( videoGrayscaleCvImagePreWarp, srcPts, dstPts );
		
		if (bLearnBg) {
			videoBgCvImage = videoGrayScaleCvImage;
			panel->setValueB("B_LEARN_BG", false);
			bLearnBg = false;
		}
		if (ofGetElapsedTimef() < 2.5){
			videoBgCvImage = videoGrayScaleCvImage;
		}
		
		if (bTrackBlobs) {
			videoDiffCvImage.absDiff(videoGrayScaleCvImage, videoBgCvImage);
			videoDiffCvImage.threshold(threshold);
			
			for (int i = 0; i < maxOperations; i++){
				
				if (i < nErosions)	videoDiffCvImage.erode();
				if (i < nDilations)	videoDiffCvImage.dilate();
				
			}
		}
		
		if (bTrackBlobs) contourFinder.findContours( videoDiffCvImage, minBlobSize, maxBlobSize, nBlobsTofind, false, true );
		
		//motion history stuff
		if (bTrackMotion) {
			videoMotionCvImage.absDiff(videoGrayScaleCvImage, videoPrevFrameCvImage);
			videoMotionCvImage.threshold( motionThreshold );
			//videoMotionHistoryCvImage.threshold(threshold);
			
			videoMotionHistoryCvImage += videoMotionCvImage;
			videoMotionHistoryCvImage -= motionImageDecrement;
			videoMotionHistoryCvImage.blur();
			
			videoPrevFrameCvImage = videoGrayScaleCvImage;
		}
		
		if (bTrackBlobs) {
			// let's track these blobs !
			// a) assume all trackers are *not* found this frame. 
			for (int i = 0; i < trackerObjects.size(); i++){
				trackerObjects[i].bFoundThisFrame = false;
			}
			
			// b) for all blobs this frame, let's see if we can match them to the trackers.  
			int nBlobsFoundThisFrame = contourFinder.nBlobs;
			nBlobsFound = nBlobsFoundThisFrame;
			
			// assume that none have been found:
			bool bAmIFoundThisFrame[nBlobsFoundThisFrame];
			if (nBlobsFoundThisFrame > 0){
				
				for (int i = 0; i < nBlobsFoundThisFrame; i++){
					bAmIFoundThisFrame[i] = false;
				}
				
				// now, look through every tracker, and see how far away they are from this blob.
				// find the minimum distance, and see if this is reasonable. 
				
				for (int i = 0; i < nBlobsFoundThisFrame; i++){
					
					float	minDistance = 100000;
					int		minIndex	= -1;
					
					for (int j = 0; j < trackerObjects.size(); j++){
						if (trackerObjects[j].bFoundThisFrame == false){
							
							float dx = trackerObjects[j].pos.x - contourFinder.blobs[i].centroid.x;
							float dy = trackerObjects[j].pos.y - contourFinder.blobs[i].centroid.y;
							float distance = sqrt(dx*dx + dy*dy);
							if (distance < minDistance){
								minDistance = distance;
								minIndex = j;
							}
						}
					}
					
					if (minIndex != -1 && minDistance < 100){		// 100 = just a guess.
						trackerObjects[minIndex].pos.x = contourFinder.blobs[i].centroid.x;
						trackerObjects[minIndex].pos.y = contourFinder.blobs[i].centroid.y;
						
						trackerObjects[minIndex].posSmooth.x = 0.99f * trackerObjects[minIndex].posSmooth.x + 
						0.01f * trackerObjects[minIndex].pos.x;
						trackerObjects[minIndex].posSmooth.y = 0.99f * trackerObjects[minIndex].posSmooth.y + 
						0.01f * trackerObjects[minIndex].pos.y;
						
						trackerObjects[minIndex].whoThisFrame = i;
						trackerObjects[minIndex].bFoundThisFrame = true;
						trackerObjects[minIndex].nFramesActive ++;
						bAmIFoundThisFrame[i] = true;	// we got one !
					}
				}
			}
			
			// c) for all non found blobs, add a tracker. 
			
			if (nBlobsFoundThisFrame > 0){
				for (int i = 0; i < nBlobsFoundThisFrame; i++){
					if (bAmIFoundThisFrame[i] == false){
						
						BlobTracker temp;
						temp.pos = contourFinder.blobs[i].centroid;
						temp.posSmooth = temp.pos;
						
						temp.nFramesActive = 0;
						temp.whoThisFrame = i;
						temp.bFoundThisFrame = true;
						temp.id = trackerIndex;
						trackerObjects.push_back(temp);
						trackerIndex ++;
					}
				}
			}
			
			// d) kill all trackers that haven't been found
			// remove_if sorts to the end via a boolean value, 
			// http://en.wikipedia.org/wiki/Erase-remove_idiom
			
			trackerObjects.erase( remove_if(trackerObjects.begin(), trackerObjects.end(), notFountThisFrame), trackerObjects.end() );
		}
		
	}
}

void ofxBlobTracker::draw() {
	ofFill();
	if (bDrawDiagnostic) {
		ofSetColor(255, 255, 255);
		videoGrayscaleCvImagePreWarp.draw(20, 20);
		videoGrayScaleCvImage.draw(40 + video_width, 20);
		videoBgCvImage.draw(20, 40 + video_height);
		videoDiffCvImage.draw(40 + video_width, 40 + video_height);
		if (bTrackMotion) {
			videoMotionCvImage.draw(20, 60 + (video_height * 2));
			videoMotionHistoryCvImage.draw(40 + video_width, 60 + (video_height * 2));
		}
		
		// draw out the warping src points //////////
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(20, 20, 0);
		ofBeginShape();
		ofNoFill();
		for (int i = 0; i < 4; i++){
			ofVertex(srcPts[i].x, srcPts[i].y); 
		}
		ofEndShape(true);
		
		ofFill();
		for (int i = 0; i < 4; i++){
			ofCircle(srcPts[i].x, srcPts[i].y, 4); 
		}
		
		ofPopMatrix();
		//////////////////////////////////////////////
		
		ofSetColor(0, 0, 0);
		ofRect(24, 24, 220, 22);
		ofRect(48 + video_width, 24, 170, 22);
		ofRect(24, 48 + video_height, 170, 22);
		if (bTrackMotion) {
			ofRect(48 + video_width, 48 + video_height, 170, 22);
			ofRect(24, 72 +( video_height * 2), 170, 22);
		}
		
		
		ofSetColor(255, 255, 255);
		ofDrawBitmapString("Video Gray Scale Pre Warp", 35, 40);
		ofDrawBitmapString("Video Image Warpred", 50 + video_width, 40);
		ofDrawBitmapString("Background Image", 35, 62 + video_height);
		ofDrawBitmapString("Video Difference", 50 + video_width, 62 + video_height);
		if (bTrackMotion) {
			ofDrawBitmapString("Motion Detection", 35, 84 + ( video_height * 2));
			ofDrawBitmapString("Motion History", 50 + video_width, 84 + ( video_height * 2) );
		}
	}
	ofSetColor(255, 0, 0);
	//cout << "ofxBlobTracker :: draw : bDrawContours = " << bDrawContours << endl;
	if (bDrawContours) {
		for (int i = 0; i < contourFinder.nBlobs; i++) {
			//contourFinder.blobs[i].draw(20, 40 + video_height);
			contourFinder.draw(20, 40 + video_height, video_width, video_height);
		}
	}
	ofFill();
}

// --------------------------------------------------------------------------
// utility functions ///
ofPoint ofxBlobTracker::getCvMotionImageWhiteCenter(float $threshold) {
	return getCVGrayscaleImageWhiteCenter(videoMotionCvImage, $threshold);
}

ofPoint ofxBlobTracker::getCvMotionHistoryImageWhiteCenter(float $threshold) {
	return getCVGrayscaleImageWhiteCenter(videoMotionHistoryCvImage, $threshold);
}

ofPoint ofxBlobTracker::getCVGrayscaleImageWhiteCenter(ofxCvGrayscaleImage& $image, float $threshold) {
	ofPoint pt(-1, -1);
	if (bIsFrameNew) {
		pt.set(0, 0);
		int totes = 0;
		unsigned char * pixels = $image.getPixels();
		for (int x = 0; x < video_width; x++) {
			for (int y = 0; y < video_height; y++) {
				if (pixels[y * (int)video_width + x] > $threshold) {
					totes += 1;
					pt.x += x;
					pt.y += y;
				}
			}
		}
		pt.x = pt.x / (float) totes;
		pt.y = pt.y / (float) totes;
	}
	return pt;
}


void ofxBlobTracker::learnBg() {
	bLearnBg = true;
	panel->setValueB("B_LEARN_BG", bLearnBg);
}

//------------------------------------------------------------------
//PANEL Setup, update, draw

//------------------------------------------------------------------
void ofxBlobTracker::setupPanel() {
	panel->addPanel("CV Settings", 1, false);
	
	panel->setWhichPanel("CV Settings");
	panel->setWhichColumn(0);
	
	panel->addToggle("Learn Background ", "B_LEARN_BG", false);
	panel->addSlider("Threshold ", "THRESHOLD", 127, 0, 255, true);
	panel->addSlider("Motion Threshold", "MOTION_THRESHOLD", 127, 0, 255, true);
	
	panel->addSlider("Dilations ", "N_DILATIONS", 2, 0, 20, true);
	panel->addSlider("Erosions ", "N_EROSION", 0, 0, 20, true);
	
	panel->addSlider("Min Blob Size ", "MIN_BLOB_SIZE", 50, 0, video_width * video_height * 0.05f, true);
	panel->addSlider("Max Blob Size ", "MAX_BLOB_SIZE", 20000, 0, video_width * video_height * 0.5f, true);
	panel->addSlider("Num Blobs to find ", "N_BLOBS_TO_FIND", 10, 0, 15, true);
	
	panel->addToggle("Track Blobs", "B_TRACK_BLOBS", true);
	panel->addToggle("Track Motion ", "B_TRACK_MOTION", false);
	panel->addSlider("Motion Decrement ", "N_MOTION_DEC", 20, 0, 255, true);
	
	panel->addToggle("Draw Diagnostic", "B_DRAW_DIAGNOSTIC", true);
	panel->addToggle("Draw Contours", "B_DRAW_CONTOURS", true);
	
	if (LIVE_VIDEO) {
		panel->addToggle("Video Settings", "B_VIDEO_SETTINGS", false);
	}
	
	panel->addPanel("Video Image Warping", 1, false);
	panel->setWhichPanel("Video Image Warping");
	panel->addSlider2D("pta", "CV_MANAGER_PANEL_VIDEO_PTA", 0, 0, 0, video_width, 0, video_height, true);
	panel->addSlider2D("ptb", "CV_MANAGER_PANEL_VIDEO_PTB", video_width, 0, 0, video_width, 0, video_height, true);
	panel->addSlider2D("ptc", "CV_MANAGER_PANEL_VIDEO_PTC", video_width, video_height, 0, video_width, 0, video_height, true);
	panel->addSlider2D("ptd", "CV_MANAGER_PANEL_VIDEO_PTD", 0, video_height, 0, video_width, 0, video_height, true);
}

//------------------------------------------------------------------
void ofxBlobTracker::updatePanel() {
	bLearnBg =			panel->getValueB("B_LEARN_BG");
	threshold =			panel->getValueI("THRESHOLD");
	nDilations =		panel->getValueI("N_DILATIONS");
	nErosions =			panel->getValueI("N_EROSION");
	maxOperations =		MAX(nDilations, nErosions);
	minBlobSize =		panel->getValueI("MIN_BLOB_SIZE");
	maxBlobSize =		panel->getValueI("MAX_BLOB_SIZE");
	nBlobsTofind =		panel->getValueI("N_BLOBS_TO_FIND");
	
	bTrackBlobs =		panel->getValueB("B_TRACK_BLOBS");
	bTrackMotion =  	panel->getValueB("B_TRACK_MOTION");
	motionThreshold =	panel->getValueI("MOTION_THRESHOLD");
	motionImageDecrement = panel->getValueI("N_MOTION_DEC");
	
	bDrawDiagnostic =	panel->getValueB("B_DRAW_DIAGNOSTIC");
	bDrawContours	=	panel->getValueB("B_DRAW_CONTOURS");
	
	srcPts[0].set( panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTA", 0), panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTA", 1));
	srcPts[1].set( panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTB", 0), panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTB", 1));
	srcPts[2].set( panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTC", 0), panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTC", 1));
	srcPts[3].set( panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTD", 0), panel->getValueI("CV_MANAGER_PANEL_VIDEO_PTD", 1));
	
	
	if (LIVE_VIDEO) {
		if (bVideoSettings) {
			videoSettings();
			bVideoSettings = false;
			panel->setValueB("B_VIDEO_SETTINGS", false);
		}
		
		bVideoSettings = panel->getValueB("B_VIDEO_SETTINGS");
	}
}



