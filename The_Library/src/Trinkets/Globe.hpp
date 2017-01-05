//
//  Globe.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/4/17.
//
//

#ifndef Globe_hpp
#define Globe_hpp

#include <stdio.h>

#endif /* Globe_hpp */

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#pragma once


class Globe{
    
public:
    
    Globe();
    
    void setup(ofVec3f shelfPos);
    void update();
    void draw();
    
    
    
    
    ofxAssimpModelLoader model;
    
    float modelScale;
    
    ofTexture tex;
    ofFbo fbo;
    
    ofVec3f pos;
    
};