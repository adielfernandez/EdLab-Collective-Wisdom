//
//  SceneController.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 2/13/17.
//
//

#include "SceneController.hpp"


SceneController::SceneController(){
    
}

void SceneController::setBookcaseRefs(Bookcase *left, Bookcase *right){
    leftBookcase = left;
    rightBookcase = right;
}

void SceneController::setFrameRef(Frame *fr){
    frame = fr;
}

void SceneController::setWallpaperRef(Wallpaper *paper){
    wallpaper = paper;
}

void SceneController::setCenterBookRef(CenterBook *book){
    centerBook = book;
}

void SceneController::setIdleTimerRef( double * idle ){
    idleTimer = idle;
}

void SceneController::setTextureCombo(int num){

    //Number order in textureCombos vector:
    //WALLPAPER - BOOKCASE - FRAME
    triggerAllWave( ofGetWidth()/2, ofGetHeight()/2, num);
    
}

void SceneController::triggerAllWave(int x, int y, int num){
    
    ofVec2f p( x, y );
    
    wallpaper -> triggerWave( textureCombos[num][0], p );
    leftBookcase -> triggerWave( textureCombos[num][1], p );
    rightBookcase -> triggerWave( textureCombos[num][1], p );
    frame -> triggerWave( textureCombos[num][2], p );
    
}

void SceneController::setup(){

    //for making randomized texture changes
    lastChangeTime = 0;
    
    //how long to wait between idle changes
    idleTriggerWaitTime = 30.0f;
    
    //how long to wait after a change to unlock the scene
    lockWaitTime = 6.0f;
    
    //pre-decided combinations of textures that look good
    
    //Number order:
    //WALLPAPER - BOOKCASE - FRAME
    textureCombos = {   { 0, 10,  3},       //0
                        { 1,  9,  1},       //1
                        { 2,  6,  4},       //2
                        { 3, 10,  9},       //3
                        { 4,  0,  8},       //4
                        { 5,  7,  0},       //5
                        { 6, 12,  7},       //6
                        { 7,  7,  5},       //7
                        { 8, 12,  6},       //8
                        { 9,  2,  2} };     //9
    
    //start app with this combo
    setTextureCombo(6);
    
}

void SceneController::update(){
    
    
    if( ofGetElapsedTimeMillis() - lastChangeTime < lockWaitTime){
        
        lockScene = true;
        
    } else {
        lockScene = false;
    }
    
    
    
    //Trigger an animation if the system is idle AND if it's been
    //long enough since the last one and the scene isn't locked
    
    if( !lockScene &&  *idleTimer > 300.0f && ofGetElapsedTimef() - lastChangeTime > idleTriggerWaitTime){
        
        float whichObject = ofRandom(3);
        
        int newTexture = floor(ofRandom(10));
        
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        
        if(whichObject < 1.0){
            
            //BOOKCASES
            leftBookcase -> triggerWave( newTexture, ofVec2f(leftBookcase -> bookcaseCorners[0].x + (leftBookcase -> bookcaseCorners[1].x - leftBookcase -> bookcaseCorners[0].x)/2.0 , y) );
            
            rightBookcase -> triggerWave( newTexture, ofVec2f(rightBookcase -> bookcaseCorners[0].x + (rightBookcase -> bookcaseCorners[1].x - rightBookcase -> bookcaseCorners[0].x)/2.0, y));
            
        } else if(whichObject < 2.0){

            //FRAME
            frame -> triggerWave( newTexture, ofVec2f(x, y));
            
        } else {
            
            //WALLPAPER
            wallpaper -> triggerWave( newTexture, ofVec2f(x, y));
        }
        
        idleTriggerWaitTime = ofRandom(30.0f, 60.0f);
        lastChangeTime = ofGetElapsedTimef();
        
    }
    
    
    
    
    
    
    
}


void SceneController::onRedecorateEvent( SceneEvent &se ){
    
    //position of wave epicenter
    ofVec2f p;
    
    if( !lockScene ){

        if(se.type == SceneEvent::CENTERBOOK){
            
            int randTex = floor( ofRandom( textureCombos.size() ) );
            
            //position of button to trigger event
            p.set( se.pos.x, se.pos.y);
            
            wallpaper -> triggerWave( textureCombos[randTex][0], p );
            
            //trigger left case from left
            p.set( 0, se.pos.y );
            leftBookcase -> triggerWave( textureCombos[randTex][1], p );
            
            //right case from right
            p.set( ofGetWidth(), se.pos.y );
            rightBookcase -> triggerWave( textureCombos[randTex][1], p );
            
            //frame from above
            p.set( se.pos.x, 0 );
            frame -> triggerWave( textureCombos[randTex][2], p );
            
        } else if(se.type == SceneEvent::BOOKCASE_LEFT || se.type == SceneEvent::BOOKCASE_RIGHT){
            
            //trigger from event center
            p.set( se.pos.x, se.pos.y );
            
            int newTex = floor( ofRandom(leftBookcase -> images.size()) );
            
            leftBookcase -> triggerWave( newTex, p );
            rightBookcase -> triggerWave( newTex, p );
            
        }
        
        lastChangeTime = ofGetElapsedTimef();
        
    }
    
}








