/*
 *  ofxBlobTracker.h
 *  openFrameworks
 *
// Original Code from Zachery Lieberman //
 *
 */

#ifndef _OFX_BLOB_TRACKER_
#define _OFX_BLOB_TRACKER_

#include "ofxOpenCv.h"
#include "ofxControlPanel.h"
#include "ofMain.h"

typedef struct {
	ofPoint			pos;
	ofPoint			posSmooth;
	unsigned int	nFramesActive;
	bool			bFoundThisFrame;
	int				whoThisFrame;
	unsigned int	id;
} BlobTracker;

class ofxBlobTracker {
public: 
	
	ofxCvColorImage			videoColorCvImage;
	ofxCvGrayscaleImage		videoGrayScaleCvImage;
	ofxCvGrayscaleImage		videoBgCvImage;
	ofxCvGrayscaleImage		videoDiffCvImage;
	
	ofxCvGrayscaleImage		videoMotionCvImage;
	ofxCvGrayscaleImage		videoMotionHistoryCvImage;
	ofxCvGrayscaleImage		videoPrevFrameCvImage;
	
	// add image so that we can warp into it //
	ofxCvGrayscaleImage		videoGrayscaleCvImagePreWarp;
	ofPoint		srcPts[4];
	ofPoint		dstPts[4];
	
	ofxCvContourFinder		contourFinder;
	
	ofVideoGrabber			videoGrabber;
	ofVideoPlayer			videoPlayer;
	
	vector < BlobTracker >		trackerObjects;
	
	unsigned int trackerIndex;
	float		video_width, video_height;
	
	int nBlobsFound;
	
	bool LIVE_VIDEO;
	bool bIsFrameNew;
	
	bool bLearnBg;
	int threshold;
	int motionThreshold;
	int nDilations;
	int nErosions;
	int maxOperations;
	int minBlobSize;
	int maxBlobSize;
	int nBlobsTofind;
	int motionImageDecrement;
	
	bool bPaused;
	bool bDrawDiagnostic;
	bool bDrawContours;
	bool bTrackBlobs;
	bool bTrackMotion;
	bool bVideoSettings;
	bool bDrawShowVideo;
	
	// used for live video //
	void setup(ofxControlPanel * $panel, float $w = 320, float $h = 240);
	void setup(ofxControlPanel * $panel, string $loc, bool $autoStart = true);
	
	void setup(ofxControlPanel * $panel, ofVideoGrabber& $grabber, float $w, float $h);
	void setup(ofxControlPanel * $panel, ofVideoPlayer& $player, float $w, float $h);
	
	void videoSettings();
	void listDevices();
	void togglePlayPause();
	void toggleDiagnostic();
	
	void learnBg();
	
	void init();
	
	// -----------------------------------------------------------------
	// Utils
	ofPoint getCvMotionImageWhiteCenter(float $threshold);
	ofPoint getCvMotionHistoryImageWhiteCenter(float $threshold);
	ofPoint getCVGrayscaleImageWhiteCenter(ofxCvGrayscaleImage& $image, float $threshold);
	
	
	//------------------------------------------------------------------
	//Control Panel Stuff
	void setupPanel();
	void updatePanel();
	
	void update();
	void draw();
	
	vector <BlobTracker> * getTracker();
	
	ofxCvContourFinder*		getContourFinder();
	ofxCvGrayscaleImage*	getCvMotionImage();
	ofxCvGrayscaleImage*	getCvMotionHistoryImagePointer();
	ofxCvGrayscaleImage&	getCvMotionHistoryImage();
	ofxCvColorImage*		getCvColorImage();
	
	//Control Panel
	ofxControlPanel * panel;
};

#endif