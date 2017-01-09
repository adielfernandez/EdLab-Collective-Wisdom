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
    void loadMedia();
    void prepareMesh();
    void mapMesh();
    
    void update();
    void draw();
    void drawDebug();
    
    void drawShadow();
    ofVboMesh shadow;
    
        
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    
    //these hold a mesh and texture
    //we'll draw when the bookcase tiles
    //are not actively flipping
    void setStaticBookcase();
    void recordStaticTexture();
    ofVboMesh staticBookcase;
    ofTexture staticBookcaseTex;
    bool bStaticCaseNeedsUpdate;
    
    
    //LEFT or RIGHT
    bool bLeftCase;
    
    //Shelf corners
    //Corners defined counter-clockwise
    //from bottom left
    //3 groups of 4 points 
    vector<vector<ofVec3f>> shelfCorners;
    
    
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
    

    //----------GUI SETUP----------
    bool bIsGuiActive;
    
    ofxLabel mappingLabel;
    ofxButton reMapMeshButton;
    unsigned long long lastButtonPressTime;

    ofxLabel controlPointsLabel;
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
    ofxToggle useShadowToggle;

    ofxButton resetBookcaseButton;
    
    
};