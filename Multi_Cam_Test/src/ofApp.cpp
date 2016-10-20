#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    ofSetVerticalSync(false);
    
    ofSetBackgroundColor(50);
    
    //----------CAMERAS----------
    leftCam.setup("LeftCam", "device/sensor0");
    rightCam.setup("RightCam", "device/sensor1");
    centerCam.setup("CenterCam", "device/sensor2");
    
    numCams = 2;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    threshold = (int)ofMap(mouseX, 0, ofGetWidth(), 0, 255, true);
    
    vector<int> settings;
    settings.push_back(threshold);
    
    //----------CAMERAS----------
    leftCam.update();
    rightCam.update();
    centerCam.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    string s;
    s += "Framerate: " + ofToString(ofGetFrameRate()) + "\n";
    s += "Current Camera: " + ofToString(currentCam) + "\n";
    

    ofVec2f guiPos(15, 60);
    ofVec2f frame1Pos(240, guiPos.y);
    ofVec2f frame2Pos(frame1Pos.x + 640 + 20, guiPos.y);
    
    
    //the image to be filled by the camera
    ofImage rawImg, threshImg;
    ofSetColor(255);
    
    if(currentCam == 0) {
        
        if(leftCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        leftCam.drawGui(guiPos.x, guiPos.y);
        
        rawImg.setFromPixels(leftCam.rawPix);
        rawImg.draw(frame1Pos);
        
        threshImg.setFromPixels(leftCam.threshPix);
        threshImg.draw(frame2Pos);
        
        s += "Camera Framerate: " + ofToString(leftCam.camFrameRate) + "\n";
        
    } else if(currentCam ==1){
        
        if(rightCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        
        rightCam.drawGui(guiPos.x, guiPos.y);
        
        rawImg.setFromPixels(rightCam.rawPix);
        rawImg.draw(frame1Pos);
        
        threshImg.setFromPixels(rightCam.threshPix);
        threshImg.draw(frame2Pos);
        
        s += "Camera Framerate: " + ofToString(rightCam.camFrameRate) + "\n";
        
    } else if(currentCam == 2){
        
        if(centerCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        centerCam.drawGui(guiPos.x, guiPos.y);
        
        rawImg.setFromPixels(centerCam.rawPix);
        rawImg.draw(frame1Pos);
        
        threshImg.setFromPixels(centerCam.threshPix);
        threshImg.draw(frame2Pos);
        
        s += "Camera Framerate: " + ofToString(centerCam.camFrameRate) + "\n";
        
    }
    
    
    

    
    
    ofSetColor(255);
    ofDrawBitmapString(s, 10, 15);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == OF_KEY_RIGHT){
        currentCam++;
        
        if(currentCam > numCams) currentCam = 0;
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
