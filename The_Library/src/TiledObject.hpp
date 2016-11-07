//
//  TiledObject.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/6/16.
//
//

#ifndef TiledObject_hpp
#define TiledObject_hpp

#include <stdio.h>

#endif /* TiledObject_hpp */


#include "ofMain.h"
#include "Tile.hpp"

#pragma once


class TiledObject{
    
public:
    
    TiledObject();
    
    void setupTiledObject();
    void update();
    void draw();
    
    vector<Tile> tiles;
    vector<ofImage> images;

    
    int currentImg, nextImg;
    
    ofImage result;
    
    
    //-----EFFECTS-----
    void applyEffectToAll( Tile::Effect e );
    void triggerWave(ofVec2f epicenter);
    bool bWave;
    float waveSpeed;
    ofVec2f waveEpicenter;
    int waveTileIndex;
    unsigned long waveStartTime;

    
    
};




