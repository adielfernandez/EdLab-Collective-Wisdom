#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    ofSetVerticalSync(false);
    
    ofSetBackgroundColor(50);
    
    //----------CAMERAS----------
//    leftCam.setup("LeftCam", "device/sensor0");
//    rightCam.setup("RightCam", "device/sensor1");
    centerCam.setup("CenterCam", "device/sensor0");
    
    numCams = 3;
    currentCam = 2;
    

    font.load("fonts/Aller_Rg.ttf", 50);
    
    
    oscIP = "192.168.1.6";
    oscPort = 12345;
    oscSender.setup(oscIP, oscPort);
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    //----------CAMERAS----------
//    leftCam.update();
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

    

    
    
    if(currentCam == 0 || currentCam == 1) {
        
//        WallCam *thisCam = currentCam == 0 ? &leftCam : &rightCam;
////        WallCam *thisCam = &leftCam;
//        
//        
//        if(thisCam -> isThreadRunning()){
//            ofBackgroundGradient(ofColor(80), ofColor(0));
//        } else {
//            ofBackground(200, 0, 0);
//        }
//        
//        
//        string title;
//        if(currentCam == 0){
//            title = "Left Wall Cam";
//        } else {
//            title = "Right Wall Cam";
//        }
//        
//        ofSetColor(255);
//        font.drawString(title, frame1Pos.x, font.stringHeight("A") + 10);
//        
//        
//        s += "Camera Framerate: " + ofToString(thisCam -> camFrameRate) + "\n";
//        
//        
//        thisCam -> drawGui(guiPos.x, guiPos.y);
//        thisCam -> drawRaw(frame2Pos.x, frame2Pos.y);
//        thisCam -> drawThresh(frame2Pos.x, frame2Pos.y + thisCam -> camHeight + 30, false);
//        thisCam -> drawSceneProxy(frame1Pos.x + 20, frame1Pos.y);
        
        
        
        
    } else if(currentCam == 2) {
    
        
        if(centerCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        centerCam.drawGui(guiPos.x, guiPos.y);
        centerCam.drawRaw(frame1Pos.x, frame1Pos.y);
        centerCam.drawThresh(frame2Pos.x, frame2Pos.y);

        
        
        s += "Camera Framerate: " + ofToString(centerCam.camFrameRate) + "\n";
        
        
        
        //draw touch data
        for(int i = 0; i < centerCam.touches.size(); i++){
            
            ofVec2f p = centerCam.touches[i].pos;
            float rad = ofMap(centerCam.touches[i].dist, 0, 70, 7, 50, true);
            
            if(centerCam.touches[i].bIsTouching){
                ofSetColor(0, 255, 0);
                
            } else {
                ofSetColor(0, 128, 255);
            }
            
            ofSetLineWidth(3);
            ofNoFill();
            ofDrawCircle(p, rad);
            
        }

        
        
        
        
        
        
        
    }
    
        
    
    //go through the cameras and see if they have any data to send
//    
//    if(leftCam.bNewTouchesToSend){
//        
//        //prepare a bundle to send
//        ofxOscBundle bundle;
//        
//        //then add a new message for each touch to the bundle
//        for(int i = 0; i < leftCam.touches.size(); i++){
//
//            //gather the data
//            
//            int id = leftCam.touches[i].id;
//            float x = leftCam.touches[i].pos.x;
//            float y = leftCam.touches[i].pos.y;
//            float dist = leftCam.touches[i].dist;
//            bool touching = leftCam.touches[i].bIsTouching;
//            
//            ofxOscMessage m;
//            
//            m.setAddress("/LeftCam/touch");
//            m.addIntArg(id);
//            m.addFloatArg(x);
//            m.addFloatArg(y);
//            m.addFloatArg(dist);
//            m.addIntArg(touching);
//            
//            bundle.addMessage(m);
//            
//        }
//        
//        oscSender.sendBundle(bundle);
//        leftCam.bNewTouchesToSend = false;
//        
//        cout << "Sent bundle for left cam" << endl;
//        
//    }
    
    
    
//    if(mouseX < frame2Pos.x){
//        ofSetColor(255);
//        ofDrawBitmapStringHighlight(ofToString(mouseX) + ", " + ofToString(mouseY), mouseX + 10, mouseY - 10);
//    }
    
    ofSetColor(255);
    ofDrawBitmapString(s, 10, 15);
    
    
}

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
