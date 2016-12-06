#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(200);
    ofSetVerticalSync(false);
    
    ofSetBackgroundColor(50);
    
    //----------CAMERAS----------
    leftCam.setup("LeftCam", "device/sensor0");
//    rightCam.setup("RightCam", "device/sensor1");
//    centerCam.setup("CenterCam", "device/sensor2");
    
    numCams = 1;
    
    sound.load("pop.mp3");
    
}

//--------------------------------------------------------------
void ofApp::update(){

    threshold = (int)ofMap(mouseX, 0, ofGetWidth(), 0, 255, true);
    
    vector<int> settings;
    settings.push_back(threshold);
    
    //----------CAMERAS----------
    leftCam.update();
//    rightCam.update();
//    centerCam.update();
    
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

    ofSetColor(255);
    
    if(currentCam == 0) {
        
        if(leftCam.isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        leftCam.drawGui(guiPos.x, guiPos.y);
        leftCam.drawRaw(frame2Pos.x, frame2Pos.y);
        leftCam.drawThresh(frame2Pos.x, frame2Pos.y + leftCam.camHeight + 10, false);
        
        
        //drawing on points overlapping on the space
        vector<ofPoint> ps;
        ps.resize(6);
        ps[0] = ofPoint(273, 90);
        ps[1] = ofPoint(854, 90);
        ps[2] = ofPoint(853, 397);
        ps[3] = ofPoint(851, 703);
        ps[4] = ofPoint(273, 700);
        ps[5] = ofPoint(273, 396);
        
        
        ofPolyline pLine;
        pLine.addVertices(ps);
        pLine.close();
        
        ofSetColor(255, 200, 0);
        ofSetLineWidth(1);
        pLine.draw();
        
        //draw the points too
        for(int i = 0; i < ps.size();i++){
            ofFill();
            ofDrawCircle(ps[i].x, ps[i].y, 7);
        }
        
        
        //draw touch data
        for(int i = 0; i < leftCam.touches.size(); i++){
         
            ofVec2f p = leftCam.touches[i].pos;
            float rad = ofMap(leftCam.touches[i].dist, 0, 70, 7, 50, true);
            
            if(leftCam.touches[i].bIsTouching){
                ofSetColor(0, 255, 0);
                
                if(!sound.isPlaying()){
                    sound.play();
                }
                
            } else {
                ofSetColor(0, 128, 255);
            }
            
            ofSetLineWidth(3);
            ofNoFill();
            ofDrawCircle(p, rad);
            
        }
        
        
        
        
        s += "Camera Framerate: " + ofToString(leftCam.camFrameRate) + "\n";
        
    }
    
    if(mouseX < frame2Pos.x){
        ofSetColor(255);
        ofDrawBitmapStringHighlight(ofToString(mouseX) + ", " + ofToString(mouseY), mouseX + 10, mouseY - 10);
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
