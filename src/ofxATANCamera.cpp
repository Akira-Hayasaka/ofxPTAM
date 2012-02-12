/*
 *  ofxATANCamera.cpp
 *  try_PTAM3
 *
 *  Created by Akira Asia on 10/04/09.
 *  Copyright 2010 This is Ampontang. No rights reserved.
 *
 */

#include "ofxATANCamera.h"
#include <fstream>

void ofxATANCamera::manualParamUpdate(string cfgFile) {

	ifstream stream;
	stream.open(ofToDataPath(cfgFile).c_str());
	string params;
	getline(stream, params);
	stream.close();

	unsigned int loc = params.find( "=", 0 );
	params.erase(0, loc+1);
	vector<char*> tmp;
	char *strchar = std::strcpy(new char[params.size() + 1], params.c_str());
	char *tp;
	tp = strtok(strchar, " ");
	tmp.push_back(tp);
	while (tp != NULL) {
			tp = strtok( NULL," " );
			tmp.push_back(tp);
	}
	
	Vector<5> vUpdate;
	for (int i = 0; i < 5; i++) {
		vUpdate[i] = atof(tmp[i+1]);
	}
	
	(*mgvvCameraParams) = (*mgvvCameraParams) + vUpdate;
	RefreshParams();

}

// TODO: do complete Test!
bool ofxATANCamera::paramTest(){
	
	cout << mvFocal[0] << endl;
	if (mvFocal[0] == 320) {
		return false;
	}else {
		return true;
	}
	
}
