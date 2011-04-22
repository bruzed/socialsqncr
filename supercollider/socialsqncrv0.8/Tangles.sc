/*
Tangles class
*/

Tangles {
	
	var id; //id of the creature
	var scale; //scale
	var notes; //we need 80 notes - 8 notes * 10 segments
	var seq; //variable to hold the sequence
	var s; //this is the incoming osc server
	var sOut; //outgoing osc server
	var numSEGMENTS = 10; //num segments
	var numNOTES = 8; //num notes per segment
	var baseFREQ; //= 57; //base frequency midi 57 = A @ 220
	//this stuff will come in from oF
	var lookUpArray; //look up array
	var lookUpTable; //look up table to check if note is played or not
	var beat; //the beat
	var creatureTempo; //creature tempo
	var onBeat; //the beat offset
	var pan; //pan creature
	var amp; //creature amp
	var vol; //actual creature vol
	var previousAmp; //creature previous amp
	var sustain; //creature sustain -> > behavior
	var rq; //creature rq -> < behavior
	var trig; //creature trig -> = behavior
	var prevGTPct; //previous greater than percentage used to add or subtract behavior
	var prevLTPct; //previous less than percentage used to add or subtract behavior
	var prevETPct; //previous equal to percentage used to add or subtract behavior
	var segment; //the playseq segment
	var prevSegment; //previous segment
	var interval; //interval
	var anotherJ;
	var volRamp; //ramp up the volume -> fix creature create screen
	
	//constructor
	*new { arg id, creatureTempo;
		^super.new.init(id, creatureTempo);
	}
	
	init { arg iid, iCreatureTempo;
		id = iid;
		pan = 0;
		amp = 0;
		vol = 0.7;
		previousAmp = 0;
		sustain = 1;
		rq = 0.004;
		trig = 0.5;
		prevGTPct = 0;
		prevLTPct = 0;
		prevETPct = 0;
		segment = 0;
		prevSegment = 0;
		interval = 0;
		creatureTempo = iCreatureTempo;
		onBeat = Array.series(8, 1, 1);
		onBeat = onBeat.choose;
		//notes = List.new;
		notes = Array2D.new(numSEGMENTS, numNOTES); //segments, notes
		//baseFREQ = Array.rand(80,33,57); //random base frequencies
		baseFREQ = Array.rand(8,33,45); //random base frequencies
		//lookUpArray = Array.fill(80, {2.rand}); // TEST ITEM
		//lookUpTable = Array2D.fromArray(numSEGMENTS, numNOTES, lookUpArray);
		volRamp = Task({
			10.do({arg i; // play 100 times. For infinite length, use : inf.do();
				//"ramp that shit up".postln;
				if( amp < vol, { amp = amp + 0.01});
				0.5.wait;
			});
		});
	}
	
	setScale_ { arg iscale;
		scale = iscale;
		/*80.do { arg i;
			notes.add(57+scale.choose).midicps;
		}*/
		numSEGMENTS.do { arg i;
			for (0, (numNOTES - 1), { arg j;
				//notes[i, j] = ( baseFREQ + scale.choose ).midicps;
				//notes[i, j] = ( baseFREQ.choose + scale.choose ).midicps; //random base freq
				//scale.size.postln;
				if(scale.size < 8, 
					{
						anotherJ = 0;
						notes[i, j] = ( baseFREQ[j] + scale[anotherJ] ).midicps; //random base freq
					},
					{
						notes[i, j] = ( baseFREQ[j] + scale[j] ).midicps; //random base freq
					}
				);
			});
		}
		
		//^Post << notes;
		
	}
	
	playNote_{ arg i, j;
		var freq;
		freq = notes[i, j]; //segment, note
		//Synth( \tanglesSynth, [\freq, freq, \pan, pan, \amp, rrand(0.1, amp), \sustain, rrand(0.1, 1.0), \preamp, rrand(0.1, 0.9)] );
		//Synth( \tanglesSynth, [\freq, freq, \pan, pan, \amp, amp, \sustain, sustain, \rq, rq, \t_trig, trig] );
		Synth( \tanglesSynth, [\freq, freq, \pan, pan, \amp, 0.6, \sustain, sustain, \rq, rq, \t_trig, trig] );
	}
	
	//incoming osc server, outgoing osc server, sequence
	buildSeq { arg is, isOut, ilookUpTable;
		s = is;
		sOut = isOut;
		lookUpTable = ilookUpTable;
		//lookUpTable = Array2D.fromArray(numSEGMENTS, numNOTES, lookUpArray);  // TEST ITEM
	}
	
	playSeq { arg itempo; //ibeat
		//seq.play(quant: ibeat);
		//var i = 0; //counter to segment through the array
		//interval = 0 + id/2; //interval variation
		creatureTempo.tempo_(itempo);
		creatureTempo.schedAbs(interval, { arg beat, sec;
			//[beat, sec].postln;
			
			//outgoing osc
			if( prevSegment != segment, 
			{
				prevSegment = segment;
				s.makeBundle(s.latency, {
					sOut.sendMsg("/advanceCreatureSegment", id, segment);
					//"start out bundle".postln;
					//[id, segment].postln;
				});
			},
			{
				//[id, segment, prevSegment].postln;
			}
			);
			
			//beat 1
			if( beat % onBeat == interval, {
				//"1".postln;
				//if( i < 9, { i = i + 1; }, { i = 0; });
				if( segment < 9, { segment = segment + 1; }, { segment = 0; });
				//play notes
				numNOTES.do { arg j;
					//if( lookUpTable[i,j] == 1,
					if( lookUpTable[segment, j] == 1,
						{ 
							//lookUpTable[i,j].postln; 
							s.makeBundle(s.latency, {
								var freq;
								freq = notes[segment, j];
								//Synth( \tanglesSynth, [\freq, freq, \pan, pan, \amp, rrand(0.1, amp), \sustain, rrand(0.1, 1.0), \preamp, rrand(0.1, 0.9)] );
								Synth( \tanglesSynth, [\freq, freq, \pan, pan, \amp, amp, \sustain, sustain, \rq, rq, \t_trig, trig] );
							});
						}
					);
				};
			});
			1;
		});
	}
	
	stopSeq {
		creatureTempo.stop;
		//creatureTempo.clear;
	}
	
	getCreatureTempo {
		^creatureTempo;
	}
	
	setCreatureTempo_ { arg iCreatureTempo;
		//creatureTempo.clear;
		//creatureTempo = iCreatureTempo;
	}
	
	getID {
		^id;
	}
	
	setID_ { arg iid;
		prevSegment = 0;
		segment = 0;
		interval = 0;
		id = iid;
	}
	
	pan_ { arg ipan;
		pan = ipan;
	}
	
	amp_ { arg iamp;
		//volRamp.stop;
		amp = amp * iamp;
		"tangles".postln;
		amp.postln;
	}
	
	getAmp {
		^amp;
	}
	
	getPreviousAmp {
		^previousAmp;
	}
	
	equalTo { arg etpct;
		//if(gtpct > prevGTPct, { sustain = sustain + gtpct }, { sustain = sustain - gtpct });
		//prevGTPct = gtpct;
		//rq = rq * etpct;
		//"tangles equal to".postln;
		//rq.postln;
	}
	
	lessThan { arg ltpct;
		//if(ltpct > prevLTPct, { rq = rq + (ltpct/100) }, { rq = rq - (ltpct/100) });
		//prevLTPct = ltpct;
		//rq = rq + (pct/100);
		//trig = trig * ltpct;
		//"tangles less than".postln;
		//trig.postln;
	}
	
	greaterThan { arg gtpct;
		//if(etpct > prevETPct, { trig = trig + (etpct/100) }, { trig = trig - (etpct/100) });
		//prevETPct = etpct;
		//trig = trig + pct;
		//trig = trig * ( 1- gtpct );
		//"tangles greater than".postln;
		//trig.postln;
	}
	
	ampDown {
		previousAmp = amp;
		while ( { amp > 0 }, { amp = amp - 0.01 } );
		if(amp < 0, { amp = 0 });
		amp = 0;
	}
	
	ampUp {
		//previousAmp = amp;
		if(amp > 0, { previousAmp = amp; amp = 0 });
		while ( { amp < previousAmp }, { amp = amp + 0.01 } );
		amp = previousAmp;
	}
	
	rampUp {
		//"ramp up".postln;
		volRamp.play;
	}
	
	userHit { arg pct;
		rq = 0.004 + ( 0.4 * pct );
		"tangles".postln;
		pct. postln;
		trig.postln;
	}
	
}