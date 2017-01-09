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
#include "../Content/ScholarData.hpp"
#include "BookUIButton.hpp"
#include "../Furniture/Frame.hpp"


class CenterBook{
    
public:
    
    CenterBook();
    
    void loadModel();
    void setup(ScholarData *sData, Frame *f);
    void update();
    void draw();
    void drawDebug();
    void drawContentToTexture();
    void resetCamera();

    void mapMesh();
    
    ScholarData *scholarData;
    Frame *frame;
    
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
    ofTrueTypeFont boldUIFont;
    ofTrueTypeFont tagButtonFont;
    ofTrueTypeFont scholarFont;
    ofTrueTypeFont smallBookFont;
    vector<ofVec2f> pageTexCoords;
    float pageWidth, pageHeight;
    float page1LeftMargin, page2LeftMargin;
    float pageTopMargin;
    
    ofImage filigreeBorder;
    float borderMargin;
    ofVec2f borderPos;
    float borderHeight, borderWidth;

    //text stored as vector of lines
    //so we can draw aligned center
    vector<vector<string>> helpText;
    int currentHelpText;
    
    ofVboMesh deskMesh;
    vector<ofVec3f> meshPoints;
    
    //animation key frames (normalized)
    float animationPos;
    float targetAnimationPos;
    const float animationStart = 0.0;
    const float animationSpread1 = 0.34;
    const float animationSpread2 = 0.56;
    const float animationSpread3 = 0.77;
    const float animationEnd = 0.94;
    
    const float displayRotX = -90;
    const float displayRotZ = 180;
    
    int currentOpenPage;
    int currentScholar;
    
    //-----GUI SETUP-----
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    void setVariablesFromGui();
    bool bIsGuiActive;
    
    ofxPanel gui;
    string guiName;
    string filePath;
    
    ofxVec2Slider guiPosSlider;
    
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
    
    ofxLabel detectionLabel;
    ofxFloatSlider bookLeftBoundSlider;
    ofxFloatSlider bookCenterSlider;
    ofxFloatSlider bookPageTopSlider;
    ofxFloatSlider bookPageBottomSlider;
    ofxIntSlider touchWaitSlider;
    
    //desk positions
    ofxVec2Slider meshPt0;
    ofxVec2Slider meshPt1;
    ofxVec2Slider meshPt2;
    ofxVec2Slider meshPt3;
    
    float topMargin;
    float betweenScholars;
    float betweenNames;
    float highlightBoxHeight;
    float nameHeight;
    float lineHeight;
    
    ofImage divider;
    ofImage thinDivider;
    ofColor textColor;
    ofColor highlightColor;
    
    vector<bool> scholarHoverStates;
    vector<bool> page4OptionHoverStates;
    
    ofVec3f tagPos;
    ofVec3f tagHelpTextPos;
    string tagHelpText;
    BookUIButton tagButton;
    
    vector<string> page4OptionText;
    
    struct MouseTouch{
        
        ofVec2f pos;
        bool bIsTouching;
        
    };
    
    vector<MouseTouch> mouseTouches;
    unsigned long long lastTouchTime;

};



