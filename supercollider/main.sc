//social sqncr v0.8
//boot servers
Server.local.boot;
Server.internal.boot;

//DEFAULT VARIABLES
//list of creatures
a = List.new;
//generate a random scale
b = ScaleGen(1);

(
	/*
		Tangles synth
		> sustain	0.1 - 1.0
		< rq		0.1 - 1.0 (Actual 0.001 - 0.01)
		= t_trig	0.1 - 1.0
	*/
	SynthDef(\tanglesSynth, {arg out=0, amp=0.5, t_trig=2, freq=100, rq=0.004, sustain=0.5, preamp=0.2, pan = 0;
		var env, signal, lag;
		var rho, theta, b1, b2;
		var sig;
		b1 = 1.98 * 0.989999999 * cos(0.09);
		b2 = 0.998057.neg;
		signal = SOS.ar(K2A.ar(t_trig), 0.123, 0.0, 0.0, b1, b2);
		signal = RHPF.ar(signal, freq, rq) + RHPF.ar(signal, freq*0.5, rq);
		signal = Decay2.ar(signal, 0.4, 0.3, signal);
		//signal = signal.wrap2 * preamp;
		signal = signal * preamp;
		env = EnvGen.kr(Env.perc(0.01, sustain), doneAction: 2);
		DetectSilence.ar(signal, 0.01, doneAction:2);
		sig = (signal * env) * amp;
		sig = Pan2.ar(sig, pan, 0.5);
		Out.ar(0, sig);
	}).memStore;
	
	//Synth(\tanglesSynth);
	
	/*
		Spiky synth
		> cfreq		0.1 - 1.0 (*150 = 15.5 - 675.5)
		< phase		0.1 - 1.0 (*10 = 1 - 10)
		= sustain	0.1 - 1.0
	*/
	SynthDef(\spikySynth, { arg freq = 220, amp = 0.1, sustain = 0.5, pan = 0, bits = 10, cfreq = 0.5, phase = 0;
		var	sig;
		sig = SinOsc.ar(freq, 0, amp) * EnvGen.kr(Env.perc(0.01, sustain), doneAction: 2);
		//sig=  Decimator.ar(sig, SinOsc.ar(0.05, 0, 9000, 1000).abs, 1);
		sig = Decimator.ar(sig, SinOsc.ar(cfreq, phase, 15000, 10000), bits);
		sig = RHPF.ar(sig, 600);
		//sig = Limiter.ar(sig, 0.9, 0.1);
		sig = Pan2.ar(sig, pan, 0.2);
		Out.ar(0, sig);
	}).memStore;
	
	/*
		Slinky synth
		> cfreq1		0.1 - 1.0	
		< cfreq2		0.1 - 1.0
		= releaseTime	0.1 - 1.0
	*/
	SynthDef(\slinkySynth, { arg i_out = 0, freq = 360, gate = 1, pan = 0, amp = 0.1, attack = 0, sustain = 0.5, releaseTime = 0.5, cfreq1 = 0.25, cfreq2 = 0.3;
		var out, eg, fc, osc, a, b, w, sig;
		//fc = LinExp.kr(LFNoise1.kr(Rand(0.25,0.3)), -1,1,500,2000);
		fc = LinExp.kr(LFNoise1.kr(cfreq1), -1,1,500,2000);
		// fc = 20000;
		//osc = Mix.fill(2, {SinOsc.ar(freq * [Rand(0.99,1.01),Rand(0.99,1.01)], 0, amp) }) .distort* amp;
		osc = Mix.fill(2, {SinOsc.ar(freq * [cfreq2, cfreq2], 0, amp) }) .distort* amp;
		eg = EnvGen.kr(Env.perc(attack, releaseTime, 1, -4), gate, doneAction:2);
		out = eg * RLPF.ar(osc, fc, 0.1);
		#a, b = out;
		//sig = Mix.ar(PanAz.ar(2, [a, b], [pan, pan+0.3]));
		sig = Mix.ar(a, b);
		sig = Pan2.ar(sig, pan, 0.9);
		Out.ar(i_out, sig);
	}).memStore;
	
	/*
		Button
	*/
	
	SynthDef(\buttonClick, { arg amp = 0.1;
		var sig, env, pan;
		sig = Blip.ar([400, 604],  12, Pulse.ar(1.5));
		env = EnvGen.kr(Env.perc(0.1, 0.5, 1, 0), 1, doneAction:2);
		sig = sig * env * amp;
		sig = CombN.ar(sig, 0.1, XLine.kr(0.0001, 0.01, 20), 0.2);
		sig = RHPF.ar(sig, 600);
		sig = Pan2.ar(sig, 0, 0.1);
		Out.ar(0, sig);
	}).memStore;
	
	SynthDef(\buttonOver, { arg freq = 400, amp = 0.1;
		var signal, env;
		signal = Blip.ar( freq, Pulse.ar(1.5) );
		env = EnvGen.kr(Env.perc(0.1, 0.5, 1, 0), 1, doneAction:2);
		signal = signal.distort;
		signal = signal * env * amp;
		signal = Pan2.ar(signal, 0, 0.2);
		Out.ar(0, signal ! 2);
	}).memStore;
	
/*	Synth(\buttonOver, [\freq, 400]);
	Synth(\buttonOver, [\freq, 420]);
	Synth(\buttonOver, [\freq, 440]);
	Synth(\buttonOver, [\freq, 460]);
	Synth(\buttonOver, [\freq, 480]);
	Synth(\buttonOver, [\freq, 500]);
	Synth(\buttonOver, [\freq, 520]);
	Synth(\buttonOver, [\freq, 540]);
	Synth(\buttonOver, [\freq, 560]);
	Synth(\buttonOver, [\freq, 580]);
	Synth(\buttonClick, [\freq, 600]);*/
	
	/*
		Metronome
	*/
	
	SynthDef(\metronome, {arg tempo=1, filterfreq=1000, rq=1.0, amp = 1, level = 0.7;
		var env, signal, bass, env4, snare, hat, env3;
		env3 = EnvGen.kr(Env.perc(0.002, 0.3, 1, -2), 1, doneAction:2);
		hat = Klank.ar(`[ [ 6563, 9875 ],
			               [ 0.6, 0.5 ],
			               [ 0.002, 0.003] ], PinkNoise.ar(1));    
 		signal = Pan2.ar(hat*env3, 0, level);
		Out.ar(0, signal);
	}).memStore;
	
	//Synth(\metronome, [\level, 0.1]);
	
	/*
		Compressor
	*/
	SynthDef("limiter", {
		var audio, comp;
		audio = In.ar(0, 2);
		Compander.ar(audio, audio,
			//thresh: MouseX.kr(0.1, 1),
			thresh: 0.9,
			slopeBelow: 1,
			slopeAbove: 0.1,
			clampTime: 0.01,
			relaxTime: 0.01
		);
	}).memStore;
	
	SynthDef(\hitCreature, { arg freq = 600, amp = 0.4, dur = 2, pan = 0.0; // we add a new argument
		var signal, env;
		env = EnvGen.ar(Env.perc(0.01, dur), doneAction:2); // doneAction gets rid of the synth
		signal = Array.fill(15, {SinOsc.ar(freq+(430.rand), 1.0.rand, 15.reciprocal)}) * env;
		signal = Pan2.ar(signal, pan);
		Out.ar(0, signal * amp);
	}).memStore;
	
	//Synth(\hitCreature);
	
)

j = Synth("limiter");
j.free;

/*
	buttons
	0 - create
	1 - tangles
	2 - spiky
	3 - slinky
	4 - take new photo
	5 - keep photo
*/
t = List.new;

//OSC RESPONDERS
//incoming osc port 57120

//outgoing OSC port 12009
//~netAddr = NetAddr("127.0.0.1", 12009);
~netAddr = NetAddr("169.254.175.225", 12009);
c = Responders(s, ~netAddr, a, b, t); //incoming osc server, outgoing osc server, creature list, scale, buttons
//////////////////////////////////////////////////////
////////////////      TEST        ////////////////////
//////////////////////////////////////////////////////
n = NetAddr("127.0.0.1", 57120);

//metronome
n.sendMsg("/startMetronome", 1);
n.sendMsg("/stopMetronome", 1);

//button
n.sendMsg("/buttonOver", 0, 50);
n.sendMsg("/buttonClick", 3);

//create
n.sendMsg("/create", 0, 0); //tangles
n.sendMsg("/create", 1, 1); //spiky
n.sendMsg("/create", 2, 2); //slinky
n.sendMsg("/create", 0, 3); //tangles
n.sendMsg("/create", 1, 4); //spiky
n.sendMsg("/create", 2, 5); //slinky

//build sequence
u = Array.fill(80, {2.rand});
n.sendMsg("/buildSequence", 0, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);
n.sendMsg("/buildSequence", 1, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);
n.sendMsg("/buildSequence", 2, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);
n.sendMsg("/buildSequence", 3, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);
n.sendMsg("/buildSequence", 4, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);
n.sendMsg("/buildSequence", 5, 	u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], 
								u[8], u[9], u[10], u[11], u[12], u[13], u[14], u[15], 
								u[16], u[17], u[18], u[19], u[20], u[21], u[22], u[23], 
								u[24], u[25], u[26], u[27], u[28], u[29], u[30], u[31], 
								u[32], u[33], u[34], u[35], u[36], u[37], u[38], u[39], 
								u[40], u[41], u[42], u[43], u[44], u[45], u[46], u[47], 
								u[48], u[49], u[50], u[51], u[52], u[53], u[54], u[55], 
								u[56], u[57], u[58], u[59], u[60], u[61], u[62], u[63], 
								u[64], u[65], u[66], u[67], u[68], u[69], u[70], u[71], 
								u[72], u[73], u[74], u[75], u[76], u[77], u[78], u[79]
								);

//start sequence
n.sendMsg("/playSequence", 0, 8);
n.sendMsg("/playSequence", 1, 8);
n.sendMsg("/playSequence", 2, 8);
n.sendMsg("/playSequence", 3, 8);
n.sendMsg("/playSequence", 4, 8);
n.sendMsg("/playSequence", 5, 8);
//pan creature
n.sendMsg("/panCreature", 0, 0);
n.sendMsg("/panCreature", 1, -0.3);
n.sendMsg("/panCreature", 2, -0.2);
n.sendMsg("/panCreature", 3, -0.2);
n.sendMsg("/panCreature", 4, 0.5);
n.sendMsg("/panCreature", 5, 0);
//creature amp
n.sendMsg("/creatureAmp", 0, 0.1);
n.sendMsg("/creatureAmp", 1, 0.2);
n.sendMsg("/creatureAmp", 2, 0.5);
n.sendMsg("/creatureAmp", 3, 0.5);
n.sendMsg("/creatureAmp", 4, 0.1);
n.sendMsg("/creatureAmp", 5, 0.7);
//stop sequence
n.sendMsg("/stopSequence", 0);
n.sendMsg("/stopSequence", 1);
n.sendMsg("/stopSequence", 2);
n.sendMsg("/stopSequence", 3);
n.sendMsg("/stopSequence", 4);
n.sendMsg("/stopSequence", 5);
//delete creature
n.sendMsg("/deleteCreature", 1);
//delete all creatures
n.sendMsg("/deleteAllCreatures", true);
//amp down all creatures
n.sendMsg("/ampDownAllCreatures", true);
//amp up all creatures
n.sendMsg("/ampUpAllCreatures", true);
s.dumpOSC(0);
//change scales
a[0].setScale_(b.getScale);
a[1].setScale_(b.getScale);
a[2].setScale_(b.getScale);
a[3].setScale_(b.getScale);
a[4].setScale_(b.getScale);
a[5].setScale_(b.getScale);
//tangles greaterThan
(
	t = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[0].greaterThan(pct);
			n.sendMsg("/tanglesGT", 0, pct);
			0.5.wait;
		})
	});
	
	u = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[0].greaterThanRev(pct);
			//a[0].greaterThan(pct);
			n.sendMsg("/tanglesGT", 0, pct);
			0.5.wait;
		})
	});
)
t.play;
t.stop;
u.play;
u.stop;
//tangles lessThan
(
	v = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[0].lessThan(pct);
			n.sendMsg("/tanglesLT", 0, pct);
			0.5.wait;
		})
	});
	
	w = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[0].lessThanRev(pct);
			//a[0].lessThan(pct);
			n.sendMsg("/tanglesLT", 0, pct);
			0.5.wait;
		})
	});
)
v.play;
v.stop;
w.play;
w.stop;
//tangles equalTo
(
	v = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[0].equalTo(pct);
			//a[3].equalTo(pct);
			n.sendMsg("/tanglesET", 0, pct);
			n.sendMsg("/tanglesET", 1, pct);
			0.5.wait;
		})
	});
	
	w = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[0].equalToRev(pct);
			//a[3].equalToRev(pct);
			//a[0].equalTo(pct);
			//a[3].equalTo(pct);
			n.sendMsg("/tanglesET", 0, pct);
			n.sendMsg("/tanglesET", 1, pct);
			0.5.wait;
		})
	});
)
v.play;
v.stop;
w.play;
w.stop;
//spiky
//spiky greaterThan
(
	d = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[1].greaterThan(pct);
			n.sendMsg("/spikyGT", 1, pct);
			0.5.wait;
		})
	});
	
	e = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[1].greaterThanRev(pct);
			//a[1].greaterThan(pct);
			n.sendMsg("/spikyGT", 1, pct);
			0.5.wait;
		})
	});
)
d.play;
d.stop;
e.play;
e.stop;
//spiky lessThan
(
	f = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[1].lessThan(pct);
			n.sendMsg("/spikyLT", 1, pct);
			1.wait;
		})
	});
	
	g = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[1].lessThanRev(pct);
			//a[1].lessThan(pct);
			n.sendMsg("/spikyLT", 1, pct);
			1.wait;
		})
	});
)
f.play;
f.stop;
g.play;
g.stop;
//spiky equalTo
(
	h = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[1].equalTo(pct);
			//a[4].equalTo(pct);
			n.sendMsg("/spikyET", 1, pct);
			n.sendMsg("/spikyET", 4, pct);
			0.5.wait;
		})
	});
	
	i = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[1].equalToRev(pct);
			//a[4].equalToRev(pct);
			//a[1].equalTo(pct);
			//a[4].equalTo(pct);
			n.sendMsg("/spikyET", 1, pct);
			n.sendMsg("/spikyET", 4, pct);
			0.5.wait;
		})
	});
)
h.play;
h.stop;
i.play;
i.stop;
//SLINKY
//slinky greaterThan
(
	j = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[2].greaterThan(pct);
			n.sendMsg("/slinkyGT", 2, pct);
			0.5.wait;
		})
	});
	
	k = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[2].greaterThanRev(pct);
			//a[2].greaterThan(pct);
			n.sendMsg("/slinkyGT", 2, pct);
			0.5.wait;
		})
	});
)
j.play;
j.stop;
k.play;
k.stop;
//slinky lessThan
(
	l = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[2].lessThan(pct);
			n.sendMsg("/slinkyLT", 2, pct);
			0.5.wait;
		})
	});
	
	m = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[2].lessThanRev(pct);
			//a[2].lessThan(pct);
			n.sendMsg("/slinkyLT", 2, pct);
			0.5.wait;
		})
	});
)
l.play;
l.stop;
m.play;
m.stop;
//slinky equalTo
(
	p = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = (i + 1)/10;
			pct.postln;
			//a[2].equalTo(pct);
			//a[5].equalTo(pct);
			n.sendMsg("/slinkyET", 2, pct);
			n.sendMsg("/slinkyET", 5, pct);
			0.5.wait;
		})
	});
	
	q = Task({
		10.do({arg i; // play 100 times. For infinite length, use : inf.do();
			var pct = 1 - (i/10);
			pct.postln;
			//a[2].equalToRev(pct);
			//a[5].equalToRev(pct);
			//a[2].equalTo(pct);
			//a[5].equalTo(pct);
			n.sendMsg("/slinkyET", 2, pct);
			n.sendMsg("/slinkyET", 5, pct);
			0.5.wait;
		})
	});
)
p.play;
p.stop;
q.play;
q.stop;