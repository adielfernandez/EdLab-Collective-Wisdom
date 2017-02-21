//
//  NotificationBanner.hpp
//  The_Library_Complete
//
//  Created by Adiel Fernandez on 2/20/17.
//
//

#ifndef NotificationBanner_hpp
#define NotificationBanner_hpp

#include <stdio.h>

#endif /* NotificationBanner_hpp */

#include "ofMain.h"
#include "ofxEasing.h"
#pragma once

class NotificationBanner{
    
public:
    
    NotificationBanner();
    
    void setup(ofVec3f bookcasePos);
    void update();
    void draw();
    
    void show(float duration = 5.0f);
    void hide();
    
    void setNewBook();
    void setArchive();
    
    ofImage newBookImg;
    ofImage archiveImg;
    
    float imgW, imgH;
    
    ofImage *currentImg;
    
    ofVec3f pos;
    ofVec3f displayedPos;
    ofVec3f hiddenPos;
    
    bool bIsShowing;
    bool bDrawToScreen;
    
    float showStartTime, hideStartTime;
    float putAwayTime;
    
    float displayTime;
    float easeInDuration, easeOutDuration;

    
};


