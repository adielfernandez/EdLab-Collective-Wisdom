#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "Wallpaper.hpp"
#include "Frame.hpp"
#include "Bookcase.hpp"

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
     2 = Camera 1 debug
     3 = Camera 2 debug
     4 = Camera 3 debug
     
    */
    int currentView;
    int numViews;
    
    
    bool bShowGUIs;
    
    Wallpaper wallpaper;
    
    Frame frame;
    
    Bookcase leftBookcase;
    Bookcase rightBookcase;
    


    // WebSocket Connection
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
    ofxLibwebsockets::Client client;

    unsigned long long lastHeartbeatTime;
    int heartbeatInterval;
    
};
