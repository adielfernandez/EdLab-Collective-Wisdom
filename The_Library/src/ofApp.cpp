#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    
    
    //----------Scene Setup----------
    
    currentView = 0;
    
    wallpaper.setup();
    
    
    
    //----------Camera Setup----------
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    
    wallpaper.update();
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();
    
    if(currentView == 0){
        
        ofBackground(0);
        
        wallpaper.draw();
        
        
        
        
    } else if(currentView == 1){
        
        
    } else if(currentView == 2){
        
        
    } else if(currentView == 3){
        
        
    } else if(currentView == 4){
        
        
    }
    
    

    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == OF_KEY_LEFT){
        wallpaper.applyEffectToAll(Tile::FLIP_OUT);
    } else if(key == OF_KEY_RIGHT){
        wallpaper.applyEffectToAll(Tile::FLIP_IN);
    } else if(key == OF_KEY_UP){
        wallpaper.applyEffectToAll(Tile::FLIP_TRANSITION_HORIZ);
    } else if(key == OF_KEY_DOWN){
        wallpaper.applyEffectToAll(Tile::FLIP_TRANSITION_VERT);
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

    wallpaper.triggerWave(ofVec2f(x, y));
    
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
