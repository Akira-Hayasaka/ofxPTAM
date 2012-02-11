/*
 *  ofxPTAM.cpp
 *  try_PTAM
 *
 *  Created by Akira Asia on 10/03/29.
 *  Copyright 2010 __This is Ampontang.__. No rights reserved.
 *
 */

#include "ofxPTAM.h"
#include "ofxATANCamera.h"

#include "cvd/colourspace.h"
#include "gvars3/instances.h"
#include "MapMaker.h"
#include "ofxTracker.h"
#include "OpenGL.h"
#include "MapPoint.h"

using namespace CVD;
using namespace GVars3;

void ofxPTAM::initPTAM(int imgW, int imgH) {

	imgWidth = imgW;
	imgHeight = imgH;	
		
	mimFrameBW.resize(ImageRef(imgWidth,imgHeight));
	mimFrameRGB.resize(ImageRef(imgWidth,imgHeight));	

	GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters", ATANCamera::mvDefaultParams, HIDDEN);
	mpCamera = new ofxATANCamera("Camera");
	mpCamera->manualParamUpdate("camera.cfg");	
	
	if (!mpCamera->paramTest()) {
		cout << endl;
		cout << "! Camera.Parameters is not set, need to run the CameraCalibrator tool" << endl;
		cout << "  and/or put the Camera.Parameters= line into the appropriate .cfg file." << endl;
		exit(1);
	}	
	
	mpMap = new Map;
	mpMapMaker = new MapMaker(*mpMap, *mpCamera);
	mpTracker = new ofxTracker(ImageRef(imgWidth,imgHeight), *mpCamera, *mpMap, *mpMapMaker);	
	
	bMapBuildComplete = false;

}

void ofxPTAM::updatePTAM(unsigned char *pixels) {
	
	#ifdef USE_PTAM_VIDEOSRC
		mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB);  
	#else
		ImageRef mirSize = ImageRef(imgWidth,imgHeight);
		BasicImage<CVD::byte> imCaptured(pixels, mirSize);
		mimFrameBW.resize(mirSize);
		for (int y=0; y<mirSize.y; y++) {
			for (int x=0; x<mirSize.x; x++) {
				pixels++;
				mimFrameBW[y][x]        = *pixels;
				pixels++;
				pixels++;
			}
		}
	#endif
	mpTracker->TrackFrame(mimFrameBW, false);
	bMapBuildComplete = mpMap->IsGood();	
}

#ifdef USE_PTAM_VIDEOSRC
void ofxPTAM::drawImg() {
	ofImage img;
	img.setFromPixels(reinterpret_cast<unsigned char*>(mimFrameRGB.data()), imgWidth, imgHeight, OF_IMAGE_COLOR);
	img.draw(0,0);
}
#endif

void ofxPTAM::drawTrail() {
	mpTracker->drawTrail();
}

void ofxPTAM::drawGrid() {
    if (bMapBuildComplete)
        mpTracker->drawGrid();
}

ofMatrix4x4 ofxPTAM::getRotationMatrix() const {
    SE3<> se3CfromW = mpTracker->GetCurrentPose();
	ofMatrix4x4 matrix;
    
    /*
    matrix.set(se3CfromW.get_rotation().get_matrix()[0][0], 
               se3CfromW.get_rotation().get_matrix()[0][1],
               se3CfromW.get_rotation().get_matrix()[0][2], 
               0, 
               se3CfromW.get_rotation().get_matrix()[1][0], 
               se3CfromW.get_rotation().get_matrix()[1][1], 
               se3CfromW.get_rotation().get_matrix()[1][2], 
               0, 
               se3CfromW.get_rotation().get_matrix()[2][0], 
               se3CfromW.get_rotation().get_matrix()[2][1],
               se3CfromW.get_rotation().get_matrix()[2][3], 
               0, 
               0, 0, 0, 1);
     */
    /*
    matrix.translate(se3CfromW.get_translation()[0] * PTAM_SCALE, 
                     se3CfromW.get_translation()[1] * PTAM_SCALE, 
                     se3CfromW.get_translation()[2] * PTAM_SCALE);
    */
    
    matrix.set(se3CfromW.get_rotation().get_matrix()[0][0], se3CfromW.get_rotation().get_matrix()[1][0], se3CfromW.get_rotation().get_matrix()[2][0], 0, 
               se3CfromW.get_rotation().get_matrix()[0][1], se3CfromW.get_rotation().get_matrix()[1][1], se3CfromW.get_rotation().get_matrix()[2][1], 0, 
               se3CfromW.get_rotation().get_matrix()[0][2], se3CfromW.get_rotation().get_matrix()[1][2],se3CfromW.get_rotation().get_matrix()[2][3], 0, 
               0, 0, 0, 1);
    /*
    matrix.translate(se3CfromW.get_translation()[0], 
                     se3CfromW.get_translation()[1], 
                     se3CfromW.get_translation()[2]);
    
    matrix.scale(PTAM_SCALE, PTAM_SCALE, PTAM_SCALE);
    */
    return matrix;
}

void ofxPTAM::beginAR() {
	glPushMatrix();
	
	// Set up AR
	mpCamera->SetImageSize(ImageRef(imgWidth,imgHeight));
	SE3<> se3CfromW = mpTracker->GetCurrentPose();
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	glMultMatrix(mpCamera->MakeUFBLinearFrustumMatrix(0.005, 100));
	glMultMatrix(se3CfromW);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrix(SE3<>());

	string sCaption;
	sCaption = mpTracker->GetMessageForUser();
	ofSetWindowTitle(sCaption);

	// test
	GLdouble glm[16];
	glm[0] = se3CfromW.get_rotation().get_matrix()[0][0]; glm[1] = se3CfromW.get_rotation().get_matrix()[1][0]; glm[2] = se3CfromW.get_rotation().get_matrix()[2][0]; glm[3] = 0;
	glm[4] = se3CfromW.get_rotation().get_matrix()[0][1]; glm[5] = se3CfromW.get_rotation().get_matrix()[1][1]; glm[6] = se3CfromW.get_rotation().get_matrix()[2][1]; glm[7] = 0;
	glm[8] = se3CfromW.get_rotation().get_matrix()[0][2]; glm[9] = se3CfromW.get_rotation().get_matrix()[1][2]; glm[10] = se3CfromW.get_rotation().get_matrix()[2][2]; glm[11] = 0;
	glm[12] = 0; glm[13] = 0; glm[14] = 0; glm[15] = 1;	
	//cout << glm << endl;
	//cout << se3CfromW.get_translation() << endl;
	//cout << mpCamera->MakeUFBLinearFrustumMatrix(0.005, 100) << endl;
}
void ofxPTAM::endAR() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPopMatrix();
}

screenCoord* ofxPTAM::getCurrentCamPos(int zDepth) {
	SE3<> se3CfromW = mpTracker->GetCurrentPose();	
	Vector<3> v3z = makeVector( 0.0, 0.0, 1.0 );
	Vector<3> dir = se3CfromW.inverse().get_rotation() * v3z;
	Vector<3> pos = se3CfromW.inverse().get_translation();
	pos += dir * zDepth;
	//pos += v3z * zDepth;
	screenCoord *sc = new screenCoord();
	sc->x = pos[0];
	sc->y = pos[1];
	sc->z = pos[2];
	return sc;
}

screenCoord* ofxPTAM::getRndmTargetsPos() {
	int n = rand() % mpMap->vpPoints.size();
	Vector<3> pos = mpMap->vpPoints.at(n)->v3WorldPos;
	screenCoord *sc = new screenCoord();
	int level = mpMap->vpPoints.at(n)->nSourceLevel;
	sc->x = pos[0];
	sc->y = pos[1];
	sc->z = pos[2];
	sc->level = level;
	return sc;
}

void ofxPTAM::resetMap() {
	mpTracker->reset();
}
void ofxPTAM::startBuildMap() {
	mpTracker->buildMapBegin();
}


double ofxPTAM::mapScrnX(double val) {

	double outVal = ((val - (0)) / (imgWidth - 0) * (1.0 - (-1.0)) + (-1.0));
	outVal = outVal/PTAM_SCALE;
	return outVal;
	
}
double ofxPTAM::mapScrnY(double val) {

	double outVal = ((val - (0)) / (imgHeight - 0) * (1.0 - (-1.0)) + (-1.0));
	outVal = outVal/PTAM_SCALE;
	return -outVal;
	
}


