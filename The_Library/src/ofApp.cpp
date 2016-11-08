#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------Scene Setup----------
    
    currentView = 0;
    
    wallpaper.setup();
    
    frame.setup("frame");

    leftBookcase.setup("leftBookcase", true);
//    rightBookcase.setup("rightBookcase", false);
    //----------Camera Setup----------
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    
    wallpaper.update();
    frame.update();
    leftBookcase.update();
//    rightBookcase.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();
    
    if(currentView == 0){
        
        ofBackground(0);
        
        //push wallpaper back a tiny bit so it's always in the background
//        ofPushMatrix();
//        ofTranslate(0, 0, -1);
//
//        wallpaper.draw();
//        ofPopMatrix();
        
        
        
        ofDisableDepthTest();
        frame.draw();
        
//        leftBookcase.draw();

//        rightBookcase.draw();
//        rightBookcase.drawDebug();
        
        if(bShowGUIs){
            
            frame.drawGui();
            frame.drawDebug();
            
            wallpaper.drawGui();
            
            leftBookcase.drawGui();
            leftBookcase.drawDebug();

//            rightBookcase.drawGui();
            
            
        }
        
        
        ofEnableDepthTest();
        
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
    
    
    if(key == ' '){
        bShowGUIs = !bShowGUIs;
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

    if(button == 2){
        wallpaper.triggerWave(ofVec2f(x, y));
        frame.triggerWave(ofVec2f(x, y));
        leftBookcase.triggerWave(ofVec2f(x, y));
    }
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
