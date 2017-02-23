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




#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Book.hpp"
#include "ofxGui.h"
#include "../Content/ScholarData.hpp"
#include "BookUIButton.hpp"
#include "../Furniture/Frame.hpp"


#include "../SceneController.hpp"

#include "../Tracking/DeskCam.hpp"
#include "../Tracking/WallCam.hpp"
#include "../Tracking/DeskTouch.hpp"
#include "../Tracking/DeskTouch.hpp"

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
    
    double idleTime;
    double lastInteractionTime;
    
    ScholarData *scholarData;
    Frame *frame;
    
    //Model stuff
    ofxAssimpModelLoader model;
    const float baseScale = 819.219;
    
    float modelScale, bulkScale;
    float widthScale, heightScale, depthScale;
    float height, thickness, depth;
    
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
    
    ofVec3f bookPos;
    
    ofTexture bookTex;
    ofTexture deskTex;
    
    ofEasyCam camera;
    
    ofFbo bookTexFBO;
    
    ofFbo deskFBO;
    const float deskWidth = 400;
    const float deskHeight = 175;
    
    ofVec2f rawDeskPos;
    
    ofVboMesh deskMesh;
    vector<ofVec3f> meshPoints;
    
    
    //Book Content and UI

    //Book fonts (serifs)
    ofTrueTypeFont listFont;
    ofTrueTypeFont helpTextFont;
    ofTrueTypeFont homeButtonFont;
    ofTrueTypeFont staticTextFont;
    ofTrueTypeFont activeTextFont;
    
    
    ofColor staticTextColor;
    ofColor activeTextColor;
    ofColor listTextColor;
    ofColor buttonHighlightColor;
    ofColor buttonBaseColor;
    
    ofColor bookFiligreeColor;
    
    ofColor tagHoverColor;
    ofColor tagTextColor;
    string tagLine1;
    string tagLine2;
    bool tagHover;
    float tagColorPct;
    
    string tagDescription;
    
    vector<ofVec2f> pageTexCoords;
    float pageWidth, pageHeight;
    float page1LeftMargin, page2LeftMargin;
    float pageTopMargin;
    
    ofImage filigreeBorder;
    float borderMargin;
    ofVec2f borderPos;
    float borderHeight, borderWidth;

    ofImage cornerFiligree;
    
    float topMargin;
    float betweenListItems;
    float betweenNames;
    float highlightBoxHeight;
    float nameHeight;
    float lineHeight;
    
    ofImage divider;
    ofImage thinDivider;
    
    //----------Buttons/Logic----------
    vector<bool> listHoverStates;
    vector<float> listColorPcts;
    

    vector<bool> scholarOptionHoverStates;
    vector<float> scholarOptionColorPcts;
    
    int selectedScholarOption;
    
    //Home button
    bool bShowHomeButton;
    bool homeButtonHover;
    bool backToListHover;
    float backToListColPct;
    
    float homeButtonWidth;
    float homeButtonHeight;
    float homeButtonBottomMargin;
    
    bool scholarButtonHover;
    bool tagButtonHover;
    float scholarButtonColPct;
    float tagButtonColPct;
    
    bool redecorateButtonHover;
    vector<float> redecorateHeights;
    
    
    //position coords inverted (+X to the left)
    ofVec2f homeButtonPos;
    
    //normalized bounds of home button
    
    float homeTopY, homeBotY;
    float homeLeftX, homeRightX;
    
    ofVec3f apparentBookCenter;
    
    
    //text stored as vector of lines
    //so we can draw aligned center
    vector<vector<string>> helpText;
    int currentHelpText;
    
    //Book open page and the current scholar on view
    int currentOpenPage;
    int currentScholar;
    int currentTag;
    

    
    bool bViewingScholars;
    
    
    
    struct MouseTouch{
        
        ofVec2f pos;
        bool bIsTouching;
        
    };
    
    vector<MouseTouch> mouseTouches;
    unsigned long long lastTouchTime;
    
    //stores the touch data
    vector<DeskTouch> touches;
    
    
    //event listening between book
    //controller and buttons
    ofEvent<ButtonEvent> newButtonClickEvt;

    ofEvent<SceneEvent> redecorateEvent;
    
    
    //-----------------------------
    //----------GUI SETUP----------
    //-----------------------------
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
    
    ofxIntSlider filigreeTopMarginSlider;
    ofxIntSlider filigreeLeftMarginSlider;
    ofxIntSlider filigreeWidthSlider;
    ofxIntSlider filigreeHeightSlider;
    ofxIntSlider helpTextTopSlider;
    ofxFloatSlider colorLerpInSlider;
    ofxFloatSlider colorLerpOutSlider;
    
    
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
    ofxToggle printNormCoordsToggle;
    ofxFloatSlider bookLeftBoundSlider;
    ofxFloatSlider bookCenterSlider;
    ofxFloatSlider bookPageTopSlider;
    ofxFloatSlider bookPageBottomSlider;
    ofxIntSlider touchWaitSlider;
    ofxIntSlider verticalShiftSlider;
    ofxFloatSlider idleTimeToResetSlider;
    
    
    //desk control points
    ofxVec2Slider meshPt0;
    ofxVec2Slider meshPt1;
    ofxVec2Slider meshPt2;
    ofxVec2Slider meshPt3;
    


};



#endif /* CenterBook_hpp */
