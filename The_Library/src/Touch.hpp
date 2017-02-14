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
    
    ofVec2f pos, mappedPos;
    
    
    int ID;
    float dist;
    bool bIsTouching;
    
    double birthTime = 0;
    double age;
    
    void update(){
        age = ofGetElapsedTimef() - birthTime;
    }
    
};
    