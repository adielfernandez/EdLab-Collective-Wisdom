//
//  DeskTouch.hpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 12/15/16.
//
//

#ifndef DeskTouch_hpp
#define DeskTouch_hpp

#include <stdio.h>

#endif /* DeskTouch_hpp */



#include "ofMain.h"

#pragma once

class DeskTouch{
    
public:
    
    DeskTouch();
    
    void setNewTouch(int _id, ofVec2f _p, float _d);
    void renewTouch(ofVec2f p, float d);
    void update();
    
    //position of object in wall space
    ofVec2f pos;
    
    //how far from the wall it is
    float dist;
    
    //blob ID
    //Maybe use for swipe gesture later
    int id;
    
    
    //number of points we'll use to smooth
    int numPosSmoothingPts;
    int numDistSmoothingPts;
    deque<ofVec2f> posHistory;
    deque<float> distHistory;
    
    //raw touch data
    ofVec2f rawCamPos;
    float rawDepth;
    
    //we'll use this flag to remove
    //touches that haven't been update
    bool bUpdated;
    
    //anything less than this is considered
    //a touch on wall
    int distForTouch;
    
    bool bIsTouching;
    
    
};