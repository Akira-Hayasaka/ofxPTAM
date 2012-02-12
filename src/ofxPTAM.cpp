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

void ofxPTAM::init(int imgW, int imgH) {

	imgWidth = imgW;
	imgHeight = imgH;	
		
	mimFrameBW.resize(ImageRef(imgWidth,imgHeight));
	//mimFrameRGB.resize(ImageRef(imgWidth,imgHeight));	

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

void ofxPTAM::update(ofPixelsRef _pixelsRef){
    int nChannels = _pixelsRef.getNumChannels();
    unsigned char * pixels = _pixelsRef.getPixels();

    ImageRef mirSize = ImageRef(imgWidth,imgHeight);
    BasicImage<CVD::byte> imCaptured(pixels, mirSize);
    mimFrameBW.resize(mirSize);
    
    if (nChannels == 1){
        for (int y=0; y<mirSize.y; y++) {
            for (int x=0; x<mirSize.x; x++) {
                mimFrameBW[y][x]        = *pixels;
                pixels++;
            }
        }
    } else if (nChannels > 1){
        for (int y=0; y<mirSize.y; y++) {
            for (int x=0; x<mirSize.x; x++) {
                pixels++;
                mimFrameBW[y][x]        = *pixels;
                pixels++;
                pixels++;
            }
        }
    } else if (nChannels == 4){
        for (int y=0; y<mirSize.y; y++) {
            for (int x=0; x<mirSize.x; x++) {
                pixels++;
                mimFrameBW[y][x]        = *pixels;
                pixels++;
                pixels++;
                pixels++;
            }
        }
    }

	mpTracker->TrackFrame(mimFrameBW, false);
	bMapBuildComplete = mpMap->IsGood();
}

void ofxPTAM::draw() {
    if (bMapBuildComplete)
        mpTracker->drawGrid();
    else
        mpTracker->drawTrail();
}

void ofxPTAM::resetMap() {
	mpTracker->reset();
}
void ofxPTAM::startBuildMap() {
	mpTracker->buildMapBegin();
}

ofMatrix4x4 ofxPTAM::getRotationMatrix() const {
    SE3<> cvdMapMatrix = mpTracker->GetCurrentPose();
    Matrix<4> cvdCamMatrix = mpCamera->MakeUFBLinearFrustumMatrix(0.005, 100);
    
	ofMatrix4x4 ofMapMatrix;
    ofMatrix4x4 ofCamMatrix;
    
    ofMapMatrix.set(cvdMapMatrix.get_rotation().get_matrix()[0][0], 
                    cvdMapMatrix.get_rotation().get_matrix()[0][1],
                    cvdMapMatrix.get_rotation().get_matrix()[0][2], 
                    0, 
                    cvdMapMatrix.get_rotation().get_matrix()[1][0], 
                    cvdMapMatrix.get_rotation().get_matrix()[1][1], 
                    cvdMapMatrix.get_rotation().get_matrix()[1][2], 
                    0, 
                    cvdMapMatrix.get_rotation().get_matrix()[2][0], 
                    cvdMapMatrix.get_rotation().get_matrix()[2][1],
                    cvdMapMatrix.get_rotation().get_matrix()[2][3], 
                    0, 
                    0, 0, 0, 1);
    
    //ofMapMatrix.translate(cvdMapMatrix.get_translation()[0], cvdMapMatrix.get_translation()[1], cvdMapMatrix.get_translation()[2]);
    
    ofCamMatrix.set(cvdCamMatrix[0][0], cvdCamMatrix[0][1], cvdCamMatrix[0][2], cvdCamMatrix[0][3],
                    cvdCamMatrix[1][0], cvdCamMatrix[1][1], cvdCamMatrix[1][2], cvdCamMatrix[1][3],
                    cvdCamMatrix[2][0], cvdCamMatrix[2][1], cvdCamMatrix[2][2], cvdCamMatrix[2][3],
                    cvdCamMatrix[3][0], cvdCamMatrix[3][1], cvdCamMatrix[3][2], cvdCamMatrix[3][3]);   
                    
    //ofMapMatrix.postMult(ofCamMatrix);
    
    ofMapMatrix = ofCamMatrix * ofMapMatrix;
    
    return ofMapMatrix;
}

/*
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
*/

