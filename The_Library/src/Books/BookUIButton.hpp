//
//  BookUIButton.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#ifndef BookUIButton_hpp
#define BookUIButton_hpp

#include <stdio.h>

#endif /* BookUIButton_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once

class BookUIButton{
    
public:
    
    BookUIButton();
    
    void setup(int _type, ofVec3f startPos, float yPos);
    void update();
    void draw();
    
    void show();
    void hide();

    void hover();
    void select();
    
    void checkForClicks(int x, int y);
    
    
    //0 = exit, 1 = prev, 2 = next
    int type;
    
    bool state;
    
    bool bIsDisplayed;
    bool bIsUnavailable;
    bool bIsHovering;
    bool bIsHidden;
    
    //positioning
    ofVec3f hiddenPos, displayedPos, currentPos;
    
    //formatting
    float buttonRad, ringRad;
    float ringWeight;
    float buttonScale;
    
    //button colors
    ofColor currentButtonCol;
    ofColor unavailableButtonCol;
    ofColor availableButtonCol;
    
    //ring colors
    ofColor currentRingCol;
    ofColor availableRingCol;
    ofColor unavailableRingCol;
    ofColor ringHoverCol;
    ofColor ringSelectCol;
    float symbolWeight;
    
    float colorLerpSpeed;
    
    float symbolDim;
    ofPolyline symbolLine;
    
    
};