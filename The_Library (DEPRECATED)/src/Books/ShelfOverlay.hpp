//
//  ShelfOverlay.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 12/28/16.
//
//

#ifndef ShelfOverlay_hpp
#define ShelfOverlay_hpp

#include <stdio.h>
#pragma once

#include "ofMain.h"
#include "ofxEasing.h"


#endif /* ShelfOverlay_hpp */

class ShelfOverlay{
    
public:
    
    ofVec2f pos;
    float width, height;
    
    float trans = 0.0;
    float maxTrans = 255;
    float currentHeight = 0;
    

    
    bool bIsActive = false;
    bool bFadeOut = false;
    bool bIsInUse = false;
    
    //how long to wait after trigger to start action
    //in seconds
    float openDelay = 0.5f;
    float openDuration = 2.0f;
    float closeDelay = 0.5f;
    float closeDuration = 1.0f;
    
    double triggerTime;
    
    void activate(){
        bIsActive = true;
        bIsInUse = true;
        triggerTime = ofGetElapsedTimef();
        
    }
    
    void deactivate(){
        bIsActive = false;
        triggerTime = ofGetElapsedTimef();
    }
    
    void update(){
    
        if(bIsActive){
            //wait a second before bringing the overlay down
            
            if(ofGetElapsedTimeMillis() - triggerTime > openDelay){
                
                double now = ofGetElapsedTimef() - closeDelay;
                
                float pct = ofxeasing::map_clamp(now, triggerTime, triggerTime + openDuration, 0.0, 1.0, &ofxeasing::cubic::easeIn);
                
                currentHeight = ofMap(pct, 0.0f, 1.0f, 0.0f, height);
                
                //set to true so to be ready for fade out
                //when we become inactive
                bFadeOut = true;
            }
        } else {
            
            if(ofGetElapsedTimeMillis() - triggerTime > closeDelay){

                if(bFadeOut){
                    double now = ofGetElapsedTimef() - closeDelay;
                    
                    float pct = ofxeasing::map_clamp(now, triggerTime, triggerTime + closeDuration, 0.0f, 1.0f, &ofxeasing::cubic::easeOut);
                    
                    currentHeight = ofMap(pct, 0.0f, 1.0f, height, 0.0f);
                    
                    if(pct == 1.0f){
                        bFadeOut = false;
                        
                        //done fading, mark bookshelf as available
                        bIsInUse = false;
                    }
                }
            }
        }
    }
    
    void draw(){
        
        if(bIsActive || bFadeOut){
            ofSetColor(0);
            ofDrawRectangle(pos.x, pos.y, -56, width, currentHeight);
        }
        
    }
    
    
};

