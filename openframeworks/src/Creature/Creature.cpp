/*
 *  Creature.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Creature.h"

Creature::~Creature() {
	movementPts.clear();
	spine.clear();
	texture.clear();
	for (int i = 0; i < totalPlaneNormals; i++) {
		delete planeNormals[i];
		delete planeCenters[i];
	}
	
	planeNormals.clear();
	planeCenters.clear();
	extrusionPts.clear();
}

void Creature::initiate(int $numPts, int $numSegmentPts, float $maxRadius, float $spineLength, float $extremetySpacing, float $tentacleDivLength) {
	numMovementPts	= $numPts; // stores the amount of recording points
	numBodyPts		= numMovementPts + 1; // add one, so that the movement points will lie in between segments //
	numSegmentPts	= $numSegmentPts;
	float minMid	= 2.f;
	float maxMid	= 4.f;
	spineLength		= $spineLength;
	tentacleDivLength  = $tentacleDivLength;
	
	spawnTime = ofGetElapsedTimeMillis(); // these get set later in the setLifeAtts( float $healthPct )
	deathTime = ofGetElapsedTimeMillis() + 2000;
	bRemoveMe = false;
	bDying = false;
	
	//ID = -1; // check to make sure this is not -1 before sending to superCollider //
	// not in initiate function since not all creatures ( Select Creature Scene ) have an id and are sending sounds //
	
	texture.allocate(CREATURE_THUMB_WIDTH, CREATURE_THUMB_HEIGHT, GL_RGB);
	bHasTexture = false;
	
	segmentIndex	= -1;
	segAngleInc		= TWO_PI / (float) numSegmentPts;
	bNewSegmentSelected = false;
	
	for (int i = 0; i < numMovementPts + 1; i++) {
		MovementSegment ms;
		//ms.set( 0, 0, 0 );
		movementPts.push_back( ms );
		movementPts[movementPts.size()-1].initiate(numSegmentPts, $maxRadius);
	}
	
	totalPlaneNormals = numMovementPts * numSegmentPts;
	for (int i = 0; i < totalPlaneNormals; i++) {
		planeNormals.push_back( new ofxVec3f(0, 0, 0) );
		planeCenters.push_back( new ofxVec3f(0, 0, 0) );
	}
	
	for( int i = 0; i < numBodyPts; i++) {
		CreatureSegment cs;
		cs.loc.set(0, 0, 0);
		cs.extremetySpacing = $extremetySpacing;
		spine.push_back( cs );
	}
	
	for (int i = 0; i < numBodyPts; i++) {
		
		float distX = 1;
		if (i <= minMid) {
			distX = (float)i / minMid;
		} else if (i >= maxMid) {
			distX = 1 - ( ((float)i - maxMid) / (numBodyPts-1 - maxMid));
		}
		
		distX *= $maxRadius;
		spine[i].radFromCenter = distX;
		
		spine[i].initiate(numSegmentPts, spine[i].radFromCenter, spine[i].loc.x, spine[i].loc.y, spine[i].loc.z);
		
	}
	
	// set the default colors //
	segmentHighColor.r = 255; segmentHighColor.g = 50; segmentHighColor.b = 100; segmentHighColor.a = 200;
	segmentBaseColor.r = 255; segmentBaseColor.g = 255; segmentBaseColor.b = 255; segmentBaseColor.a = 50;
	
	extremetyHighColor.r = 200; extremetyHighColor.g = 255; extremetyHighColor.b = 50; extremetyHighColor.a = 200;
	extremetyBaseColor.r = 255; extremetyBaseColor.g = 255; extremetyBaseColor.b = 255; extremetyBaseColor.a = 150;
	
	length = (float)(spineLength-1) * numBodyPts;
	
	tagParticle.loc.set( spine[0].loc.x, spine[0].loc.y, spine[0].loc.z );
	
	lesserPct = greaterPct = equalPct = 0.f;
	
	bRenderWireFrame = false;
}

void Creature::setBodyRadiusFromMotion( float $motionPct, float $midPct, float $minRadius, float $maxRadius ) {
	float maxRadius = ofMap($motionPct, 0, 1.f, $minRadius, $maxRadius);
	int midSeg = floor( (float)numBodyPts * $midPct ) - 1;
	if (midSeg < 1) midSeg = 1;
	if (midSeg > numBodyPts - 2) midSeg = numBodyPts - 2;
	for (int i = 0; i < numBodyPts; i++) {
		float distX = 1;
		if (i <= midSeg) {
			distX = (float)i / midSeg;
		} else if (i >= midSeg) {
			distX = 1 - ( ((float)i - midSeg) / (numBodyPts - 1 - midSeg));
		}
		
		distX *= maxRadius;
		spine[i].radFromCenter = spine[i].radius = distX;
		
	}
}

void Creature::setMovementAtts( float $motionPct ) {
	//spine[0].DAMPING = $motionPct * .85 + .15f;
	// (motionPct) + .01)
	// the motionPct is applied in the update function !! //
	motionPct = $motionPct; 
	//cout << "Creature :: setMovementAtts : spine[0].DAMPING = " << spine[0].DAMPING << endl;
}

void Creature::setLifeAtts( float $healthPct, int $lifeSpan, int $lifeSpanDiff, float $deathStartPct ) {
	spawnTime = ofGetElapsedTimeMillis();
	deathTime = ofGetElapsedTimeMillis() + $lifeSpan + ((float)$lifeSpanDiff * $healthPct );
	deathStartPct = $deathStartPct;
	bRemoveMe = false;
}

// called externally to being killing this little mofo //
void Creature::startDying(float $lifeSpan) {
	spawnTime = ofGetElapsedTimeMillis();
	deathTime = ofGetElapsedTimeMillis() + ($lifeSpan);
	deathStartPct = 1.f;
	bDying = true;
	
	// deathPct = 1 - ((float)(ofGetElapsedTimeMillis() - spawnTime) / (float)(deathTime - spawnTime));
}

void Creature::setSegmentColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a ) {
	segmentHighColor.r = $r; segmentHighColor.g = $g; segmentHighColor.b = $b; segmentHighColor.a = $a;
	segmentBaseColor.r = $low_r; segmentBaseColor.g = $low_g; segmentBaseColor.b = $low_b; segmentBaseColor.a = $low_a;
}

void Creature::setExtremetyColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a ) {
	extremetyHighColor.r = $r; extremetyHighColor.g = $g; extremetyHighColor.b = $b; extremetyHighColor.a = $a;
	extremetyBaseColor.r = $low_r; extremetyBaseColor.g = $low_g; extremetyBaseColor.b = $low_b; extremetyBaseColor.a = $low_a;
}

void Creature::setColors( int $r, int $g, int $b, int $a, int $low_r, int $low_g, int $low_b, int $low_a ) {
	setSegmentColors( $r, $g, $b, $a, $low_r, $low_g, $low_b, $low_a );
	setExtremetyColors( $r, $g, $b, $a, $low_r, $low_g, $low_b, $low_a );
}


///////////////////////////////////////////////////

void Creature::setCreatureAlongVector(ofxVec3f $centerAxis, ofxVec3f $directionNormal) {
	float centerDist = length * .5;
	for (int i = 0; i < numBodyPts; i++) {
		float currDist = ((float)i * spineLength) - centerDist;
		ofxVec3f dir = $directionNormal * -currDist;
		dir += $centerAxis;
		spine[i].loc.set(dir.x, dir.y, dir.z);
		//spine[i].loc.set($x - (i * spineLength), $y, $z);
		
	}
}

void Creature::advanceSegment() {
	segmentIndex += 1;
	if (segmentIndex >= numMovementPts) segmentIndex = 0;
	bNewSegmentSelected = true;
}

void Creature::setActiveSegment(int $segmentIndex) {
	if($segmentIndex != segmentIndex) {
		//cout << "Creature :: Setting active segment " << $segmentIndex << endl;
		segmentIndex = $segmentIndex;
		if (segmentIndex >= numMovementPts) segmentIndex = numMovementPts;
		if (segmentIndex < 0) segmentIndex = 0;
		bNewSegmentSelected = true;
	}
}

void Creature::getNotesForSegment( int* $array, int $segmentIndex) {
	for (int i = 0; i < CREATURE_NUM_NOTES_PER_SEGMENT; i++) {
		$array[i] = movementPts[$segmentIndex].notes[i];
		//$array[i] = 1;
	}
}

void Creature::reset( float $radius ) {
	for (int i = 0; i < numMovementPts; i++) {
		movementPts[i].reset( $radius );
	}
}

void Creature::setTextureFromPixels(unsigned char* $pixels) {
	bHasTexture = true;
	texture.loadData($pixels, CREATURE_THUMB_WIDTH, CREATURE_THUMB_HEIGHT, GL_RGB);
}

void Creature::constrain3D(float $width, float $height, float $depth) {
	if (spine[0].loc.x > $width) {
		//spine[0].vel.x *= -100;
		spine[0].vel.x += -1;
		//spine[0].loc.x = $width;
	}
	if (spine[0].loc.x < 0) {
		//spine[0].vel.x *= -100;
		spine[0].vel.x += 1;
		//spine[0].loc.x = 0;
	}
	
	if (spine[0].loc.y > $height) {
		//spine[0].vel.y *= -100;
		spine[0].vel.y += -1;
		//spine[0].loc.y = $height;
	}
	if (spine[0].loc.y < 0) {
		//spine[0].vel.y *= -100;
		spine[0].vel.y += 1;
		//spine[0].loc.y = 0;
	}
	
	if (spine[0].loc.z > 0) {
		//spine[0].vel.z *= -100;
		spine[0].vel.z += -1;
		//spine[0].loc.z = $depth;
	}
	if (spine[0].loc.z < -$depth) {
		//spine[0].vel.z *= -100;
		spine[0].vel.z += 1;
		//spine[0].loc.z = 0;
	}
}

void Creature::constrain3D( const ofxVec3f& $minBounds, const ofxVec3f& $maxBounds ) {
	if (spine[0].loc.x > $maxBounds.x) {
		//spine[0].vel.x *= -100;
		spine[0].vel.x += -100;
		//spine[0].loc.x = $width;
	}
	if (spine[0].loc.x < $minBounds.x) {
		//spine[0].vel.x *= -100;
		spine[0].vel.x += 100;
		//spine[0].loc.x = 0;
	}
	
	if (spine[0].loc.y > $maxBounds.y) {
		//spine[0].vel.y *= -100;
		spine[0].vel.y += -100;
		//spine[0].loc.y = $height;
	}
	if (spine[0].loc.y < $minBounds.y) {
		//spine[0].vel.y *= -100;
		spine[0].vel.y += 10;
		//spine[0].loc.y = 0;
	}
	
	if (spine[0].loc.z > $maxBounds.z) {
		//spine[0].vel.z *= -100;
		spine[0].vel.z += -100;
		//spine[0].loc.z = $depth;
	}
	if (spine[0].loc.z < $minBounds.z) {
		//spine[0].vel.z *= -100;
		spine[0].vel.z += 100;
		//spine[0].loc.z = 0;
	}
}

//------------------------------------------------------------
void Creature::addForFlocking( Creature* $cr ) {
	
	ofxVec3f diff;//, diffNormalized;
	float distance;
	
	diff			= $cr->spine[0].loc - spine[0].loc;
	distance		= diff.length();
	//diffNormalized	= diff;
	//diffNormalized.normalize();
	
	if( distance > 0 && distance < seperation.minDist ){
		seperation.sum += diff;
		$cr->seperation.sum -= diff;
		seperation.count++;
		$cr->seperation.count++;
	}
	
	if( distance > 0 && distance < alignment.minDist ) {
		alignment.sum += $cr->spine[0].vel;
		$cr->alignment.sum += spine[0].vel;
		alignment.count++;
		$cr->alignment.count++;
	}
	
	if( distance > 0 && distance < cohesion.minDist ){
		cohesion.sum += $cr->spine[0].loc;
		$cr->cohesion.sum += spine[0].loc;
		cohesion.count++;
		$cr->cohesion.count++;
	}
}

void Creature::addFlockingForces() {
	// seperation
	if(seperation.count > 0){
		seperation.sum /= (float)seperation.count;
	}
	
	// alignment
	if(alignment.count > 0){
		alignment.sum /= (float)alignment.count;
	}
	
	// cohesion
	if(cohesion.count > 0){
		cohesion.sum /= (float)cohesion.count;
		cohesion.sum -= spine[0].loc;
	}
	
	spine[0].acc -= (seperation.sum.normalized()		* seperation.strength);
	spine[0].acc += (alignment.sum.normalized()			* alignment.strength);
	spine[0].acc += (cohesion.sum.normalized()			* cohesion.strength);
}

void Creature::resetFlockingForces() {
	// reset the flock info: 
	cohesion.count 	    = 0;
	seperation.count	= 0;
	alignment.count		= 0;
	
	cohesion.sum.set(0, 0, 0); 	
	seperation.sum.set(0, 0, 0); 	
	alignment.sum.set(0, 0, 0); 
}

// updateRecordingBody(), located above called when recording a new creature //
void Creature::update() {
	//cout << "Creature :: update : spine[0].DAMPING = " << spine[0].DAMPING << endl;
	
	ofxVec3f T1, T2, A;
	float alpha;
	bDying = false; 
	
	//ofxVec3f new_forward(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
	ofxVec3f new_forward, approx_up, new_side, new_up;
	ofxVec3f dirNormal, cross;
	
	deathPct = 1 - ((float)(ofGetElapsedTimeMillis() - spawnTime) / (float)(deathTime - spawnTime));
	if (deathPct < 0) deathPct = 0;
	if (deathPct > 1) deathPct = 1;
	if ( deathPct < deathStartPct ) {// .1 is good for this for shorter times //
		bDying = true;
		//cout << "Creature :: update : before Map deathPct = " << deathPct << endl;
		deathPct = ofMap(deathPct, 0, deathStartPct, 0, 1);
		
		ofColor tempSegHighColor = ColorPalette::convertRgbToHsv(segmentHighColor.r, segmentHighColor.g, segmentHighColor.b); // get hsv values from rgb //
		tempSegHighColor = ColorPalette::convertHsvToRgb(tempSegHighColor.r, tempSegHighColor.g * deathPct, tempSegHighColor.b); // reduce saturation //
		
		ofColor tempSegBaseColor = ColorPalette::convertRgbToHsv(segmentBaseColor.r, segmentBaseColor.g, segmentBaseColor.b);
		tempSegBaseColor = ColorPalette::convertHsvToRgb(tempSegBaseColor.r, tempSegBaseColor.g * deathPct, tempSegBaseColor.b); // reduce saturation //
		
		setColors( tempSegHighColor.r, tempSegHighColor.g, tempSegHighColor.b, segmentHighColor.a * deathPct + 10, 
				   tempSegBaseColor.r, tempSegBaseColor.g, tempSegBaseColor.b, segmentBaseColor.a * deathPct + 10 );
		
		//cout << "Creature :: update : after Map deathPct = " << deathPct << endl;
		
		if (deathPct < .00001f) {
			bRemoveMe = true;
		}
	} else {
		deathPct = 1.f;
	}
	
	//cout << "Rendering the body of the creature" << endl;
	for (int i = 0; i < numBodyPts; i++) {
		if (i > 0) {
			spine[i].DAMPING = .7f;
			spine[i].fixedSpring( spine[i-1], .65f, spineLength, false);
		} else {
			float spineDampMult = motionPct + .12f;
			spine[i].acc *= spineDampMult * deathPct;
			//spine[i].addDamping( spine[i].DAMPING * deathPct );
		}
		spine[i].update();
		if (i == segmentIndex) {
			spine[i].segColorPct = 1.f;
			if (bNewSegmentSelected) {
				spine[i].segColorPosPct = 0.f; // so the tendrils and what not can start and fan out in sequence //
				bNewSegmentSelected = false;
			}
		}
	}
	
	for (int i = 0; i < numBodyPts; i++) {
		dirNormal;
		if (i > 0 && i < numBodyPts - 1) {
			dirNormal = spine[i-1].loc - spine[i].loc;
		} else if (i == numBodyPts - 1) {
			dirNormal = spine[i-1].loc - spine[i].loc;
		} else if (i == 0) {
			dirNormal = spine[i].loc - spine[i+1].loc;
		}
		dirNormal.normalize();
		
		
		//ofxVec3f up(0, 1, 0);
		
		if (i == 0) {
			
			// Reynolds Method http://red3d.com/cwr/steer/gdc99/index.html //////////////////////////
			//ofxVec3f new_forward(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
			new_forward.set(dirNormal.x, dirNormal.y, dirNormal.z);
			new_forward.normalize();
			approx_up.set(-spine[i].orientation.up.x, -spine[i].orientation.up.y, -spine[i].orientation.up.z);
			approx_up.normalize();
			new_side = new_forward.crossed(approx_up);
			new_up = new_forward.crossed(new_side);
			new_up.normalize();
			//cout << "approx_up = " << approx_up.x << ", " << approx_up.y << ", " << approx_up.z << endl;
			
			spine[i].orientation.up.set(new_up.x, new_up.y, new_up.z);
			spine[i].orientation.forward.set(new_forward.x, new_forward.y, new_forward.z);
			spine[i].orientation.side.set(new_side.x, new_side.y, new_side.z);
			
			//////////////////////////////////////////////////////////////////////////////
		}  else if (i < numBodyPts) {
			
			// Game Programming Jems 2 Approach //////////////////////////////////////////////////////
			//spine[i].orientation.up.set(spine[i-1].orientation.up.x, spine[i-1].orientation.up.y, spine[i-1].orientation.up.z);
			//spine[i].orientation.up.set(spine[0].orientation.up.x, spine[0].orientation.up.y, spine[0].orientation.up.z);
			//ofxVec3f T1(spine[i].prevVel.x, spine[i].prevVel.y, spine[i].prevVel.z);
			//ofxVec3f T2(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
			//ofxVec3f T1(spine[i-1].prevLoc.x, spine[i-1].prevLoc.y, spine[i-1].prevLoc.z);
			T1.set(spine[i-1].loc.x, spine[i-1].loc.y, spine[i-1].loc.z);
			T2.set(spine[i].loc.x, spine[i].loc.y, spine[i].loc.z);
			//ofxVec3f T1(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
			//ofxVec3f T2(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
			A = T1.crossed(T2);
			//A.normalize();
			alpha = T1.angleRad(T2);
			
			//ofxVec3f new_forward(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
			new_forward.set(dirNormal.x, dirNormal.y, dirNormal.z);
			//ofxVec3f new_forward(spine[i].orientation.forward.x, spine[i].orientation.forward.y, spine[i].orientation.forward.z);
			//ofxVec3f new_forward(spine[0].orientation.forward.x, spine[0].orientation.forward.y, spine[0].orientation.forward.z);
			//new_forward.normalize();
			//new_forward.rotateRad(alpha, A);
			//ofxVec3f approx_up(-spine[0].orientation.up.x, -spine[0].orientation.up.y, -spine[0].orientation.up.z);
			//ofxVec3f approx_up(-spine[i].orientation.up.x, -spine[i].orientation.up.y, -spine[i].orientation.up.z);
			approx_up.set(-spine[i-1].orientation.up.x, -spine[i-1].orientation.up.y, -spine[i-1].orientation.up.z);
			approx_up.normalize();
			approx_up.rotateRad(alpha, A);
			
			new_side = new_forward.crossed(approx_up);
			new_up = new_forward.crossed(new_side);
			
			spine[i].orientation.up.set(new_up.x, new_up.y, new_up.z);
			spine[i].orientation.forward.set(new_forward.x, new_forward.y, new_forward.z);
			spine[i].orientation.side.set(new_side.x, new_side.y, new_side.z);
			
			//angle = alpha;
			 
			////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		
		
		// Reynolds Method http://red3d.com/cwr/steer/gdc99/index.html //////////////////////////
		/*
		ofxVec3f new_forward(dirNormal.x, dirNormal.y, dirNormal.z);
		new_forward.normalize();
		ofxVec3f approx_up(spine[i].orientation.up.x, spine[i].orientation.up.y, spine[i].orientation.up.z);
		approx_up.normalize();
		ofxVec3f new_side = new_forward.crossed(approx_up);
		ofxVec3f new_up = new_forward.crossed(new_side);
		 /////////////////////////////////////////////////////////////////////////////
		*/
		//float dist = spine[i].radFromCenter + spine[i].bodyPts[0].radius; // since all of the radiuses are the same. radii?
		
		float dist = spine[i].radFromCenter * (MAX(deathPct, .3)); // kill this mofo //
		cross = spine[i].orientation.forward.perpendiculared(spine[i].orientation.up);
		cross.normalize();
		//cross.rotateRad( angle, spine[i].orientation.forward);
		cross *= dist;
		//float angle = 0;
		for(int j = 0; j < spine[i].numPts; j++) {
			
			//float dist = spine[i].radFromCenter + spine[i].bodyPts[j].radius;
			
			// Fixed Up Method ///////////////////////////////////////////////////////////////////////
			//ofxVec3f cross = dirNormal.perpendiculared(up);
			//cross.rotateRad(angle, dirNormal);
			//cross *= dist.length();
			//////////////////////////////////////////////////////////////////////////////////////////
			
			// Game Programming Jems 2 Approach //////////////////////////////////////////////////////
			//ofxVec3f cross = spine[i].orientation.forward.perpendiculared(spine[i].orientation.up);
			//cross.normalize();
			//cross.rotateRad( angle, spine[i].orientation.forward);
			//cross *= dist;
			if (j > 0) 
				cross.rotateRad( segAngleInc, spine[i].orientation.forward);
			/////////////////////////////////////////////////////////////////////////////
			
			
			// Reynolds Method //////////////////////////////////////////////////////
			/*
			ofxVec3f cross(new_side.x, new_side.y, new_side.z);
			cross.normalize();
			cross.rotateRad(angle, new_forward);
			cross *= dist.length();
			 */
			/////////////////////////////////////////////////////////////////////////////
			//ofxVec3f spot = 
			
			spine[i].bodyPts[j].set(spine[i].loc.x + cross.x, spine[i].loc.y + cross.y, spine[i].loc.z + cross.z );
			// segAngleInc is calculated once in initiate function //
			//angle += segAngleInc;
		}
		
		//spine[i].prevVel.set(spine[i].vel.x, spine[i].vel.y, spine[i].vel.z);
		//spine[i].prevLoc.set(spine[i].loc.x, spine[i].loc.y, spine[i].loc.z);
		
		// Reynolds Method //////////////////////////////////////////////////////
		//spine[i].orientation.up.set(new_up.x, new_up.y, new_up.z);
		//spine[i].orientation.forward.set(new_forward.x, new_forward.y, new_forward.z);
		//spine[i].orientation.side.set(new_side.x, new_side.y, new_side.z);
		///////////////////////////////////////////////////////////////////////////
	}
	
	// calculate the plane normals and centers so we don't have to repeat if for each derived class //
	ofxVec3f v1, v2, planeCenter;
	int pt2Index;
	int planeNormalIndex = 0;
	for (int i = 0; i < numBodyPts; i++) {
		if (i < numBodyPts - 1) {
			
			for(int j = 0; j < spine[i].numPts; j++) {
				
				pt2Index = j+1;
				if (j == spine[i].numPts - 1) {
					pt2Index = 0;
				} 
				v1.set(spine[i+1].bodyPts[j] - spine[i].bodyPts[j]);
				
				v2 = spine[i+1].bodyPts[j] - spine[i+1].bodyPts[pt2Index];
				
				if (i == numBodyPts - 2) {
					v1 = spine[i].bodyPts[j] - spine[i].bodyPts[pt2Index];
					v2 = spine[i].bodyPts[j] - spine[i+1].bodyPts[j];
				}
				//for (int x = 0; x < video_width; x++) {
				//	for (int y = 0; y < video_height; y++) {
				//		int pixIndex = (y * video_width + x) * 3;
				
				// ofxVec3f planeNormal = v2.cross( v1 );
				// planeNormal.normalize();
				
				v2.cross( v1 );
				planeNormalIndex = j * numMovementPts + i;
				planeNormals[planeNormalIndex]->set(v2.x, v2.y, v2.z);
				//*planeNormals[planeNormalIndex] = v2.cross( v1 );
				planeNormals[planeNormalIndex]->normalize();
				
				planeCenter = (spine[i].bodyPts[pt2Index] + spine[i+1].bodyPts[j]) * .5;
				planeCenters[planeNormalIndex]->set(planeCenter.x, planeCenter.y, planeCenter.z);
			}
		}
	}
	
	if (bHasTexture) {
		tagParticle.prevLoc = spine[0].loc + (70 * deathPct);
		//tagParticle.fixedSpring( tagParticle.prevLoc, .3f, 30.f * deathPct);
		tagParticle.addDamping( tagParticle.DAMPING );
		tagParticle.vel += tagParticle.acc;
		tagParticle.loc += tagParticle.vel;
		tagParticle.acc.set(0, 0, 0);
	}
}


void Creature::draw() {
	float colorPct, oneOverColorPct;
	int totesTriIndices = numBodyPts * (spine[0].numPts + 2);
	glLineWidth (2.f);
	//GLfloat triStripIndices[ totesTriIndices * 3 ];
	for (int i = 0; i < numBodyPts; i++) {
		if (i == 0) {
			/*
			glBegin(GL_LINES);
			ofSetColor(255, 255, 0);
			ofxVec3f upDraw(spine[i].orientation.up.x, spine[i].orientation.up.y, spine[i].orientation.up.z);
			upDraw *= 60;
			glVertex3f(upDraw.x + spine[i].loc.x, upDraw.y + spine[i].loc.y, upDraw.z + spine[i].loc.z);
			glVertex3f(spine[i].loc.x, spine[i].loc.y, spine[i].loc.z);
			ofSetColor(0, 255, 0);
			ofxVec3f fDraw(spine[i].orientation.forward.x, spine[i].orientation.forward.y, spine[i].orientation.forward.z);
			fDraw *= 60;
			glVertex3f(fDraw.x + spine[i].loc.x, fDraw.y + spine[i].loc.y, fDraw.z + spine[i].loc.z);
			glVertex3f(spine[i].loc.x, spine[i].loc.y, spine[i].loc.z);
			
			glEnd();
			 */
		}
		
		if (i < numBodyPts - 1) {
			//ofEnableAlphaBlending();
			
			colorPct = spine[i].segColorPct;
			oneOverColorPct = 1 - colorPct;
			
			ofSetColor((colorPct * segmentHighColor.r) + (oneOverColorPct * segmentBaseColor.r), 
					   (colorPct * segmentHighColor.g) + (oneOverColorPct * segmentBaseColor.g), 
					   (colorPct * segmentHighColor.b) + (oneOverColorPct * segmentBaseColor.b),
					   (colorPct * segmentHighColor.a) + (oneOverColorPct * segmentBaseColor.a));
			
			// only for the record scene, so that we can keep track of where the current index is //
			if (bRenderWireFrame) {
				if (i == segmentIndex) {
					ofSetColor(segmentHighColor.r, segmentHighColor.g, segmentHighColor.b, segmentHighColor.a);
				} else if (i < segmentIndex) {
					ofSetColor(segmentBaseColor.r, segmentBaseColor.g, segmentBaseColor.b, segmentBaseColor.a);
				} else {
					ofColor deadColor = ColorPalette::getInactiveCreatureColor();
					ofSetColor(deadColor.r, deadColor.g, deadColor.b, deadColor.a);
				}
			}
			
			glBegin(GL_TRIANGLE_STRIP);
			for(int j = 0; j < spine[i].numPts; j++) {
				glVertex3f(spine[i].bodyPts[j].x, spine[i].bodyPts[j].y, spine[i].bodyPts[j].z);
				glVertex3f(spine[i+1].bodyPts[j].x, spine[i+1].bodyPts[j].y, spine[i+1].bodyPts[j].z);
			}
			
			glVertex3f(spine[i].bodyPts[0].x, spine[i].bodyPts[0].y, spine[i].bodyPts[0].z);
			glVertex3f(spine[i+1].bodyPts[0].x, spine[i+1].bodyPts[0].y, spine[i+1].bodyPts[0].z);
			
			glEnd();
			
			//ofDisableAlphaBlending();
			
			if (i > 0) {
				float white = 255 * deathPct;
				ofSetColor(white, white, white);
				glBegin(GL_LINE_LOOP);
				for(int j = 0; j < spine[i].numPts; j++) {
					glVertex3f(spine[i].bodyPts[j].x, spine[i].bodyPts[j].y, spine[i].bodyPts[j].z);
				}
				glEnd();
			}
			
			/*
			if (i % 2 == 0)
				ofSetColor(255, 255, 255);
			else
				ofSetColor(100, 100, 100);
			
			glPolygonMode (GL_BACK, GL_LINE);		// Draw Backfacing Polygons As Wireframes
			glLineWidth (1.f);			// Set The Line Width
			
			glBegin(GL_TRIANGLE_STRIP);
			for(int j = 0; j < spine[i].numPts; j++) {
				glVertex3f(spine[i].bodyPts[j].loc.x, spine[i].bodyPts[j].loc.y, spine[i].bodyPts[j].loc.z);
				glVertex3f(spine[i+1].bodyPts[j].loc.x, spine[i+1].bodyPts[j].loc.y, spine[i+1].bodyPts[j].loc.z);
			}
			
			glVertex3f(spine[i].bodyPts[0].loc.x, spine[i].bodyPts[0].loc.y, spine[i].bodyPts[0].loc.z);
			glVertex3f(spine[i+1].bodyPts[0].loc.x, spine[i+1].bodyPts[0].loc.y, spine[i+1].bodyPts[0].loc.z);
			
			glEnd();
			glPolygonMode (GL_BACK, GL_FILL);
			*/
			
			//ofFill();
			// spine[i].numPts = numSegmentPts, which is set in the initiate function //
			/*
			for(int j = 0; j < spine[i].numPts; j++) {
				if (i > 1 && i < numBodyPts - 2) {
					ofSetColor(0, 0, 0);
					glPushMatrix();
					glTranslatef(spine[i].bodyPts[j].loc.x, spine[i].bodyPts[j].loc.y, spine[i].bodyPts[j].loc.z);
					glutSolidSphere(3, 6, 6);
					glPopMatrix();
				}
			}
			 */
		}
	}
	
	if (bHasTexture) {
		
		glBegin(GL_LINES);
		glVertex3f(spine[0].loc.x, spine[0].loc.y, spine[0].loc.z);
		glVertex3f(tagParticle.prevLoc.x, tagParticle.prevLoc.y, tagParticle.prevLoc.z); // this is set in the update //
		glVertex3f(tagParticle.prevLoc.x, tagParticle.prevLoc.y, tagParticle.prevLoc.z);
		glVertex3f(tagParticle.loc.x, tagParticle.loc.y, tagParticle.loc.z);
		glEnd();
		
		/*
		ofxVec3f midCurveLoc = tagParticle.prevLoc - spine[0].loc;
		ofxVec3f endCurveLoc = tagParticle.loc - spine[0].loc;
		ofSetColor(255, 255, 255);
		glPushMatrix();
		glTranslatef(spine[0].loc.x, spine[0].loc.y, spine[0].loc.z);
		ofCurve(0, 0, midCurveLoc.x, midCurveLoc.y, midCurveLoc.x, midCurveLoc.y, endCurveLoc.x, endCurveLoc.y );
		glPopMatrix();
		*/
		
		glPushMatrix();
		glTranslatef(tagParticle.loc.x, tagParticle.loc.y, tagParticle.loc.z);
		ofSetColor(255, 255, 255);
		texture.draw(0, 0, (float)CREATURE_THUMB_WIDTH * deathPct, (float)CREATURE_THUMB_HEIGHT * deathPct);
		glPopMatrix();
		 
	}
	glLineWidth (1.f);
}


