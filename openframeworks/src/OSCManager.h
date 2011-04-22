/*
 *  OSCManager.h
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OSC_MANAGER_
#define _OSC_MANAGER_

#include "ofxOsc.h"
#include "Constants.h"

// listen on port 12345
//#define HOST "127.0.0.1"
//#define HOST			"169.254.222.197"
//#define SEND_PORT		12000
//#define RECEIVE_PORT	12009
//#define NUM_MSG_STRINGS 20



class OSCManager {
public:
	
	string HOST;
	int SEND_PORT;
	int RECEIVE_PORT;
	int NUM_MSG_STRINGS;
	
	/// OSC in the place to be //
	ofxOscReceiver	receiver;
	ofxOscSender	sender;
	
	vector <OSCAdvanceSegmentMessage>  segmentAdvMessages; // struct located in Constants //
	bool		bHasAdvMessages;
	
	bool		bHasMetronome;
	
	int			current_msg_string;
	
	bool bBeatReceived; // has the beat been sent? //
	
	OSCManager();
	
	void setup();
	void setup(string $host, int $sendPort, int $receivePort);
	void createNewCreature( int $creatureType, unsigned int $creatureIndex );
	void sendNoteHit( int $creatureIndex, int $segmentIndex, int $noteIndex );
	void buildCreature( unsigned int $creatureIndex, int* notesArray, int totes );
	void playSequence( int $creatureIndex, int $bpm );
	void creatureAmp( int $creatureIndex, float $ampPct );
	void rampUp( int $creatureIndex );
	
	void sendCreatureBehaviors(int $creatureType, int $creatureIndex, float $gtPct, float $ltPct, float $etPct);
	void setPan( int $creatureIndex, float $pan);
	
	// metronome //
	void startMetronome();
	void stopMetronome();
	
	void deleteCreature( int $creatureIndex );
	void deleteAllCreatures( ); // deleteAllCreatures //
	
	void ampDownAllCreatures();
	void ampUpAllCreatures();
	
	void clearAdvMessages();
	
	// button Functions //
	// button IDs are located in Constants //
	void buttonOver(int $btnID, float $pct);
	void buttonClick(int $btnID);
	
	void update();
	
	void sendFrequency( float $freq );
};

#endif