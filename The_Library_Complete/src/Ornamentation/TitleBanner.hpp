//
//  TitleBanner.hpp
//  The_Library_Complete
//
//  Created by Adiel Fernandez on 2/20/17.
//
//

#ifndef TitleBanner_hpp
#define TitleBanner_hpp

#include <stdio.h>

#endif /* TitleBanner_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once

class TitleBanner{
    
public:
    
    TitleBanner();
    
    void setup();
    void update();
    void draw();
    
    void show(float duration = 5.0f);
    void hide();
    
    ofImage titleImg;
    ofImage subTitleImg;
    
    
    ofVec3f titlePos, subTitlePos;
    ofVec3f titleDisplayedPos, subTitleDisplayedPos;
    ofVec3f titleHiddenPos, subTitleHiddenPos;
    
    bool bIsShowing;
    bool bDrawToScreen;
    
    float showStartTime, hideStartTime;
    
    float displayTime;
    float easeInDuration, easeOutDuration;
    float stagger;
};




