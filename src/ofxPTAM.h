/*
 *  ofxPTAM.h
 *  try_PTAM
 *
 *  Created by Akira Asia on 10/03/29.
 *  Copyright 2010 __This is Ampontang.__. No rights reserved.
 *
 */

#pragma once

//#define USE_PTAM_VIDEOSRC
#define PTAM_SCALE 0.001

#ifdef USE_PTAM_VIDEOSRC
#include "VideoSource.h"
#endif
#include "cvd/image.h"
#include "cvd/rgb.h"
#include "cvd/byte.h"
#include <vector>


class ofxATANCamera;
class Map;
class MapMaker;
class ofxTracker;


// we cannot import ofMain.h because of TooN's operator override error.
// so cannot use core OF.
// if anyone know workarround, please tell me.

struct screenCoord {
	public:
		double x;
		double y;
		double z;
		int	   level;
};

class ofxPTAM {
	
	private:
		
		#ifdef USE_PTAM_VIDEOSRC
		VideoSource							mVideoSource;
		#endif
		CVD::Image<CVD::Rgb<CVD::byte> >	mimFrameRGB;
		CVD::Image<CVD::byte>				mimFrameBW;
	
		Map									*mpMap; 
		MapMaker							*mpMapMaker; 
		ofxTracker							*mpTracker; 
		ofxATANCamera						*mpCamera;
	
		int									imgWidth;
		int									imgHeight;
		
	public:
		
		void								initPTAM(int imgW, int imgH);
		void								updatePTAM(unsigned char *pixels = 0);
		
		#ifdef USE_PTAM_VIDEOSRC
		void								drawImg();		
		#endif
		void								drawTrail();				
		
		void								beginAR();
		void								endAR();		

		screenCoord*						getCurrentCamPos(int zDepth);
		screenCoord*						getRndmTargetsPos();
	
		void								startBuildMap();	
		void								resetMap();

		bool								bMapBuildComplete;	
		
		double								mapScrnX(double val);
		double								mapScrnY(double val);


};		
		

		