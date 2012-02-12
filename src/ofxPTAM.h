/*
 *  ofxPTAM.h
 *  try_PTAM
 *
 *  Created by Akira Asia on 10/03/29.
 *  Copyright 2010 __This is Ampontang.__. No rights reserved.
 *
 */

#pragma once

#define PTAM_SCALE 0.001

#ifdef USE_PTAM_VIDEOSRC
#include "VideoSource.h"
#endif
#include "cvd/image.h"
#include "cvd/rgb.h"
#include "cvd/byte.h"
#include <vector>

#include "ofMain.h"

class ofxATANCamera;
class Map;
class MapMaker;
class ofxTracker;

struct screenCoord {
public:
    double x;
    double y;
    double z;
    int	   level;
};

class ofxPTAM {
public:
    void    init(int imgW, int imgH);
    void    update(ofPixelsRef _pixelsRef);
    void	draw();
    
    void	startBuildMap();	
    void	resetMap();
    bool    isMapBuild() const { return bMapBuildComplete; };
		
    // TODOS
    //  - use the ofxFaceTracking intarfaces 
    ofVec2f     getPosition() const;
	float       getScale() const;
	ofVec3f     getOrientation() const;
	ofMatrix4x4 getRotationMatrix() const;
    
    /*
    // Let´s keep the things clean for the moment
    void	beginAR();
    void	endAR();		

    screenCoord*	getCurrentCamPos(int zDepth);
    screenCoord*	getRndmTargetsPos();
        
    double			mapScrnX(double val);
    double			mapScrnY(double val);*/
    
private:
    //CVD::Image<CVD::Rgb<CVD::byte> >	mimFrameRGB;    // No usfull for the moment
    CVD::Image<CVD::byte>   mimFrameBW;
	
    Map                 *mpMap; 
    MapMaker            *mpMapMaker; 
    ofxTracker          *mpTracker; 
    ofxATANCamera       *mpCamera;
	
    int					imgWidth;
    int					imgHeight;
    
    bool				bMapBuildComplete;
};		
		

		