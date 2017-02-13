#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxOrbbecAstra.h"
#include "WallCam.h"
#include "DeskCam.hpp"
#include "WallTouch.hpp"
#include "DeskTouch.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
    ofTrueTypeFont font;
    
//    WallCam leftCam;
//    WallCam rightCam;
    DeskCam centerCam;
    
    
    int currentCam, numCams;
    int threshold;
    
    
    ofxOscSender oscSender;
    string oscIP;
    int oscPort;
    
    
    
};
