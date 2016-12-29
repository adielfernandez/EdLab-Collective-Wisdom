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
#include "ofxGui.h"
#include "Tile.hpp"
#include "TiledObject.hpp"

#pragma once


class Wallpaper: public TiledObject{
    
public:
    
    Wallpaper();
    
    void setup();
    void loadMedia();
    void mapMesh(int res);
    void update();
    void draw();
    
    int getResFromSlider(int sliderVal);
    
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    
    int gridSpacing;
    ofVec2f sceneDim;
    
    ofxLabel mappingLabel;
    ofxIntSlider tileResSlider;
    ofxButton reMapMeshButton;

    bool bIsGuiActive;
    int tileRes;
    string tileResSliderName;
    bool needsReMap;
    

};