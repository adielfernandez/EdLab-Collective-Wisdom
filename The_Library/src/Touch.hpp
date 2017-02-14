//
//  Touch.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 2/13/17.
//
//

#ifndef Touch_hpp
#define Touch_hpp

#include <stdio.h>

#endif /* Touch_hpp */

#pragma once
#include "ofMain.h"


struct Touch{
    
    ofVec2f pos;
    ofVec2f mappedPos;
    ofVec2f mappedXFlip;
    
    int maxRad = 30;
    int minRad = 4;
    int maxDist = 70;
    
    int ID;
    float dist;
    bool bIsTouching;
    
    double birthTime = 0;
    double age;
    
    bool drawFlippedX = false;
    
    ofColor farColor = ofColor(255);
    ofColor closeColor = ofColor(255, 200, 0);
    
    void update(){
        age = ofGetElapsedTimef() - birthTime;
    }
    
    void draw(){
        
        ofVec3f p;
        
        if( !drawFlippedX ){
            p = mappedPos;
        } else {
            p = mappedXFlip;
        }
                
        float rad = ofMap(dist, 0, maxDist, minRad, maxRad, true);
        
        //touch is green if touching
        //otherwise, lero from white to green-ish
        if(bIsTouching){
            
            //draw a fat X when touching
            ofSetColor(closeColor);
            
            ofSetLineWidth(6);
            
            //width of X
            int w = 20;
            ofDrawLine(p.x - w/2, p.y - w/2, -100, p.x + w/2, p.y + w/2, -100);
            ofDrawLine(p.x - w/2, p.y + w/2, -100, p.x + w/2, p.y - w/2, -100);
            
//            ofDrawCircle(p.x, p.y, -100, rad * 2);

            
        } else {
            
            float colorPct = ofMap(dist, 0, maxDist, 1.0, 0.0, true);
            ofSetColor( farColor.lerp(closeColor, colorPct) );
            
            ofSetLineWidth(4);
            ofNoFill();
            ofDrawCircle(p.x, p.y, -100, rad);
            ofFill();
        }
        
        
    }
    
};




