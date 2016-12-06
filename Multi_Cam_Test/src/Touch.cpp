//
//  Touch.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#include "Touch.hpp"


Touch::Touch(){
    
}



void Touch::setNewTouch(int _id, ofVec2f _p, float _d){
    
    id = _id;
    pos = _p;
    dist = _d;

    bIsTouching = false;

    //set as unreachable until the value is update
    //with the real touch threshold
    distForTouch = -1;
    
    update();
    
}

void Touch::renewTouch(ofVec2f p, float d){
    
    //add the new position to the last one and average to smooth the data a little
    pos = (pos + p) / 2.0f;
    
    //Do not average the depth, keep that updating quickly
    dist = d;
    
    update();
    
}


void Touch::update(){
    
    if(dist < distForTouch)
        bIsTouching = true;
    else
        bIsTouching = false;
    
    bUpdated = true;
    
}



