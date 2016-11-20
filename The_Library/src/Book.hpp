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
    
    void setup(int type, ofTexture *_tex);
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
    
    
    /*
     //When model scale = 1.0,
     The real world pixel dimensions are below
     
     Scale to multiply mesh verts: 32.4672
     realMin X: -1.2483
     realMax X: -432.41
     realMin Y: -1.95889
     realMax Y: -59.3313
     realMin Z: -339.395
     realMax Z: 300.192
     
     */
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
    //Actual amount needed to scale
    //the vertices to get real world size (in px)
    const float vertScale = 32.4672;
    
    ofVec3f modelRealDim;
    float realMaxX;
    float realMinX;
    float realMaxY;
    float realMinY;
    float realMaxZ;
    float realMinZ;
    
    
    
};



