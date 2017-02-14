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
//    string tag;
    int tagNum;
    ofColor tagCol;
    bool bIsCenterBookButton;
    bool bScholarButton;
    ofVec3f src;
    
};

class BookUIButton{
    
public:
    
    BookUIButton();
    
    void setup(int _type, ofVec3f bookPos);
    void setLibraryInfo(vector<ofVec3f> shelf, int _shelfNum, int _bookNum);
    void setIcons(ofImage *icon, ofImage *hover);
    void setTag(string t1, string t2, int _tagNum, ofColor c);
    void setFont( ofTrueTypeFont *f );
    void update();
    void draw();
    
    void show();
    void hide();

    void hover();
    void select();
    
    void checkForClicks(int x, int y, bool touchState);
    
    
    //0 = exit, 1 = prev, 2 = next
    //3 = tag button, 4 = centerbook 
    int type;
    int bookNum;
    int shelfNum;
    
    //for the one off button we draw
    //in the central book
    bool bIsCenterBookButton = false;
    bool bScholarButton = false;
    
    ofTrueTypeFont *font;
    ofColor tagCol, tagOutlineCol;
    
    int tagNum;
//    string tag;
    string tagLine1, tagLine2;
    int linesInTag;
    ofVec3f tagLine1Pos, tagLine2Pos;
    string tagHelp;
    ofVec3f tagHelpPos;
    float tagHelpTrans;
    float tagHelpScale;
    
    ofColor helpTextCol;
    
    bool buttonState;
    unsigned long long lastButtonPress;
    int debounceTime;
    
    bool bIsDisplayed;
    bool bIsUnavailable;
    bool bIsHovering;
    bool bIsHidden;
    
    //positioning
    ofVec3f hiddenPos, displayedPos, currentPos;
    
    float buttonLerpSpeed;
    
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
    
    float hoverTrans;
    unsigned long long lastHoverTime;
    
    float pushButtonScale;
    
    //event listening between book
    //controller and buttons
    ofEvent<ButtonEvent> newButtonClickEvt;
    
    
};



