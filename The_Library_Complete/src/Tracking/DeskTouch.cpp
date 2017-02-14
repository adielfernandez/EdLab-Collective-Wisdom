//
//  DeskTouch.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 12/15/16.
//
//

#include "DeskTouch.hpp"



DeskTouch::DeskTouch(){
    
}



void DeskTouch::setNewTouch(int _id, ofVec2f _p, float _d){
    
//    numPosSmoothingPts = 10;
//    numDistSmoothingPts = 6;
    
    posHistory.clear();
    distHistory.clear();
    
    id = _id;
    pos = _p;
    dist = _d;
    
    bIsTouching = false;
    
    //set as unreachable until the value is update
    //with the real touch threshold
    distForTouch = -1;
    
    update();
    
}

void DeskTouch::renewTouch(ofVec2f p, float d){
    
    //add this point to the history list
    posHistory.push_back(p);
    distHistory.push_back(d);
    
    //if we have more points than we want to smooth,
    //remove the oldest one
    if(posHistory.size() > numPosSmoothingPts){
        posHistory.pop_front();
    }
    if(distHistory.size() > numDistSmoothingPts){
        distHistory.pop_front();
    }
    
    //Find the average of all the history of points we've stored
    ofVec2f avgPos;
    float avgDist;
    
    for(int i = 0; i < posHistory.size(); i++){
        avgPos += posHistory[i];
    }
    for(int i = 0; i < distHistory.size(); i++){
        avgDist += distHistory[i];
    }
    avgPos /= posHistory.size();
    avgDist /= distHistory.size();
    
    //set current position as the average
    pos = avgPos;
    
    //Do not average the depth, keep that updating quickly
    dist = avgDist;
    
    update();
    

    
}


void DeskTouch::update(){
    
    if(dist < distForTouch)
        bIsTouching = true;
    else
        bIsTouching = false;
    
    bUpdated = true;
    
}

void DeskTouch::draw(){
    
    ofVec3f p = mappedXFlip;

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




