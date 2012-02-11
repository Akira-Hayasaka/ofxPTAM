/*
 *  stroke.cpp
 *  try_PTAM11_stroke
 *
 *  Created by Akira Asia on 10/04/20.
 *  Copyright 2010 This is Ampontang. No rights reserved.
 *
 */


#include "ofMain.h"

class particle{
	public:
	
	particle(ofPoint startPos, float _size, float _speed, int orient = 0){
		pos = sPos = startPos;
		size = _size;
		speed = _speed;
		orientation = orient;
		len = speed * 470;
	}
	
	void update(){
		if( curLen > len )return;
		
		curLen += speed;
		
		if( len - curLen < 30 ){
			speed *= 0.973;
		}

		if( pos.x > 0 && orientation == 1 ){
			pos.x -= speed;
		}
		if( pos.x < 320 && orientation == 3 ){
			pos.x += speed;
		}
		if( pos.y < 480 && orientation == 0 ){
			pos.y += speed;
		}
		if( pos.x > 0 && orientation == 2 ){
			pos.y -= speed;
		}		
	}
	
	void draw(){
		ofPushStyle();
		ofFill();
		
		if( orientation == 1 || orientation == 3){
			ofRect(sPos.x, sPos.y-size, pos.x - sPos.x, 2);
			ofCircle(pos.x, pos.y-0.5, size);
		}else{
			ofRect(sPos.x-size, sPos.y, 2, pos.y - sPos.y);
			ofCircle(pos.x-0.5, pos.y, size);
		}
		ofPopStyle();
	}
	
	float len;
	float curLen;
	ofPoint sPos;
	ofPoint pos;
	float speed;
	float size;
	vector <ofPoint> pts; 
	int orientation;
};


class stroke{
public:
	
//	void update(){
//		for(int i = 0; i < particles.size(); i++){
//			particles[i].update();
//		}
//	}

	void draw(bool reverse){
		if( reverse ) ofSetColor(255 - r, 255 - g, 255 - b);
		else ofSetColor(r, g, b);

//		for(int i = 0; i < particles.size(); i++){
//			particles[i].draw();
//		}
		
		int drawTo = MIN(pts.size(), MIN(lNormal.size(), rNormal.size()));
		
		if(drawTo < 5)return;
		
		int numToDraw = pts.size() * 2;
		GLfloat ptsToDraw[numToDraw*2];
		
			int p = 0;
			for(int j =0; j < drawTo; j++){
//				ptsToDraw[p]	= pts[j].x + lNormal[j].x; 
//				ptsToDraw[p+1]	= pts[j].y + lNormal[j].y;
				ptsToDraw[p]	= pts[j].x + lNormal[j].x/2; 
				ptsToDraw[p+1]	= pts[j].y + lNormal[j].y/2;				
			
				p+=2;
//				ptsToDraw[p]	= pts[j].x + rNormal[j].x; 
//				ptsToDraw[p+1]	= pts[j].y + rNormal[j].y;
				ptsToDraw[p]	= pts[j].x + rNormal[j].x/2; 
				ptsToDraw[p+1]	= pts[j].y + rNormal[j].y/2;	
							
				p+=2;
			}
	
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, ptsToDraw);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, numToDraw);		
			glDisableClientState(GL_VERTEX_ARRAY);
		
		ofSetColor(255, 255, 255);
	}
	
	int id; 
	
	vector <ofPoint> pts;
	vector <ofPoint> lNormal;
	vector <ofPoint> rNormal;
	
	bool closed;
	
	float r,  g,  b;

	vector <particle> particles; 
};
