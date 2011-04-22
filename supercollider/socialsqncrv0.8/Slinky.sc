/*
Slinky class
*/

Slinky {
	
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
	var pan; //creature pan
	var amp; //creature amp
	var vol; //actual creature volume
	var previousAmp; //creature previous amp
	var cfreq2; //creature control freq2 -> > behavior
	var cfreq1; //creature control freq 1 -> < behavior
	var releaseTime; //creature note release time -> = behavior
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
		vol = 0.8;
		previousAmp = 0;
		cfreq2 = 0.3;
		cfreq1 = 0.25;
		releaseTime = 0.5;
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
		//baseFREQ = Array.rand(87, 45, 69); //random base frequencies
		baseFREQ = Array.rand(8, 45, 69); //random base frequencies
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
		//Synth(\slinkySynth, [\freq, freq, \pan, pan, \amp, rrand(0.1, amp), \attack, rrand(0.1, 0.3)]);
		//Synth(\slinkySynth, [\freq, freq, \pan, pan, \amp, amp, \attack, rrand(0.1, 0.3), \cfreq2, cfreq2, \cfreq1, cfreq1, \releaseTime, releaseTime]);
		Synth(\slinkySynth, [\freq, freq, \pan, pan, \amp, 0.2, \attack, rrand(0.1, 0.3), \cfreq2, cfreq2, \cfreq1, cfreq1, \releaseTime, releaseTime]);
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
		creatureTempo.tempo_(itempo);
		creatureTempo.schedAbs(interval, { arg beat, sec;
			//[beat, sec].postln;
			
			//beat 1
			if( beat % onBeat == interval, {
				
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
				
				//"1".postln;
				//if( i < 9, { i = i + 1; }, { i = 0; });
				if( segment < 9, { segment = segment + 1; }, { segment = 0; });
				//play notes
				numNOTES.do { arg j;
					if( lookUpTable[segment,j] == 1,
						{ 
							//lookUpTable[i,j].postln; 
							s.makeBundle(s.latency, {
								var freq;
								freq = notes[segment,j];
								//Synth(\slinkySynth, [\freq, freq, \pan, pan, \amp, rrand(0.1, amp), \attack, rrand(0.1, 0.3)]);
								Synth(\slinkySynth, [\freq, freq, \pan, pan, \amp, amp, \attack, rrand(0.1, 0.3), \cfreq2, cfreq2, \cfreq1, cfreq1, \releaseTime, releaseTime]);
								if( segment == 0, { sOut.sendMsg("/advanceCreatureSegment", id, segment); });
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
		amp = amp * iamp;
		"slinky".postln;
		amp.postln;
	}
	
	getAmp {
		^amp;
	}
	
	getPreviousAmp {
		^previousAmp;
	}
	
	greaterThan { arg gtpct;
		//if(gtpct > prevGTPct, { cfreq1 = cfreq1 + gtpct }, { cfreq1 = cfreq1 - gtpct });
		//prevGTPct = gtpct;
		//"slinky greater than".postln;
		//cfreq1.postln;
	}
	
	lessThan { arg ltpct;
		//if(ltpct > prevLTPct, { cfreq2 = cfreq2 + (ltpct/100) }, { cfreq2 = cfreq2 - (ltpct/100) });
		//prevLTPct = ltpct;
		//"slinky less than".postln;
		//cfreq1.postln;
	}
	
	equalTo { arg etpct;
		//if(etpct > prevETPct, { releaseTime = releaseTime + (etpct/2) }, { releaseTime = releaseTime - (etpct/2) });
		//prevETPct = etpct;
		//"equal to".postln;
		//releaseTime.postln;
	}
	
	ampDown {
		previousAmp = amp;
		while ( { amp > 0 }, { amp = amp - 0.01; amp.postln } );
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
		cfreq2 = 0.25 + (0.25 * pct);
		"slinky".postln;
		pct.postln;
		cfreq2.postln;
	}
	
}