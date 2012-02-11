#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxPTAM.h"
#include "ofxFBOTexture.h"
#include "ofx3DModelLoader.h"

#include "stroke.cpp"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
			
		ofxPTAM					ptam;
		int						camWidth;
		int						camHeight;	
		
		ofVideoGrabber			grabber;
		
		ofxFBOTexture			myFBO;
		ofx3DModelLoader		squirrelModel;
		
		ofxFBOTexture			myFBO2;
		vector<screenCoord*>	campos;
		
		ofxFBOTexture			myFBO3;
		vector<screenCoord*>	targets;
		
		ofPoint					mousePos;
		
		vector<stroke>			strokes;
		bool bFirst;
		bool reverse;
		bool bClear;
		int whichMode;
		int orientation;		
};

#endif
