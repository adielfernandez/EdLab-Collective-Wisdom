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

#pragma once

class Book{
    
    
public:
    
    Book();
    
    void loadModel(int bType, int tType);
    void setup(ofTexture *_tex, ofTrueTypeFont *_font);
    void update();
    void triggerDisplay();
    void draw();
    
    //Support Methods
    bool bPrintDebug;
    
    
    //Book type: 0 = short, 1 = medium, 2 = tall
    int bookType;
    int texType;
    int fontType;
    
    //shelf numbers left 0,1,2 and right 3,4,5
    int shelfNum;
    
    bool bIsActive;
    bool bIsAnimating;
    
    //visuals and texturing
    ofxAssimpModelLoader model;
    ofFbo textureFBO;
    ofMaterial material;
    ofTexture *tex;
    ofTrueTypeFont *font;
    ofVboMesh spineMesh;
    
    //positioning and animation
    ofVec3f pos;
    ofVec3f pulledOutPos;
    ofVec3f displayPos;
    ofVec3f storedPos;
    const float storedRotX = -90;
    const float storedRotZ = 90;
    const float displayRotX = -90;
    const float displayRotZ = 180;
    float currentRotX, currentRotZ;
    
    double animStartTime;
    float animPos;
    //animation key frames (normalized)
    const float animationStart = 0.0;
    const float animFirstPages = 0.34;
    const float animSecondPages = 0.56;
    const float animThirdPages = 0.77;
    const float animationEnd = 0.94;
    
    //event to notify bookController that
    //the book has returned to the shelf
    ofEvent<bool> bookReturnedEvt;
    void bookReturned();
    
    //-----Dimensions-----
    
    //This is the fudge factor.
    //AssImp models do not reliable scale
    //resize to expected results. This factor
    //is determined visually to give
    //books the desired size
    float modelScale;
    
    //these are the actual min/max
    //dimensions of the closed book
    float thickness, depth, height;

    vector<ofVec2f> pageTexCoords;
    vector<ofVec2f> pageDims;
    
    
    
};



