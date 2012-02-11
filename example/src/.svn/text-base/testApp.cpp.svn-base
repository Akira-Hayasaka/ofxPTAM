#include "testApp.h"


GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};


void testApp::setup(){
	ofEnableAlphaBlending();
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	
    squirrelModel.loadModel("squirrel/NewSquirrel.3ds", 20);
    squirrelModel.setRotation(0, 90, 0, 1, 0);
    squirrelModel.setScale(0.9, 0.9, 0.9);
    squirrelModel.setPosition(0, 0, 0);	

	camWidth 		= ofGetWidth();	
	camHeight 		= ofGetHeight();
	
	ptam.initPTAM(camWidth,camHeight);
	
	myFBO.allocate(camWidth,camHeight);
	myFBO2.allocate(camWidth,camHeight);
	myFBO3.allocate(camWidth,camHeight);
	
	grabber.setVerbose(true);
	grabber.listDevices();
	grabber.setDeviceID(3);
	grabber.initGrabber(camWidth,camHeight);
}

void testApp::update(){

	grabber.grabFrame();
	
	ptam.updatePTAM(grabber.getPixels());
	
	//draw map centered obj
	myFBO.clear();
	myFBO.begin();
	if (ptam.bMapBuildComplete) {
		ptam.beginAR();
			glScaled(PTAM_SCALE, PTAM_SCALE, PTAM_SCALE);
			
			for(int i = 0; i < strokes.size(); i++){
				strokes[i].draw(false);
			}			

		ptam.endAR();
	}
	myFBO.end();	
	
	// draw camera center (may be calibraition false)
	myFBO2.clear();
	myFBO2.begin();
	if (ptam.bMapBuildComplete) {
		ptam.beginAR();
			for (int i = 0; i < campos.size(); i++) {
				ofSetColor(255,0,0);
				glPushMatrix();
					glTranslated(campos[i]->x,campos[i]->y,campos[i]->z);
					glScaled(PTAM_SCALE, PTAM_SCALE, PTAM_SCALE);
					ofCircle(0,0,40);
				glPopMatrix();
				ofSetColor(255,255,255);
			}
			
		ptam.endAR();
	}
	myFBO2.end();
	
	
	// draw targets
	myFBO3.clear();
	myFBO3.begin();
	if (ptam.bMapBuildComplete) {
	
		ptam.beginAR();
		glBegin(GL_LINE_STRIP);
			for (int i = 0; i < targets.size(); i++) {
				//cout << targets[i]->level << endl;
				if (targets[i]->level != 0) {
					ofSetColor(255/targets[i]->level, 255/targets[i]->level, 255/targets[i]->level);
				}else {
					ofSetColor(255, 255, 255);
				}
				
				glVertex3d(targets[i]->x,targets[i]->y,targets[i]->z);
			}
			ofSetColor(255, 255, 255);
		glEnd();
		ptam.endAR();
	}
	myFBO3.end();
}

void testApp::draw(){

	//ptam.drawImg();
	grabber.draw(0,0);
	ptam.drawTrail();	
	
	myFBO.draw(0,0);
	myFBO2.draw(0,0);
	myFBO3.draw(0,0);
}


void testApp::keyPressed(int key){
	switch (key){
		case ' ':
			ptam.startBuildMap();
			break;
		case 'r':
			ptam.resetMap();
			cout << "clear map..." << endl;
			targets.clear();
			cout << "clear targets..." << endl;
			campos.clear();
			cout << "clear camera position..." << endl;
			strokes.clear();
			cout << "clear strokes..." << endl;
			break;
		case 'c':
			if (ptam.bMapBuildComplete) {
				cout << "get current camera posistion..." << endl;
				screenCoord *scoord = ptam.getCurrentCamPos(5);
				campos.push_back(scoord);
			}else {
				cout << "no map created yet!" << endl;
			}
			break;
		case 't':
			if (ptam.bMapBuildComplete) {
				cout << "clear all cached targets..." << endl;
				targets.clear();
				for (int i = 0; i < 100; i++) {
					targets.push_back(ptam.getRndmTargetsPos());
				}
				cout << "get 100 random targets." << endl;
			}else {
				cout << "no map created yet!" << endl;
			}
			break;
		case '0':
			whichMode = 0;
			break;
		case '1':
			whichMode = 1;
			break;
		case '2':
			whichMode = 2;
			break;
		case '3':
			whichMode = 3;
			break;

	}
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){

	double xd = ptam.mapScrnX(x);
	double yd = ptam.mapScrnY(y); 

	int which = -1;
	for(int i = 0; i < strokes.size(); i++){
		if( !strokes[i].closed ){
			which = i; break;
		}
	}
	
	if( which != -1 ){
		
		stroke & s = strokes[which];
		
		if( s.pts.size() ){
			
			float dx = xd-s.pts.back().x;
			float dy = yd-s.pts.back().y;
			float len = sqrt(dx*dx + dy*dy);
			
			if(len >= 2.0){
				ofPoint tmp(xd, yd);
				tmp *= 0.3;
				tmp += ( s.pts.back() ) * 0.7;
				s.pts.push_back(tmp);
			}		
		}else{
			s.pts.push_back(ofPoint(xd, yd));	
			s.lNormal.push_back(ofPoint(0,0));
			s.rNormal.push_back(ofPoint(0,0));	
		}
		
//		if( (int)ofRandom(0, 5) == 3 ){
//			
//
//			int orient = 0;
//
//			s.particles.push_back( particle(s.pts.back(), ofRandom(1, 1.5), ofRandom(0.45, 1.3), orient) );
//		}
		
		if( s.pts.size() > 1){
			ofPoint diff = s.pts.back() - s.pts[s.pts.size()-2];
			s.lNormal.push_back( ofPoint( -diff.y, diff.x) );
			s.rNormal.push_back( ofPoint( diff.y, -diff.x) );
			
			s.lNormal.back() = (s.lNormal.back()) * 0.3 + s.lNormal[s.lNormal.size()-2] * 0.7;	
			s.rNormal.back() = (s.rNormal.back()) * 0.3 + s.rNormal[s.rNormal.size()-2] * 0.7;
			
			s.lNormal.back() *= 0.73;
			s.rNormal.back() *= 0.73;
			
		}
	}

}
void testApp::mousePressed(int x, int y, int button) {
	
	double xd = ptam.mapScrnX(x);
	double yd = ptam.mapScrnY(y); 
	
	if(xd < ptam.mapScrnX(20) && yd > ptam.mapScrnY(460) ){
		whichMode++;
		if( whichMode >= 4 )whichMode = 0;
		bClear = true;
		return;
	}

	bFirst = false;
	
	if( strokes.size() ){		
		if( strokes.size() > 50 ){
			strokes.erase(strokes.begin(), strokes.begin()+1);
		}
	}
	
	int r, g, b;
	if( whichMode == 0 ){
		r = 255;
		g = 0;
		b = 255;
	}else if( whichMode == 1){
		r = 255;
		g = 255;
		b = 255;
	}else if( whichMode == 2){
		r = 0;
		g = 0;
		b = 0;
	}else{
		r = 255;
		g = 0;
		b = 0;
	}
	
	strokes.push_back(stroke());
	strokes.back().r = r;
	strokes.back().g = g;
	strokes.back().b = b;
	strokes.back().closed = false;

}

void testApp::mouseReleased(){
	printf("frameRate: %.3f, frameNum: %i\n", ofGetFrameRate(), ofGetFrameNum());
	int which = -1;
	for(int i = 0; i < strokes.size(); i++){
		if( !strokes[i].closed ){
			which = i; break;
		}
	}
	
	if( which >= 0 ){	
		strokes[which].closed = true;
		if( strokes[which].pts.size() > 10 ){
			float amnt = 1.0;
			for(int i = strokes[which].pts.size()-5; i < strokes[which].pts.size(); i++){
				strokes[which].lNormal[i] *= amnt;
				strokes[which].rNormal[i] *= amnt;
				if( amnt > 0)amnt -= 0.25;
			}
		}
									
	}	
}

void testApp::mouseReleased(int x, int y, int button){
	mousePos = ofPoint(x, y);
	cout << ptam.mapScrnX(mousePos.x) << endl;
}
void testApp::windowResized(int w, int h){}

