#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    ofSetVerticalSync(false);
    
    ofSetBackgroundColor(50);
    
    //----------CAMERAS----------
    leftCam.setup("LeftCam", "device/sensor0");
//    rightCam.setup("RightCam", "device/sensor1");
    centerCam.setup("CenterCam", "device/sensor1");
    
    numCams = 3;
    currentCam = 2;
    

    font.load("fonts/Aller_Rg.ttf", 50);
    
//    oscIP = "localhost";
    oscIP = "192.168.1.2";
    oscPort = 12345;
    oscSender.setup(oscIP, oscPort);
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    //----------CAMERAS----------
    leftCam.update();
//    rightCam.update();
    centerCam.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    string s;
    s += "Framerate: " + ofToString(ofGetFrameRate()) + "\n";
    s += "Current Camera: " + ofToString(currentCam) + "\n";
    

    //position content on screen
    ofVec2f guiPos(15, 60);
    ofVec2f frame1Pos(260, 100);
    ofVec2f frame2Pos(frame1Pos.x + 640 + 10, 100);
    ofVec2f frame3Pos(frame1Pos.x, frame1Pos.y + 480 + 20);
    

    
    
    if(currentCam == 0 || currentCam == 1) {
        
//        WallCam *thisCam = currentCam == 0 ? &leftCam : &rightCam;
        WallCam *thisCam = &leftCam;
        
        
        if(thisCam -> isThreadRunning()){
            ofBackgroundGradient(ofColor(80), ofColor(0));
        } else {
            ofBackground(200, 0, 0);
        }
        
        
        string title;
        if(currentCam == 0){
            title = "Left Wall Cam";
        } else {
            title = "Right Wall Cam";
        }
        
        ofSetColor(255);
        font.drawString(title, frame1Pos.x, font.stringHeight("A") + 10);
        
        
        s += "Camera Framerate: " + ofToString(thisCam -> camFrameRate) + "\n";
        
        
        thisCam -> drawGui(guiPos.x, guiPos.y);
        thisCam -> drawRaw(frame2Pos.x, frame2Pos.y);
        thisCam -> drawThresh(frame2Pos.x, frame2Pos.y + thisCam -> camHeight + 30, false);
        thisCam -> drawSceneProxy(frame1Pos.x + 20, frame1Pos.y);
        
        
        
        
    } else if(currentCam == 2) {
    
        ofSetColor(255);
        font.drawString("Center Desk Cam", frame1Pos.x, font.stringHeight("A") + 10);
        
        if(centerCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        centerCam.drawGui(guiPos.x, guiPos.y);
        centerCam.drawRaw(frame1Pos.x, frame1Pos.y);
        centerCam.drawThresh(frame2Pos.x, frame2Pos.y);
        centerCam.drawDeskProxy(frame2Pos.x, frame2Pos.y + 480 + 40);
        
        s += "Camera Framerate: " + ofToString(centerCam.camFrameRate) + "\n";
        
        
        
    }
    
        
    
    //go through the cameras and see if they have any data to send
    if(leftCam.bNewTouchesToSend){
        
        //prepare a bundle to send
        ofxOscBundle bundle = getBundleForTouches(leftCam.touches, "/LeftCam/touch");
        
        ofxOscMessage m;
        m.setAddress("/FrameStamp");
        bundle.addMessage(m);
        
        float thisFrameRate = 1.0/( (ofGetElapsedTimeMillis() - lastOSCTime) / 1000.0 );
        
        //average this framerate with the last one to smooth out numbers
        //and get a better reading.
        oscRate = (thisFrameRate + lastOSCRate)/2;
        lastOSCRate = thisFrameRate;
        
        //
        lastOSCTime = ofGetElapsedTimeMillis();
        cout << "OSC Send FrameRate: " << oscRate << endl;
        
        oscSender.sendBundle(bundle);
        
        leftCam.bNewTouchesToSend = false;
        
//        cout << "Sent bundle for left cam" << endl;
        
    }
    
    //go through the cameras and see if they have any data to send
    if(centerCam.bNewTouchesToSend){
        
        //prepare a bundle to send
        ofxOscBundle bundle = getBundleForTouches(centerCam.touches, "/CenterCam/touch");
        
        //send it
        oscSender.sendBundle(bundle);
        
        //mark as sent
        centerCam.bNewTouchesToSend = false;
        
        cout << "Sent bundle for center cam" << endl;
        
    }

    
    ofSetColor(255);
    ofDrawBitmapString(s, 10, 15);
    
    
}


template <class TouchType>
ofxOscBundle ofApp::getBundleForTouches(vector<TouchType> touches, string address){
    
    ofxOscBundle bundle;
    
    for(int i = 0; i < touches.size(); i++){
        
        //gather the data
        
        int id = touches[i].id;
        float x = touches[i].pos.x;
        float y = touches[i].pos.y;
        float dist = touches[i].dist;
        bool touching = touches[i].bIsTouching;
        
        ofxOscMessage m;
        
        m.setAddress(address);
        m.addIntArg(id);
        m.addFloatArg(x);
        m.addFloatArg(y);
        m.addFloatArg(dist);
        m.addIntArg(touching);
        
        bundle.addMessage(m);
        
    }

    return bundle;
    
}

//explicit instantiation of tempated function
template ofxOscBundle ofApp::getBundleForTouches<WallTouch>(vector<WallTouch> touches, string address);
template ofxOscBundle ofApp::getBundleForTouches<DeskTouch>(vector<DeskTouch> touches, string address);


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == OF_KEY_RIGHT){
        currentCam++;
        
        if(currentCam == numCams) currentCam = 0;
    }
    
    if(key == OF_KEY_LEFT){
        currentCam--;
        
        if(currentCam < 0) currentCam = numCams - 1;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
