#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    ofSetBackgroundColor(50);
    
    //----------CAMERAS----------
    leftCam.setup("LeftCam", "device/sensor0");
    rightCam.setup("RightCam", "device/sensor1");
    centerCam.setup("CenterCam", "device/sensor2");
    
    numCams = 3;
    currentCam = 0;
    

    
}

//--------------------------------------------------------------
void ofApp::update(){

    
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

    ofSetColor(255);

    
    
    if(currentCam == 0 || currentCam == 1) {
        
//        WallCam *thisCam = &leftCam;
        WallCam *thisCam = (currentCam == 0) ? &leftCam : &rightCam;
        
        if(thisCam -> isThreadRunning()){
            ofBackground(50);
        } else {
            ofBackground(200, 0, 0);
        }
        
        thisCam -> drawGui(guiPos.x, guiPos.y);
        thisCam -> drawRaw(frame2Pos.x, frame2Pos.y);
        thisCam -> drawThresh(frame2Pos.x, frame2Pos.y + thisCam -> camHeight + 10, false);
        
        
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
        
        
        
        
        
        
        s += "Camera Framerate: " + ofToString(thisCam -> camFrameRate) + "\n";
        
        
        
        //draw touch data
        for(int i = 0; i < thisCam -> touches.size(); i++){
            
            ofVec2f p = thisCam -> touches[i].pos;
            float rad = ofMap(thisCam -> touches[i].dist, 0, 70, 7, 50, true);
            
            if(thisCam -> touches[i].bIsTouching){
                ofSetColor(0, 255, 0);
                
            } else {
                ofSetColor(0, 128, 255);
            }
            
            ofSetLineWidth(3);
            ofNoFill();
            ofDrawCircle(p, rad);
            
        }

        
        
        
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
