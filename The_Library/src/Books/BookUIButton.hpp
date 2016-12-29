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

struct ButtonEvent{
    
    int type;
    int bookNum;
    int shelfNum;
    string tag;
    int tagNum;
    
};

class BookUIButton{
    
public:
    
    BookUIButton();
    
    void setup(int _type, ofVec3f bookPos, vector<ofVec3f> shelf, int _shelfNum, int _bookNum);
    void setIcons(ofImage *icon, ofImage *hover);
    void setTag(string t, int _tagNum, ofColor c);
    void update();
    void draw();
    
    void show();
    void hide();

    void hover();
    void select();
    
    void checkForClicks(int x, int y);
    
    
    //0 = exit, 1 = prev, 2 = next
    //3 = tag button
    int type;
    int bookNum;
    int shelfNum;
    
    ofColor tagCol, tagOutlineCol;
    
    string tag, tagLine1, tagLine2;
    int tagNum;
    
    int linesInTag;
    
    bool buttonState;
    
    bool bIsDisplayed;
    bool bIsUnavailable;
    bool bIsHovering;
    bool bIsHidden;
    
    //positioning
    ofVec3f hiddenPos, displayedPos, currentPos;
    
    //corners of the shelf the buttons are in
    vector<ofVec3f> shelfCorners;
    ofVec3f bookDisplayPos;
    float shelfHeight;
    
    //formatting
    float buttonScale;
    float buttonWidth;
    float buttonHeight;
    
    ofImage *buttonIcon;
    ofImage *hoverIcon;
    
    
    
    //event listening between book
    //controller and buttons
    ofEvent<ButtonEvent> newButtonClickEvt;
    
    
};



