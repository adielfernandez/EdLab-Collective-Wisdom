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
    int numCols, numRows;
    ofVec2f sceneDim;
    
    ofImage result;
    
    
    //-----EFFECTS-----
    void applyEffectToAll( Tile::Effect e );
    void triggerWave(ofVec2f epicenter);
    bool bWave;
    ofVec2f waveEpicenter;
    int waveTileIndex;
    float waitToNextWaveTile;
    unsigned long lastWaveTileTime;
    
};