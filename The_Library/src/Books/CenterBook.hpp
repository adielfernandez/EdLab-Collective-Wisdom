//
//  CenterBook.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 12/11/16.
//
//

#ifndef CenterBook_hpp
#define CenterBook_hpp

#include <stdio.h>

#endif /* CenterBook_hpp */


#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Book.hpp"
#include "ofxGui.h"


class CenterBook{
    
public:
    
    CenterBook();
    
    void loadModel();
    void setup();
    void update();
    void draw();
    void drawDebug();
    void drawContentToTexture();
    void resetCamera();

    void mapMesh();
    
    //Model stuff
    ofxAssimpModelLoader model;
    const float baseScale = 819.219;
    
    float modelScale, bulkScale;
    float widthScale, heightScale, depthScale;
    float height, thickness, depth;
    
    ofVec3f bookPos;
    
    ofTexture bookTex;
    ofTexture deskTex;
    
    ofEasyCam camera;
    
    ofFbo bookTexFBO;
    
    ofFbo deskFBO;
    const float deskWidth = 400;
    const float deskHeight = 175;
    
    ofVec2f rawDeskPos;
    
    //Book Content and UI
    ofTrueTypeFont font;
    vector<string> pageText;
    vector<ofVec2f> pageTexCoords;
    float pageWidth, pageHeight;
    float page1LeftMargin, page2LeftMargin;
    float pageTopMargin;
    
    
    ofVboMesh deskMesh;
    vector<ofVec3f> meshPoints;
    
    //animation key frames (normalized)
    float animationPos;
    const float animationStart = 0.0;
    const float animationSpread1 = 0.34;
    const float animationSpread2 = 0.56;
    const float animationSpread3 = 0.77;
    const float animationEnd = 0.94;
    
    const float displayRotX = -90;
    const float displayRotZ = 180;
    
    //-----GUI SETUP-----
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    void setVariablesFromGui();
    
    ofxPanel gui;
    string guiName;
    string filePath;
    
    ofxLabel positioningLabel;
    ofxVec2Slider bookPosSlider;
    ofxFloatSlider bulkScaleSlider;
    ofxFloatSlider depthScaleSlider;
    ofxFloatSlider heightScaleSlider;
    ofxFloatSlider widthScaleSlider;
    
    ofxLabel mappingLabel;
    ofxToggle showRawDeskToggle;
    ofxButton reMapMeshButton;
    ofxToggle drawWireframeToggle;
    
    ofxLabel contentTextureLabel;
    ofxFloatSlider page1LeftMarginSlider;
    ofxFloatSlider page2LeftMarginSlider;
    ofxFloatSlider pageTopMarginSlider;
    ofxToggle drawBookTexToggle;
    
    
    
    //desk positions
    ofxVec2Slider meshPt0;
    ofxVec2Slider meshPt1;
    ofxVec2Slider meshPt2;
    ofxVec2Slider meshPt3;
    
    
    struct MouseTouch{
        
        ofVec2f pos;
        bool bIsTouching;
        
    };
    
    vector<MouseTouch> mouseTouches;
    
};



