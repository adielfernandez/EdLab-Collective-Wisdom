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
    triggerAllWave( 1920/2, 1200/2, num);
    
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
    
    //how long to wait between idle changes, start at
    //zero, it will be given random wait times later
    idleTriggerWaitTime = 0.0f;
    
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
    lastTextureCombo = 6;
    
    //start app with this combo
    setTextureCombo(lastTextureCombo);
    
    titleBanner.setup();
    
    //set up the notification banners by telling them where to be
    ofVec3f leftCasePos = ( leftBookcase -> bookcaseCorners[0] + leftBookcase -> bookcaseCorners[1] )/2.0f;
    ofVec3f rightCasePos = ( rightBookcase -> bookcaseCorners[0] + rightBookcase -> bookcaseCorners[1] )/2.0f;
    
    leftBanner.setup( leftCasePos );
    rightBanner.setup( rightCasePos );
    
}

void SceneController::update(){
    
    titleBanner.update();
    leftBanner.update();
    rightBanner.update();
    
    
    //handle locked booleans
    if( ofGetElapsedTimef() - lastChangeTime < lockWaitTime){
        lockScene = true;
    } else {
        lockScene = false;
    }
    

    //lock the elements if they're still animating

    if(leftBookcase -> bIsAnimating || rightBookcase -> bIsAnimating){
        lockBookcases = true;
    } else {
        lockBookcases = false;
    }
    
    lockWallpaper = wallpaper -> bIsAnimating;
    lockFrame = frame -> bIsAnimating;
    
    
    
    
    
    
    //Trigger an animation if the system is idle AND if it's been
    //long enough since the last one and the scene isn't locked
    if( !lockScene &&  *idleTimer > 30.0f && ofGetElapsedTimef() - lastChangeTime > idleTriggerWaitTime){
        
        int newTexNum;
        
        float x = ofRandom(1920);
        float y = ofRandom(1200);
        
        bool somethingChanged = false;

        //go through each object and trigger them if it's safe to
        
        if( !lockBookcases ){
            
            newTexNum = floor(ofRandom( leftBookcase -> images.size() ));
            
            //BOOKCASES
            leftBookcase -> triggerWave( newTexNum, ofVec2f(leftBookcase -> bookcaseCorners[0].x + (leftBookcase -> bookcaseCorners[1].x - leftBookcase -> bookcaseCorners[0].x)/2.0 , y) );
            
            rightBookcase -> triggerWave( newTexNum, ofVec2f(rightBookcase -> bookcaseCorners[0].x + (rightBookcase -> bookcaseCorners[1].x - rightBookcase -> bookcaseCorners[0].x)/2.0, y));
            
            somethingChanged = true;
            
        }
        
        
        if( !lockFrame ){
            
            newTexNum = floor(ofRandom( frame -> images.size() ));
            
            //FRAME
            frame -> triggerWave( newTexNum, ofVec2f(x, y));
            somethingChanged = true;
            
        }
        
        if( !lockWallpaper ){
            
            newTexNum = floor(ofRandom( wallpaper -> images.size() ));
            
            //WALLPAPER
            wallpaper -> triggerWave( newTexNum, ofVec2f(x, y));
            somethingChanged = true;
            
        }

        
        //if we've done anything this time
        //set the banner and note the time
        if( somethingChanged ){
            
            titleBanner.show();
            
            lastChangeTime = ofGetElapsedTimef();
            idleTriggerWaitTime = ofRandom(60.0f, 120.0f);
            
//            cout << "Triggering idle event, next event in: " << idleTriggerWaitTime << endl;
            
        }
        
    }
    
    
}



void SceneController::drawTitleBanner(){
    titleBanner.draw();
}

void SceneController::drawLeftBanner(){
    leftBanner.draw();
}

void SceneController::drawRightBanner(){
    rightBanner.draw();
}

void SceneController::onBookSpawnEvent( BookSpawnEvent &be ){
    
    
    // Right or left
    if( be.bIsLeftBookcase ){
        
        if( be.type == BookSpawnEvent::ARCHIVE ){
            leftBanner.setArchive();
        } else {
            leftBanner.setNewBook();
        }
        
        leftBanner.show();
        
    } else {
        
        if( be.type == BookSpawnEvent::ARCHIVE ){
            rightBanner.setArchive();
        } else {
            rightBanner.setNewBook();
        }
        
        rightBanner.show();
        
    }



}



void SceneController::onRedecorateEvent( SceneEvent &se ){
    
    //position of wave epicenter
    ofVec2f p;
    
    if( !lockScene ){

        if(se.type == SceneEvent::CENTERBOOK){
            
//            int randTex = floor( ofRandom( textureCombos.size() ) );
            lastTextureCombo++;
            
            if(lastTextureCombo == textureCombos.size()){
                lastTextureCombo = 0;
            }
            
            int combo = lastTextureCombo;
            
            //position of button to trigger event
            p.set( se.pos.x, se.pos.y);
            
            wallpaper -> triggerWave( textureCombos[combo][0], p );
            
            //trigger left case from left
            p.set( 0, se.pos.y );
            leftBookcase -> triggerWave( textureCombos[combo][1], p );
            
            //right case from right
            p.set( 1920, se.pos.y );
            rightBookcase -> triggerWave( textureCombos[combo][1], p );
            
            //frame from above
            p.set( se.pos.x, 0 );
            frame -> triggerWave( textureCombos[combo][2], p );

            
        } else if(se.type == SceneEvent::BOOKCASE_LEFT || se.type == SceneEvent::BOOKCASE_RIGHT){
            
            if( !lockBookcases ){
                
                //trigger from event center
                p.set( se.pos.x, se.pos.y );
                
                int newTex = floor( ofRandom(leftBookcase -> images.size()) );
                
                
                
                //offset the waves to make the gap feel less wide
                //as the wave travels through it
                if( se.type == SceneEvent::BOOKCASE_LEFT ){
                    leftBookcase -> triggerWave( newTex, p );
                    rightBookcase -> triggerWave( newTex, p + ofVec2f(300, 0));
                } else {
                    leftBookcase -> triggerWave( newTex, p + ofVec2f(-300, 0));
                    rightBookcase -> triggerWave( newTex, p );
                }
                
                
            }
            
        } else if(se.type == SceneEvent::WALLPAPER){
            
            if( !lockWallpaper ){
                
                //trigger from event center
                p.set( se.pos.x, se.pos.y );
                
                int newTex = floor( ofRandom(wallpaper -> images.size()) );
                
                wallpaper -> triggerWave( newTex, p );
                
            }
            
        } else if(se.type == SceneEvent::FRAME){
            
            if( !lockFrame ){
                
                //trigger from event center
                //            p.set( se.pos.x, se.pos.y );
                
                //start wave from bottom of frame
                p.set( (frame -> frameCorners[2] + frame -> frameCorners[3])/2.0f );
                
                int newTex = floor( ofRandom(frame -> images.size()) );
                
                frame -> triggerWave( newTex, p );
                
            }
            
            
        }
        
        
        
    }
    
}






