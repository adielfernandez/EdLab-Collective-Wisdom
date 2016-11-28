//
//  Frame.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/6/16.
//
//

#ifndef Frame_hpp
#define Frame_hpp

#include <stdio.h>

#endif /* Frame_hpp */


#include "ofMain.h"
#include "ofxGui.h"
#include "Tile.hpp"
#include "TiledObject.hpp"

#pragma once


class Frame: public TiledObject{

public:
    
    Frame();
    
    void setup(string name);
    void loadMedia();
    void prepareMesh();
    void mapMesh();
    
    void update();
    void draw();
    void drawDebug();
    
    void drawShadow();
    ofPath shadow;
    
    void setupGui();
    void drawGui(int x, int y);
    void drawGui();
    void saveSettings();
    void loadSettings();
    
    
    vector<ofImage> portraits;
    int currentPortrait;
    ofMesh portraitMesh;
    vector<ofVec3f> portraitVerts;
    ofFbo portraitFbo;
    void setPortraitTexCoords(int x, int y);
    unsigned long long lastPortraitChange;
    
    
    //Tile coordinates are based on
    //the frame corners and the percentages
    //of the control points
    vector<float> controlPointPcts;
    vector<ofVec2f> controlPoints;
    vector<ofVec2f> frameCorners;
    vector<ofVec2f> texCoordControlPts;
    vector<ofVec2f> texCoordCorners;
    
    //also what percentage of the width/height
    //each of the tiles are on the frame borders
    float vertBorderTileHeightPct;
    float horizBorderTileWidthPct;
    
    //mesh prepping variables
    float frameWidth, frameHeight;
    float borderWidthPct, borderHeightPct;
    
    bool bIsGuiActive;
    
    ofxLabel mappingLabel;
    ofxButton reMapMeshButton;
    unsigned long long lastMapTime;
    
    ofxVec2Slider frameCorner0;
    ofxVec2Slider frameCorner1;
    ofxVec2Slider frameCorner2;
    ofxVec2Slider frameCorner3;
    ofxFloatSlider controlPtPct0;
    ofxFloatSlider controlPtPct1;
    ofxFloatSlider controlPtPct2;
    ofxFloatSlider controlPtPct3;
    ofxFloatSlider controlPtPct4;
    ofxFloatSlider controlPtPct5;
    ofxFloatSlider controlPtPct6;
    ofxFloatSlider controlPtPct7;
    

    
    
};



