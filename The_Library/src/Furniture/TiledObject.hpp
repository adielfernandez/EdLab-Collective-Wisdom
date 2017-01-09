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
#include "ofxGui.h"

#pragma once


class TiledObject{
    
public:
    
    TiledObject();
    
    void setupTiledObject(bool isBookcase);
    void update();
    void draw();
    
    void setupCommonGui();
    void updateCommonGui();
    
    vector<Tile> tiles;
    vector<ofImage> images;
    
    bool bIsBookcase;
    int currentImg, nextImg;
    
    ofImage result;
    
    //Common Gui elements to
    //all child classes
    ofxPanel gui;
    string guiName;
    string filePath;
    
    ofxVec2Slider guiPosSlider;
    
    ofxLabel settingsLabel;
    ofxFloatSlider waveSpeedSlider;
    ofxFloatSlider effectDurationSlider;
    ofxFloatSlider backEasingSlider;
    
    
    
    
    //-----EFFECTS-----
    void applyEffectToAll( Tile::Effect e );
    void triggerWave(ofVec2f epicenter);
    bool bWave;
    float waveSpeed;
    ofVec2f waveEpicenter;
    int waveTileIndex;
    unsigned long waveStartTime;
    
    bool bIsAnimating;
    double lastAnimationTime;

    //convenience algorithm for finding
    //intersection of two line segments
    //used for finding points BETWEEN control points
    ofVec2f getIntersectionPoint(ofVec2f line1Start, ofVec2f line1End, ofVec2f line2Start, ofVec2f line2End);
    
};




