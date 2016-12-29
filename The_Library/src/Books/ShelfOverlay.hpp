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
    
    //how long to wait after trigger to start action
    //in seconds
    float delay;
    
    bool bIsActive = false;
    bool bFadeOut = false;
    
    double triggerTime;
    
    void activate(){
        bIsActive = true;
        triggerTime = ofGetElapsedTimef();
    }
    void deactivate(){
        bIsActive = false;
        triggerTime = ofGetElapsedTimef();
    }
    
    void update(){
    
        if(bIsActive){
            //wait a second before bringing the overlay down
            delay = 0.5f;
            
            if(ofGetElapsedTimeMillis() - triggerTime > delay){
                
                double now = ofGetElapsedTimef() - delay;
                
                float pct = ofxeasing::map_clamp(now, triggerTime, triggerTime + 1.5f, 0.0, 1.0, &ofxeasing::linear::easeOut);
                
                trans = ofMap(pct, 0.0f, 1.0f, 0.0f, maxTrans);
                
                //set to true so to be ready for fade out
                //when we become inactive
                bFadeOut = true;
            }
        } else {

            delay = 1.0f;
            
            if(ofGetElapsedTimeMillis() - triggerTime > delay){

                if(bFadeOut){
                    double now = ofGetElapsedTimef() - delay;
                    
                    float pct = ofxeasing::map_clamp(now, triggerTime, triggerTime + 1.0f, 0.0f, 1.0f, &ofxeasing::linear::easeOut);
                    
                    trans = ofMap(pct, 0.0f, 1.0f, maxTrans, 0.0f);
                    
                    if(pct == 1.0f){
                        bFadeOut = false;
                    }
                }
            }
        }
    }
    
    void draw(){
        
        if(bIsActive || bFadeOut){
            ofSetColor(0, 0, 0, trans);
            ofDrawRectangle(pos.x, pos.y, -55, width, height);
        }
        
    }
    
    
};

