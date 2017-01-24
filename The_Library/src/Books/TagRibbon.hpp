//
//  TagRibbon.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/11/17.
//
//

#ifndef TagRibbon_hpp
#define TagRibbon_hpp

#include <stdio.h>

#endif /* TagRibbon_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once

class TagRibbon{
    
public:
    
    TagRibbon();
    
    void setup(ofVec3f src, ofVec3f dst, int nPts, ofColor col);
    void update();
    void draw();
    
    ofColor tagCol;
    ofVec3f srcPos, dstPos;
    ofVec3f apexPos;
    
    int dstBookNum;
    
    vector<ofVec3f> points;
    ofVboMesh mesh;
    
    float ribbonWidth;
    float ribbonTaper;
    float colorRange;
    bool bDrawWireframe;
    
    double startTime;
    
    bool bIsDead;
    bool bTouchDown;
    int bookIndex;
    
    bool bUseNoise;
    float noiseAmp;
    float noiseScale;
    float noiseSpeed;
    
    //values set by gui
    int numPoints;
    float pctApex;
    float dt;
    float arcHeight;
    float duration;
    
};






