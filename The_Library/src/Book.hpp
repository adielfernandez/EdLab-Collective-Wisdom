//
//  Book.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/15/16.
//
//

#ifndef Book_hpp
#define Book_hpp

#include <stdio.h>

#endif /* Book_hpp */

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#pragma once

class Book{
    
    
public:
    
    Book();
    
    void loadModel(int type);
    void setup(ofTexture *_tex);
    void update();
    void draw();
    
    //Support Methods
    
    //Book type: 0 = short, 1 = medium, 2 = tall
    int bookType;
    
    ofxAssimpModelLoader model;
    
    ofFbo liveTexture;
    
    ofMaterial material;
    
    ofTexture *tex;
    
    ofVec3f pos;
    
    
    //these are the actual min/max
    //dimensions of the closed book
    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;
    
    
    //Scaling of model through
    //AssImp methods
    float modelScale;
    
    float thickness, depth, height;
    float realMaxX;
    float realMinX;
    float realMaxY;
    float realMinY;
    float realMaxZ;
    float realMinZ;
    
    
    
};



