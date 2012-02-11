/*
 *  ofxTracker.h
 *  try_PTAM3
 *
 *  Created by Akira Asia on 10/04/12.
 *  Copyright 2010 This is Ampontang. No rights reserved.
 *
 */
#pragma once

#include "Tracker.h"
#include "ofMain.h"


class ofxTracker : public Tracker {
public:
	
    ofxTracker(CVD::ImageRef irVideoSize, const ATANCamera &c, Map &m, MapMaker &mm) : Tracker(irVideoSize, c, m, mm){ };
		
    void reset(){ Reset(); };
    void buildMapBegin();
    
    void drawTrail();
	void drawGrid();
	
    vector<int> trail;
};