
#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "ofxAssimpModelLoader.h"

#include "Furniture/Wallpaper.hpp"
#include "Furniture/Frame.hpp"
#include "Furniture/Bookcase.hpp"
#include "Books/CenterBook.hpp"

#include "Books/BookController.hpp"
#include "Books/SpawnRibbon.hpp"

#include "Content/Contribution.hpp"
#include "Content/ContributionManager.hpp"
#include "Content/ScholarData.hpp"

#include "ofxOsc.h"
#include "Touch.hpp"

#include "SceneController.hpp"

#pragma once

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
     1 = Wallpaper and Frame debug
     2 = Bookcases Debug
     3 = CenterBook Debug
     
    */
    int currentView;
    int numViews;
    
    //Interface elements
    ofImage bgEdgeMask;
    
    ofEasyCam camera;
    
    
    bool bShowFrameRate;
    
    //----------Scholar Setup----------
    ScholarData scholarData;
    
    //----------Scene Setup----------
    SceneController sceneController;
    
    bool bIsFullscreen;
    
    Wallpaper wallpaper;
    
    Frame frame;
    
    Bookcase leftBookcase;
    Bookcase rightBookcase;
    
    BookController bookController;
    
    CenterBook centerBook;
    
    //show coordinates of mouse to
    //help place things on screen
    bool bShowMouseCoords;

    //for drawing the saving/loading
    //feedback on screen
    double lastSaveTime, lastLoadTime;
    

    // WebSocket Connection
    bool connectToServer;
    
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );
    
    ofxLibwebsockets::Client client;

    //Heartbeat sending
    unsigned long long lastHeartbeatTime;
    int heartbeatInterval;
    bool bSendHeartbeat;
    
    //-----MESSAGE LOGGING AND RETRIEVAL-----
    ContributionManager contributionManager;
    
    //how long it's been since there's
    //been any activity
    double idleTimer;
    double lastTouchTime;

    
    //DELETE ME
    int messageTagNum = 0;
    
    
    
    //OSC Receiver
    int oscReceivePort;
    ofxOscReceiver oscReceiver;

    
    
};





