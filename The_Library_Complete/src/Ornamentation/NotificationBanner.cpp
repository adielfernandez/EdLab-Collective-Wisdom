//
//  NotificationBanner.cpp
//  The_Library_Complete
//
//  Created by Adiel Fernandez on 2/20/17.
//
//

#include "NotificationBanner.hpp"


NotificationBanner::NotificationBanner(){
    
    
    
}

void NotificationBanner::setup(ofVec3f bookcasePos){
    
    newBookImg.load("assets/interface/banners/newBookBanner.png");
    newBookImg.setAnchorPercent(0.5f, 0.0f);
    
    archiveImg.load("assets/interface/banners/recycleBanner.png");
    archiveImg.setAnchorPercent(0.5f, 0.0f);
    
    //both images have same dimensions
    imgW = newBookImg.getWidth();
    imgH = newBookImg.getHeight();
    
    //set the positions we'll ease between
    //remember texture is inverted in Y due to ofEasyCam we're drawing in

    //also, push it off the wall, just below the bookcase (z = -50)
    float zDepth = -49;
    hiddenPos = bookcasePos + ofVec3f( 0, imgH, zDepth );
    displayedPos = hiddenPos + ofVec3f( 0, -imgH + 10 );

    
    pos = hiddenPos;

    
    bIsShowing = false;
    bDrawToScreen = false;
    
    easeInDuration = 1.0f;
    easeOutDuration = 1.0f;
    
    displayTime = 3.5f;
    
    currentImg = &newBookImg;
    
}

void NotificationBanner::show(float duration){
    
    if( !bIsShowing ){
        
        displayTime = duration;
        
        bIsShowing = true;
        bDrawToScreen = true;
        
        showStartTime = ofGetElapsedTimef();
        
        //declare when the banner will be put away
        putAwayTime = showStartTime + easeInDuration + displayTime;
        
    } else {
        
        //if the banner is already showing, just set the putAwayTime to be longer
        putAwayTime = ofGetElapsedTimef() + displayTime;
        
    }
    
}

void NotificationBanner::hide(){
    
    if( bIsShowing ){
        
        hideStartTime = ofGetElapsedTimef();
        bIsShowing = false;
        
    }
}

void NotificationBanner::setNewBook(){
    currentImg = &newBookImg;
}

void NotificationBanner::setArchive(){
    currentImg = &archiveImg;
}


void NotificationBanner::update(){
    
    if(bIsShowing){
        
        float now = ofGetElapsedTimef();
        
        float pct = ofxeasing::map_clamp(now, showStartTime, showStartTime + easeInDuration, 0.0f, 1.0f, &ofxeasing::back::easeOut_s, 2.5);
        
        pos = hiddenPos.getInterpolated(displayedPos, pct);
        
        //wait for "displayTime" after the banner is in displayPosition to put it away
        if( now > putAwayTime ){
            hide();
        }
        
        
    } else {
        
        float now = ofGetElapsedTimef();
        
        float pct = ofxeasing::map_clamp(now, hideStartTime, hideStartTime + easeOutDuration, 1.0f, 0.0f, &ofxeasing::back::easeIn_s, 1.0);
        
        pos = hiddenPos.getInterpolated(displayedPos, pct);
        
        if( now > hideStartTime + easeOutDuration ){
            bDrawToScreen = false;
        }
        
    }
    
    
    
}

void NotificationBanner::draw(){
    
    if( bDrawToScreen ){
        
        ofSetColor(255);
        currentImg -> draw(pos, currentImg -> getWidth(), -currentImg -> getHeight());
        
    }
    
}





