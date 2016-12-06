//
//  Touch.hpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#ifndef Touch_hpp
#define Touch_hpp

#include <stdio.h>

#endif /* Touch_hpp */

#include "ofMain.h"

#pragma once

class Touch{
    
public:
    
    Touch();
    
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
    
    //we'll use this flag to remove
    //touches that haven't been update
    bool bUpdated;
    
    //anything less than this is considered
    //a touch on wall
    int distForTouch;
    
    bool bIsTouching;
    
    
};