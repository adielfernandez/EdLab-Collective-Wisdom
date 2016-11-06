#pragma once

#include "ofMain.h"
#include "Wallpaper.hpp"

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
	
    /*
     -----View Control-----
     0 = Main interaction view
     1 = Mapping Mode
     2 = Camera 1 debug
     3 = Camera 2 debug
     4 = Camera 3 debug
     
    */
    int currentView;
    
    
    
    
    Wallpaper wallpaper;
    
    
    
    
    
};
