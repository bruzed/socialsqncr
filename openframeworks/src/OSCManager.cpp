/*
 *  OSCManager.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "OSCManager.h"

OSCManager::OSCManager() {
	setup();
}

void OSCManager::setup() {
	HOST =				"127.0.0.1";
	SEND_PORT =			12000;
	RECEIVE_PORT =		12009;
	NUM_MSG_STRINGS =	20;
	bBeatReceived		= false;
	bHasAdvMessages		= false;
}

void OSCManager::setup(string $host, int $sendPort, int $receivePort) {
	HOST =				$host;
	SEND_PORT =			$sendPort;
	RECEIVE_PORT =		$receivePort;
	NUM_MSG_STRINGS =	20;
	
	bBeatReceived		= false;
	bHasAdvMessages		= false;
	
	receiver.setup( RECEIVE_PORT );
	sender.setup( HOST, SEND_PORT );
}

void OSCManager::createNewCreature( int $creatureType, unsigned int $creatureIndex ) {
	cout << "OSCManager :: createNewCreature : type " << $creatureType << " with index: " << $creatureIndex << endl;
	ofxOscMessage m;
	m.setAddress("/create");
	m.addIntArg( $creatureType - 1 );
	m.addIntArg( $creatureIndex );
	sender.sendMessage( m );
}

// called while recording scene is active /////////////////////////////////////////////
void OSCManager::sendNoteHit( int $creatureIndex, int $segmentIndex, int $noteIndex ) {
	cout << "OSCManager :: sendNoteHit : creatureIndex = " << $creatureIndex << " segmentIndex = " << $segmentIndex << " noteIndex = " << $noteIndex << endl;
	ofxOscMessage m;
	m.setAddress("/playnote");
	m.addIntArg( $creatureIndex );
	m.addIntArg( $segmentIndex );
	m.addIntArg( $noteIndex );
	sender.sendMessage( m );
}

void OSCManager::buildCreature( unsigned int $creatureIndex, int* notesArray, int totes ) {
	cout << "OSCManager :: buildCreature : index = " << $creatureIndex << ", total notes = " << totes << endl;
	ofxOscMessage m;
	m.setAddress( "/buildSequence" );
	m.addIntArg( $creatureIndex );
	for (int i = 0; i < totes; i++) {
		//cout << i << ": " << notesArray[i] << endl;
		m.addIntArg( notesArray[i] );
	}
	sender.sendMessage( m );
}

// do not send bpm of 0. should be from 1 - 10. 
void OSCManager::playSequence( int $creatureIndex, int $bpm ) {
	cout << endl << "OSCManager :: playSequence : index " << $creatureIndex << ", bpm = " << $bpm << endl << endl;
	ofxOscMessage m;
	m.setAddress("/playSequence");
	m.addIntArg( $creatureIndex );
	m.addIntArg( $bpm );
	sender.sendMessage( m );
}

// $ampPct is from 0 - 1 //
void OSCManager::creatureAmp( int $creatureIndex, float $ampPct ) {
	cout << "OSCManager :: creatureAmp : index = " << $creatureIndex << " amp = " << $ampPct << endl;
	ofxOscMessage m;
	m.setAddress("/creatureAmp");
	m.addIntArg( $creatureIndex );
	m.addFloatArg( $ampPct );
	sender.sendMessage( m );
}

void OSCManager::rampUp( int $creatureIndex ) {
	cout << endl << "OSCManager :: rampUp : index = " << $creatureIndex << endl << endl;
	ofxOscMessage m;
	m.setAddress("/rampUp");
	m.addIntArg( $creatureIndex );
	sender.sendMessage( m );
}


void OSCManager::sendCreatureBehaviors(int $creatureType, int $creatureIndex, float $gtPct, float $ltPct, float $etPct) {
	cout << "OSCManager :: sendCreatureBehaviors : index = " << $creatureIndex << " greater = " << $gtPct << " lesser = " << $ltPct << " equal = " << $etPct << endl;
	ofxOscMessage m;
	m.setAddress("/behavior");
	m.addIntArg( $creatureType );
	m.addIntArg( $creatureIndex );
	m.addFloatArg( $gtPct );
	m.addFloatArg( $ltPct );
	m.addFloatArg( $etPct );
	sender.sendMessage( m );
}

void OSCManager::setPan( int $creatureIndex, float $pan) {
	cout << "OSCManager :: setPan : setting pan = " << $pan << endl;
	ofxOscMessage m;
	m.setAddress("/panCreature");
	m.addIntArg( $creatureIndex );
	m.addFloatArg( $pan );
	sender.sendMessage( m );
}

void OSCManager::startMetronome() {
	cout << "OSCManager :: startMetronome : Start Metronome !! " << endl;
	ofxOscMessage m;
	m.setAddress("/startMetronome");
	m.addIntArg( 1 );
	sender.sendMessage( m );
}

void OSCManager::stopMetronome() {
	cout << "OSCManager :: stopMetronome : Stop Metronome !! " << endl;
	ofxOscMessage m;
	m.setAddress("/stopMetronome");
	m.addIntArg( 1 );
	sender.sendMessage( m );
}

void OSCManager::deleteCreature( int $creatureIndex ) {
	cout << endl << "OSCManager :: deleteCreature : deleting creature with index = " << $creatureIndex << endl << endl;
	ofxOscMessage m;
	m.setAddress("/deleteCreature");
	m.addIntArg( $creatureIndex );
	sender.sendMessage( m );
}

void OSCManager::deleteAllCreatures( ) {
	cout << "OSCManager :: deleteAllCreatures : DELETING ALL CREATURES!!" << endl;
	ofxOscMessage m;
	m.setAddress("/deleteAllCreatures");
	m.addIntArg(1);
	sender.sendMessage( m );
}

void OSCManager::ampDownAllCreatures() {
	cout << endl <<  "OSCManager :: ampDownAllCreatures : Amping DOWN All Creatures !!" << endl << endl;
	ofxOscMessage m;
	m.setAddress("/ampDownAllCreatures");
	m.addIntArg(1);
	sender.sendMessage( m );
}

void OSCManager::ampUpAllCreatures() {
	cout << endl << "OSCManager :: ampUpAllCreatures : Amping UP All Creatures !!" << endl << endl;
	ofxOscMessage m;
	m.setAddress("/ampUpAllCreatures");
	m.addIntArg(1);
	sender.sendMessage( m );
}

void OSCManager::sendFrequency( float $freq ) {
	cout << "OSCManager sendFrequency of " << $freq << endl;
	
	ofxOscMessage m;
	m.setAddress( "/s_new" );
	//m.addIntArg( 1 );
	//m.addFloatArg( 3.5f );
	m.addStringArg( "singrain" );
	// m.addIntArg( synthID );
	m.addIntArg( 0 );
	m.addIntArg( 0 );
	m.addStringArg( "freq" );
	m.addFloatArg( $freq );
	m.addStringArg( "amp" );
	m.addFloatArg( 0.2 );
	m.addStringArg( "sustain" );
	m.addFloatArg( 0.8 );
	m.addFloatArg( ofGetElapsedTimef() );
	sender.sendMessage( m );
	//s.sendMsg(\s_new, \singrain, s.nextNodeID, 0, 0, \freq, 300, \amp, 0.1, \sustain, 0.8);
	
	// ~s.sendMsg("/s_new", "singrainArray", s.nextNodeID, 0, 0, \freq, $[, 440, 241, $]);
	
} 

void OSCManager::clearAdvMessages() {
	segmentAdvMessages.clear();
	bHasAdvMessages = false;
}

// Button Functions //
// $pct to function from 0 - 1
// freq = 400 -> 580
// amp  = 0 -> .6
void OSCManager::buttonOver(int $btnID, float $pct) {
	ofxOscMessage m;
	m.setAddress("/buttonOver");
	m.addIntArg( $btnID );
	float freq = 400 + ($pct * 180);
	m.addFloatArg( freq );
	float amp = ofMap($pct, 0, 1, 0, .2);
	m.addFloatArg(amp);
	cout << endl << "OSCManager :: buttonOver : BUTTON OVER pct = " << $pct << " amp = " << amp << " freq = " << freq << endl << endl;
	sender.sendMessage( m );
}

void OSCManager::buttonClick(int $btnID) {
	cout << endl << "OSCManager :: buttonClick : BUTTON CLICK for = " << $btnID << endl << endl;
	ofxOscMessage m;
	m.setAddress("/buttonClick");
	m.addIntArg( $btnID );
	sender.sendMessage( m );
}

//////////////////////

void OSCManager::update() {
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() ) {
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		//cout << "OSCManager :: received message with address " << m.getAddress() << endl;
		
		if ( m.getAddress() == "/beat" ) {
			if( m.getArgType( 0 ) == OFXOSC_TYPE_INT32 ) {
				int bBeatArg = m.getArgAsInt32( 0 );
				cout << "OSCManager :: update : Received a beat from : " << m.getAddress() << endl;
				if (bBeatReceived == false) { // make sure we haven't already noticed that we need to pick it up //
					bBeatReceived = bBeatArg > 0 ? true : false;
				}
			} else {
				cout << "OSCManager :: update : Please send the beat as an int!" << endl;
			}
		} else if (m.getAddress() == "/advanceCreatureSegment") {
			OSCAdvanceSegmentMessage tempMess;
			tempMess.creatureIndex	= m.getArgAsInt32( 0 );
			tempMess.segmentIndex	= m.getArgAsInt32( 1 );
			
			//cout << "OSCManager :: got some messages for advancing " << "creatureID: " << tempMess.creatureID << " segIndex: " << tempMess.segmentIndex << endl;
			
			segmentAdvMessages.push_back( tempMess );
			bHasAdvMessages = true; // set to false when you retrieve messages //
		} else if ( m.getAddress() == "/metronomeSend" ) {
			cout << "OSCManager :: update : Metronome Beat Detected !! " << endl;
			// this is being grabbed and set to false in the recordScene->update function //
			bHasMetronome = true;
		}
	}
}


