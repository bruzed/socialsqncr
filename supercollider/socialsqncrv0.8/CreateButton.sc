/*
	Button
*/

CreateButton {
	
	var id;
	var s; //this is the incoming osc server
	var freq; //base freq
	var amp;
	
	*new { arg id, s;
		^super.new.init(id, s);
	}
	
	init { arg iid, is;
		id = iid;
		s = is;
		freq = 0;
		amp = 0;
	}
	
	onOver { arg iFreq, iAmp; //freq, amp
		freq = iFreq;
		amp = iAmp;
		Synth(\buttonOver, [\freq, freq, \amp, amp]);
	}
	
	onClick {
		Synth(\buttonClick, [\freq, 600]);
	}
	
}