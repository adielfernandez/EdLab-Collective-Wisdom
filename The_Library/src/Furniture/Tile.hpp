//
//  Tile.hpp
//  Visuals_Test
//
//  Created by Adiel Fernandez on 10/23/16.
//
//

#ifndef Tile_hpp
#define Tile_hpp

#include <stdio.h>

#endif /* Tile_hpp */

#include "ofMain.h"
#include "ofxEasing.h"
#pragma once


class Tile{
    
public:
    
    Tile();
    
    void setup(vector<ofVec3f> verts, vector<ofVec2f> texCoords);
    void update();
    void setTextures(vector<ofImage> *imgs);
    void setActiveTexture(int num);
    void setNextTexture(int num);
    
    void draw();
    
    ofVboMesh mesh;
    
    vector<ofImage> *images;
    int activeTexNum;
    int nextTexNum;
    
    ofVec3f positionOnWall, tileCenter;
    
    ofVboMesh darkBacking;
    
    //key to allow sorting a vector of tiles
    //by distance (for wave effects)
    float distToEpicenter;
    float timeUntilWave;
    
    //overload the < operator so we can sort
    bool operator < (const Tile &t) const{
        return (this -> distToEpicenter < t.distToEpicenter);
    }
    
    
    //controlling effects
    enum Effect{
        
        //"Transition" effects are even
        //others are odd.
        FLIP_TRANSITION_HORIZ = 0,
        FLIP_TRANSITION_VERT = 2,
        FLIP_TRANSITION_AXIS = 4,
        FLIP_TRANSITION_RAND = 6,
        
        FLIP_OUT = 1,
        FLIP_IN = 3
        
    };
    
    bool bRotating;
    bool bFlipAxis;
    bool bFlipHoriz, bFlipVert;
    bool bFlipInOut;
    bool bIsBackward;
    bool bDrawDarkBacking;
    
    float flipSpeed;

    float backEasing;
    
    float startAngle, endAngle, currentAngle;
    ofVec3f rotAxis;
    
    
    double effectStartTime, effectStagger, effectEndTime;
    float effectDuration;
    
    void triggerEffect( Tile::Effect e, float stagger = 0.0, ofVec3f flipAxis = ofVec3f(0, 1, 0) );
    
    void triggerWaveTransition(ofVec2f epicenter);
    
};