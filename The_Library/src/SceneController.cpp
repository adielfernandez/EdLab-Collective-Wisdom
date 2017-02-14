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
    waitTime = 30000;
    
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
    
    
    
    
    //Trigger an animation if the system is idle AND if it's been
    //long enough since the last one
    
    if( *idleTimer > 10.0f && ofGetElapsedTimef() - lastChangeTime > waitTime){
        
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
        
        waitTime = ofRandom(5000, 10000);
        lastChangeTime = ofGetElapsedTimeMillis();
        
    }
    
    
    
    
    
    
    
}


void SceneController::onRedecorateEvent( bool &e ){
    
    int randTex = floor( ofRandom( textureCombos.size() ) );

    //just at the desk level
    ofVec3f pos( ofGetWidth()/2, 700 );
    
    triggerAllWave( pos.x, pos.y, randTex );
    
}



