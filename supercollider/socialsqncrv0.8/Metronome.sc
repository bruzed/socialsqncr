/*
	Metronome
*/

Metronome {
	
	var s; //this is the incoming osc server
	var sOut; //outgoing osc server
	var worldTempo;
	var creatureTempo;
	
	*new { arg s, sOut;
		^super.new.init(s, sOut);
	}
	
	init { arg is, isOut;
		s = is;
		sOut = isOut;
	}
	
	playMetro { arg itempo; //ibeat
		creatureTempo = TempoClock(2, 0);
		creatureTempo.tempo_(itempo);
		creatureTempo.schedAbs(0, { arg beat, sec;
			//s.makeBundle(s.latency, {
				//sOut.sendMsg("/metronomeSend", 1);
				//"metronome".postln;
			//});

			//beat 1
			if( beat % 4 == 0, {
				s.makeBundle(s.latency, {
					sOut.sendMsg("/metronomeSend", 1);
					Synth(\metronome);
				});
			});
			
			//beat 1
			if( beat % 4 == 1, {
				s.makeBundle(s.latency, {
					Synth(\metronome, [\level, 0.1]);
				});
			});
			
			//beat 1
			if( beat % 4 == 2, {
				s.makeBundle(s.latency, {
					Synth(\metronome, [\level, 0.1]);
				});
			});
			
			//beat 1
			if( beat % 4 == 3, {
				s.makeBundle(s.latency, {
					Synth(\metronome, [\level, 0.1]);
				});
			});
			
			1;
		});
	}
	
	stopMetro {
		creatureTempo.stop;
	}
	
}