//
//  SpawnEffect.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/7/17.
//
//

#ifndef SpawnEffect_hpp
#define SpawnEffect_hpp

#include <stdio.h>

#endif /* SpawnEffect_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once


struct Ribbon{
    
    ofVboMesh mesh;
    ofVec3f axis;
    float angle;
    float baseRad;
    
};


class SpawnEffect{
    
public:
    
    SpawnEffect();
    
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
    float baseRad;
    float radAmp;
    float noiseSpeed;
    float nScale;
    float colorRange;
    float orbitSpeed;
    float shrinkTime;
    
    bool useNoise;
    
    int numRibbonsToDraw;
    const int numRibbons = 10;
    
    bool shrink;
    
    
};



