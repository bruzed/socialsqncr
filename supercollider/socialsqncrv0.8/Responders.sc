/*
	Responders Class
*/

Responders {
	
	var s; //incoming osc server address
	var outgoingOscAddress; //outgoing osc server address
	var creatureList; //list of creatures
	var scaleGen; //generated scale
	var worldTempo; //the main elapsed time
	var creatureTempo; //creature tempo
	var creatureBPM; //creature BPM
	
	//RESPONDERS
	var createCreature; //responder to create creature
	var playNote; //responder to play a note
	var buildSequence; //responder to build sequence
	var playSequence; //responder to start a sequence
	var stopSequence; //responder to stop a sequence
	var deleteCreature; //responder to delete creature
	var deleteAllCreatures; //responder to delete all creatures
	var advanceCreatureSegment; //responder to advance creature segment
	var panCreature; //pan creature
	var creatureAmp; //creature amp
	var ampDownAllCreatures; //amp down all creatures
	var ampUpAllCreatures; //amp up all creatures
	//behaviors
	var tanglesGT, tanglesLT, tanglesET; //tangles behavior
	var spikyGT, spikyLT, spikyET; //spiky behavior
	var slinkyGT, slinkyLT, slinkyET; //slinky behavior
	var behavior; //all behaviors
	//metronome
	var metronome;
	var metronomeSend;
	var startMetronome;
	var stopMetronome;
	//ramp up
	var rampUp;
	//buttons
	var buttons;
	var buttonOver;
	var buttonClick;
	//user hit
	var userHit;
	
	*new { arg incomingOscAddress, outgoingOscAddress, creatureList, scaleGen, buttons;
		^super.new.init(incomingOscAddress, outgoingOscAddress, creatureList, scaleGen, buttons);
	}
	
	init { arg iIncomingOscAddress, iOutgoingOscAddress, iCreatureList, iScaleGen, iButtons;
		
		s = iIncomingOscAddress;
		outgoingOscAddress = iOutgoingOscAddress;
		creatureList = iCreatureList;
		scaleGen = iScaleGen;
		buttons = iButtons;
		creatureBPM = 8;
		
		6.do { 
			arg item, i;
			buttons.add( CreateButton( item, s ) );
		};
		
		/*
			1. Create Creature --> type, id
				0 Tangles
				1 Spiky
				2 Slinky
		*/
		createCreature = OSCresponder(nil, '/create', { arg time, resp, msg;
			var type = msg[1];
			var id = msg[2];
			worldTempo = Main.elapsedTime.ceil;
			creatureTempo = TempoClock(2, 0, worldTempo);
			//creature types
			switch (type,
				0, { creatureList.add( Tangles( id, creatureTempo ) ); },
				1, { creatureList.add( Spiky( id, creatureTempo ) ); },
				2, { creatureList.add( Slinky( id, creatureTempo ) ); }
			).postln;	
			msg.do({
				arg item, i;
				//[i, item].postln;
			});
			creatureList[id].setScale_( scaleGen.getScale ); //needs to be moved to create!
		}).add(s);
		
		/*
			2. Playnote --> id, segment index, note index
		*/
		playNote = OSCresponder(nil, '/playnote', { arg time, resp, msg;
			var id, segment, note;
			id = msg[1];
			segment = msg[2];
			note = msg[3];
			creatureList[id].playNote_(segment, note);
			msg.do({
				arg item, i;
				//[i, item].postln;
			});
		}).add(s);

		/*
			3. Build Sequence --> id, sequence of 80 notes
		*/
		buildSequence = OSCresponder(nil, '/buildSequence', { arg time, resp, msg;
			var id, notes, sequence, notesArray;
			notes = List.new;
			msg.do({
				arg item, i;
				//[i, item].postln;
				notes.add( item );
			});
			id = msg[1];
			//"incoming notes".postln;
			//notes[2..].postln;
			//"notes array".postln;
			sequence = Array2D.fromArray(10, 8, notes[2..]);
			//sequence.postln;
			creatureList[id].buildSeq(s, outgoingOscAddress, sequence);
			//creatureList[id].playSeq( creatureBPM );
		}).add(s);

		/*
			4. Start Sequence --> id, true/false
		*/
		playSequence = OSCresponder(nil, '/playSequence', { arg time, resp, msg;
			var id;
			id = msg[1];
			creatureBPM = msg[2]; //value from 1 - 10
			"creature BPM".postln;
			creatureBPM.postln;
			creatureList[id].playSeq( creatureBPM );
		}).add(s);

		/*
			5. Stop Sequence --> id, true/false
		*/
		stopSequence = OSCresponder(nil, '/stopSequence', { arg time, resp, msg;
			var id;
			id = msg[1];
			creatureList[id].stopSeq;
		}).add(s);

		/*
			6. Delete creature -> true
		*/
		deleteCreature = OSCresponder(nil, '/deleteCreature', { arg time, resp, msg;
			var id;
			id = msg[1];
			creatureList[id].stopSeq;
			//"the creature list before".postln;
			//creatureList.postln;
			creatureList.removeAt(id);
			//"the creature list after".postln;
			//creatureList.postln;
			(creatureList.size).do({
				arg item, i;
				creatureList[i].setID_(i);
			});
		}).add(s); 

		/*
			7. Delete all creatures -> true
		*/
		deleteAllCreatures = OSCresponder(nil, '/deleteAllCreatures', { arg time, resp, msg;
			msg.do({
				arg item, i;
				//[i, item].postln;
			});
			(creatureList.size).do({
				arg item, i;
				creatureList[i].stopSeq;
			});
			creatureList.clear;
		}).add(s);

		/*
			8. Advance creature segment -> id, segment coming from class
		*/
		advanceCreatureSegment = OSCresponder(outgoingOscAddress, '/advanceCreatureSegment', { arg time, resp, msg; [time, msg].postln }).add;
		
		/*
			9. Pan Creature -> id, value(-1, 1)
		*/
		panCreature = OSCresponder(nil, '/panCreature', { arg time, resp, msg;
			var id, value;
			id = msg[1];
			value = msg[2];
			creatureList[id].pan_(value);
		}).add(s);
		
		/*
			10. Creature Amplitude -> id, percentage
		*/
		creatureAmp = OSCresponder(nil, '/creatureAmp', { arg time, resp, msg; 
			var id, value;
			id = msg[1];
			value = msg[2];
			creatureList[id].amp_(value);
		}).add(s);
		
		/*
			Behavior -> type, id, gt, lt, et
		*/
		behavior = OSCresponder(nil, '/behavior', { arg time, resp, msg;
			var type, id, gt, lt, et;
			type = msg[1];
			id = msg[2];
			gt = msg[3];
			lt = msg[4];
			et = msg[5];
			//[type, id, gt, lt, et].postln;
			//creatureList[id].greaterThan(gt);
			//creatureList[id].lessThan(lt);
			//creatureList[id].equalTo(et);
			switch (type,
				0, {
					creatureList[id].greaterThan(gt);
					creatureList[id].lessThan(lt);
					creatureList[id].equalTo(et);
				},
				1, {
					creatureList[id].greaterThan(gt);
					creatureList[id].lessThan(lt);
					creatureList[id].equalTo(et);
				},
				2, {
					creatureList[id].greaterThan(gt);
					creatureList[id].lessThan(lt);
					creatureList[id].equalTo(et);
				}
			);
		}).add(s);
		
		/*
			14. Amp down all creatures to 0 -> true
		*/
		ampDownAllCreatures = OSCresponder(nil, '/ampDownAllCreatures', { arg time, resp, msg;
			msg.do({
				arg item, i;
				//[i, item].postln;
			});
			(creatureList.size).do({
				arg item, i;
				creatureList[i].ampDown;
			});
		}).add(s);
		
		/*
			15. Amp up all creatures
		*/
		ampUpAllCreatures = OSCresponder(nil, '/ampUpAllCreatures', { arg time, resp, msg;
			msg.do({
				arg item, i;
				//[i, item].postln;
			});
			(creatureList.size).do({
				arg item, i;
				creatureList[i].ampUp;
			});
		}).add(s);
		
		/*
			16. Metronome
		*/
		metronomeSend = OSCresponder(outgoingOscAddress, '/metronomeSend', { arg time, resp, msg; [time, msg].postln }).add;
		
		startMetronome = OSCresponder(nil, '/startMetronome', { arg time, resp, msg;
			//metronome.set(\tempo, 0.5.reciprocal) // 120 bpm (0.5.reciprocal = 2 bps)
			//a.set(\filterfreq, 4000) // try 1000 (for example)
			//a.set(\rq, 0.1) // try 0.5 (for example)
			metronome = Metronome(s, outgoingOscAddress);
			metronome.playMetro(2);
		}).add(s);
		
		stopMetronome = OSCresponder(nil, '/stopMetronome', { arg time, resp, msg;
			"stop it".postln;
			metronome.stopMetro;
		}).add(s);
		
		/*
			17. Ramp Up volume
		*/
		
		rampUp = OSCresponder(nil, '/rampUp', { arg time, resp, msg;
			var id;
			"ramp up".postln;
			id = msg[1];
			creatureList[id].rampUp;
		}).add(s);
		
		/*
			Buttons
		*/
		
		buttonOver = OSCresponder(nil, '/buttonOver', { arg time, resp, msg;
			var id, freq, amp;
			id = msg[1];
			freq = msg[2];
			amp = msg[3];
			//[id, freq].postln;
			buttons[id].onOver(freq, amp);
		}).add(s);
		
		buttonClick = OSCresponder(nil, '/buttonClick', { arg time, resp, msg;
			var id;
			id = msg[1];
			buttons[id].onClick;
		}).add(s);
		
		/*
			User hit
		*/
		userHit = OSCresponder(nil, '/userHit', { arg time, resp, msg;
			var id, pct;
			id = msg[1];
			pct = msg[2];
			creatureList[id].userHit(pct);
		}).add(s);
		
	}
	
}