/*
 *  ofxATANCamera.h
 *  try_PTAM3
 *
 *  Created by Akira Asia on 10/04/09.
 *  Copyright 2010 This is Ampontang. No rights reserved.
 *
 */

#pragma once

#include "ATANCamera.h"
#include "ofMain.h"

class ofxATANCamera : public ATANCamera {
public:
		
    ofxATANCamera(string sName) : ATANCamera(sName) { };
		
    void manualParamUpdate(string cfgFile);
		
    bool paramTest();
	
private:

};

