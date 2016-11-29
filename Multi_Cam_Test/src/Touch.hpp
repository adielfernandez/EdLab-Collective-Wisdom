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
    
    void setTouchData(ofVec2f _p, float _d);
    
    //position of object in wall space
    ofVec2f pos;
    
    //how far from the wall it is
    float dist;
    
    //blob ID
    //Maybe use for swipe gesture later
    int id;
    
    
};