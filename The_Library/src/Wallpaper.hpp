//
//  WallPaper.hpp
//  Visuals_Test
//
//  Created by Adiel Fernandez on 10/23/16.
//
//

#ifndef Wallpaper_hpp
#define Wallpaper_hpp

#include <stdio.h>

#endif /* Wallpaper_hpp */

#include "ofMain.h"
#include "Tile.hpp"

#pragma once


class Wallpaper{
    
public:
    
    Wallpaper();
    
    void setup();
    void update();
    void draw();
    
    vector<Tile> tiles;
    

    
    vector<ofImage> images;
    
    int currentImg, nextImg;
    
    int gridSpacing;
    ofVec2f sceneDim;
    
    
    //-----EFFECTS-----
    void applyEffectToAll( Tile::Effect e );
    void triggerWave(ofVec2f epicenter);
    bool bWave;
    float waveSpeed;
    ofVec2f waveEpicenter;
    int waveTileIndex;
    unsigned long waveStartTime;
    
};