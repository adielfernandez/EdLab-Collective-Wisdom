//
//  SpawnRibbon.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/7/17.
//
//

#ifndef SpawnRibbon_hpp
#define SpawnRibbon_hpp

#include <stdio.h>

#endif /* SpawnRibbon_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once


struct Ribbon{
    
    ofVboMesh mesh;
    ofVec3f axis;
    float angle;
    float baseRad;

    
};


class SpawnRibbon{
    
public:
    
    SpawnRibbon();
    
    void setup(ofColor tagCol);
    void update(float timeLeft);
    void reset();
    void draw();
    
    void shrinkRibbons();

    ofColor col;
    int timeOffset;
    vector<Ribbon> ribbons;
    
    int numSegments;
    float totalLength;
    float ribbonWidth;
    float ribbonTaper;
    float colorRange;
    float baseRad;
    float noiseAmp;
    float noiseSpeed;
    float nScale;
    float orbitSpeed;
    float shrinkTime;
    float staggerAmt;
    
    bool useNoise;
    
    int numRibbonsToDraw;
    const int numRibbons = 10;
    
    bool shrink;
    bool bDrawWireframe;
    
};



