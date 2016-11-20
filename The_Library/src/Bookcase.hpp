//
//  Bookcase.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/7/16.
//
//

#ifndef Bookcase_hpp
#define Bookcase_hpp

#include <stdio.h>

#endif /* Bookcase_hpp */

#include "ofMain.h"
#include "ofxGui.h"
#include "Tile.hpp"
#include "TiledObject.hpp"

#pragma once


class Bookcase: public TiledObject{
    
public:
    
    Bookcase();
    
    void setup(string name, bool _leftCase);
    
    void prepareMesh();
    void mapMesh();
    
    void update();
    void draw();
    void drawDebug();
    
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    

    
    //LEFT or RIGHT
    bool bLeftCase;
    
    //lower left corner of shelves
    ofVec2f shelf1Pos; //top
    ofVec2f shelf2Pos; //middle
    ofVec2f shelf3Pos; //bottom shelf
    
    
    //Tile coordinates are based on
    //the frame corners and the percentages
    //of the control points
    vector<float> controlPointPcts;
    vector<ofVec2f> controlPoints;
    vector<ofVec2f> bookcaseCorners;
    vector<ofVec2f> texCoordControlPts;
    vector<ofVec2f> texCoordCorners;
    
    
    //mesh prepping variables
    //see bookcase_mapping.png for details
    float bookcaseWidth, bookcaseHeight;
    float edgeTileWidth, midTileWidth;
    float topTileHeight, bookTileHeight;
    float shelfTileHeight, bottomTileHeight;
    
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
    ofxFloatSlider controlPtPct8;
    ofxFloatSlider controlPtPct9;
    ofxFloatSlider controlPtPct10;
    ofxFloatSlider controlPtPct11;
    ofxFloatSlider controlPtPct12;
    ofxFloatSlider controlPtPct13;
    ofxFloatSlider controlPtPct14;
    ofxFloatSlider controlPtPct15;
    

    
    
};