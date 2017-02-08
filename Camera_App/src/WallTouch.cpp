//
//  WallTouch.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#include "WallTouch.hpp"


WallTouch::WallTouch(){
    
}



void WallTouch::setNewTouch(int _id, ofVec2f _p, float _d){
    
    numPosSmoothingPts = 4;
    numDistSmoothingPts = 2;
    
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

void WallTouch::renewTouch(ofVec2f p, float d){

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


void WallTouch::update(){
    
    if(dist < distForTouch)
        bIsTouching = true;
    else
        bIsTouching = false;
    
    bUpdated = true;
    
}



