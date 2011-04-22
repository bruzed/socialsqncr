/*
 *  Scene.cpp
 *  openFrameworks
 *
 *  Created by Nick Hardeman on 3/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

Scene::Scene() {
	bAnimInComplete = false;
	bAnimOutComplete = false;
	bTransitioning = false;
}

void Scene::setUpPanel(ofxControlPanel* $panel) {
	panel = $panel;
}

void Scene::reset() {
	
}

void Scene::animateIn() {
	bAnimInComplete = false;
	bAnimOutComplete = false;
	bTransitioning = true;
}

void Scene::animateOut() {
	
}

void Scene::update() {
	
}

void Scene::draw() {
	
}