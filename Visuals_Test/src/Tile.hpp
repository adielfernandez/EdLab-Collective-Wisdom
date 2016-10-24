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
    
    void setup(vector<ofVec3f> verts, float _tileWidth);
    void update();
    void setNewTexture(ofTexture *_tex);
    
    void draw();
    
    ofMesh mesh;
    ofTexture *tex;
    
    ofVec3f positionOnWall, tileCenter;
    
    float startTrans;
    float currentTrans, endTrans;
    float tileWidth;
    
    
    //controlling effects
    enum Effect{
        
        //OUT effects are EVEN
        //IN effects are ODD
        
        ROT_OUT_LEFT = 0,
        ROT_OUT_RIGHT = 2,
        ROT_OUT_TOP = 4,
        ROT_OUT_BOTTOM = 6,
        ROT_OUT_RANDOM = 8,
        FLIP_OUT_HORIZ = 11,
        FLIP_OUT_VERT = 13,
        FLIP_OUT_RAND = 15,

        ROT_IN_LEFT = 1,
        ROT_IN_RIGHT = 3,
        ROT_IN_TOP = 5,
        ROT_IN_BOTTOM = 7,
        ROT_IN_RANDOM = 9,
        FLIP_IN_HORIZ = 10,
        FLIP_IN_VERT = 12,
        FLIP_IN_RAND = 14
    };
    
    bool bRotating, bFlipping;
    float flipSpeed;
    ofVec3f rotateStart, rotateEnd;
    ofVec3f currentRotation;
    
    
    double effectStartTime, effectStagger, effectEndTime;
    float effectDuration;
    
    void triggerEffect( Tile::Effect e, float stagger = 0.0 );
    
    
};