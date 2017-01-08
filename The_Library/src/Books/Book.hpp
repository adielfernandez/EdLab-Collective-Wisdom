//
//  Book.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/15/16.
//
//

#ifndef Book_hpp
#define Book_hpp

#include <stdio.h>

#endif /* Book_hpp */

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxEasing.h"
#include "../Content/Contribution.hpp"
#include "BookUIButton.hpp"
#include "SpawnEffect.hpp"


#pragma once

class Book{
    
    
public:
    
    Book();
    
    void loadModel(int bType, int tType, int fType);
    void setup(ofTexture *_tex, ofTrueTypeFont *_bookFont, ofTrueTypeFont *_UIFont);
    void setupContent(Contribution c, int _tagNum, ofColor _tagCol);
    
    void setupUI(vector<ofImage> *_icons, vector<ofVec3f> shelf);
    
    void setLocation(ofVec3f stored, ofVec3f display, int sNum, int bNum);
    
    void update();
    void triggerDisplay();
    void triggerNewBookEvt();

    void formatTextForDisplay();
    void drawContentToTexture();
    void draw();
    
    void goToAnimationPos(float a);
    
    vector<ofImage> *icons;
    
    //Support Methods
    bool bPrintDebug;
    
    
    //Book type: 0 = short, 1 = medium, 2 = tall
    int bookType;
    int texType;
    int fontType;
    
    //shelf numbers left 0,1,2 and right 3,4,5
    int shelfNum;
    //unique id for this book
    int bookID;
    
    //for books that have no message and
    //not needed in the library
    bool bIsUnused;
    
    //Books stored in the shelves
    bool bIsActive;
    bool bIsDisplayed;
    bool bIsClosing;
    double closingStartTime;
    
    bool bIsInShelf;
    
    void putInShelf();
    
    
    bool bIsNewBookEvt;
    
    //Book interface
    void showButtons();
    void hideButtons();
    
    //returns true if an exit button
    //is clicked
    bool checkButtonsForClicks(int x, int y, bool touchState);
    BookUIButton nextButton;
    BookUIButton prevButton;
    BookUIButton exitButton;
    BookUIButton tagButton;
    
    
    
    //visuals and texturing
    ofxAssimpModelLoader model;
    bool bNeedsUpdate;
    
    ofFbo textureFBO;
    ofMaterial material;
    ofTexture *tex;
    ofVboMesh spineMesh;
    
    //position and dimensions of spine in texture map
    const float spineTexCoordX = 118.397;
    const float spineTexCoordY = 13.254;
    const float spineTexWidth = 22.125;
    const float spineTexHeight = 189.917;
    
    //get a pointer to the font we'll be using
    //but also store the font list so we
    //can give the right font to the UI buttons
    ofTrueTypeFont *font;
    ofTrueTypeFont *UIFont;

    //Book Content and Page layouts
    vector<ofVec2f> pageTexCoords;
    float pageWidth, pageHeight;
    
    Contribution userContribution;
    
    //local contribution storage for ease
//    vector<string> tagList;
    int tagNum;
    ofColor tagCol;
    
    //Taglet animation
    void showTaglet();
    bool bShowTaglet;
    bool bFadeOutTaglet;
    float tagTrans;
    double tagletStartTime;
    float tagletDuration;
    
    vector<string> pageText;
    int pageSpreadsAvailable;
    float widthScale;
    float flatScale, flattenAmt;
    
    float spineTrans;
    
    //positioning and animation
    ofVec3f pos;
    ofVec3f pulledOutPos;
    ofVec3f displayPos;
    ofVec3f storedPos;
    ofVec3f spawnPos;
    
    const float storedRotX = -90;
    const float storedRotZ = 90;
    const float displayRotX = -90;
    const float displayRotZ = 180;
    float currentRotX, currentRotZ;
    
    float spawnAngleX, spawnAngleZ;
    
    double animStartTime;
    float animPos;
    float targetAnimPos;
    
    
    //animation key frames (normalized)
    const float animationStart = 0.0;
    const float animationSpread1 = 0.34;
    const float animationSpread2 = 0.56;
    const float animationSpread3 = 0.77;
    const float animationEnd = 0.94;
    
    //X, Y and Z we'll use to center the
    //book on the axis so it spins nicer
    //during spawning
    ofVec3f toCenter;
    
    
    
    //for user selection between pages
    int currentOpenPage;
    float pageLerpSpeed;
    
    //-----Dimensions-----
    
    //This is the fudge factor.
    //AssImp models do not reliable scale
    //resize to expected results. This factor
    //is determined visually to give
    //books the desired size
    float modelScale;
    float currentScale;
    float displayScale;
    
    //these are the actual min/max
    //dimensions of the closed book
    float thickness, depth, height;

    
    //-----Spawn Flourish Effect-----
    SpawnEffect spawnEffect;
    float spawnDuration;
    float spawnPosBackEasing;
    
    
};



