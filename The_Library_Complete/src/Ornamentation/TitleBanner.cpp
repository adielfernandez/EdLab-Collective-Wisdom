//
//  TitleBanner.cpp
//  The_Library_Complete
//
//  Created by Adiel Fernandez on 2/20/17.
//
//

#include "TitleBanner.hpp"


TitleBanner::TitleBanner(){
    
    
    
}

void TitleBanner::setup(){
    
    titleImg.load("assets/interface/banners/titleBanner.png");
    titleImg.setAnchorPercent(0.5f, 0.0f);
    
    subTitleImg.load("assets/interface/banners/subTitleBanner.png");
    subTitleImg.setAnchorPercent(0.5f, 0.0f);
    
    //set the positions we'll ease between
    //remember texture is inverted in Y due to ofEasyCam we're drawing in
    titleHiddenPos.set( ofGetWidth()/2, -200 );
    subTitleHiddenPos = titleHiddenPos + ofVec3f( 0, 0 );
    
    titleDisplayedPos.set( ofGetWidth()/2, titleImg.getHeight() - 15);
    subTitleDisplayedPos = titleDisplayedPos + ofVec3f( 0, -12 );
    
    titlePos = titleHiddenPos;
    subTitlePos = subTitleHiddenPos;
    
    bIsShowing = false;
    bDrawToScreen = false;
    
    easeInDuration = 2.0f;
    easeOutDuration = 1.5f;
    
    stagger = easeInDuration / 9.0f;
    
    displayTime = 10.0f;
    
}

void TitleBanner::show(float duration){
    
    if( !bIsShowing ){
        
        displayTime = duration;
        
        showStartTime = ofGetElapsedTimef();
        bIsShowing = true;
        bDrawToScreen = true;
        
    }
    
}

void TitleBanner::hide(){
    
    if( bIsShowing ){
        
        hideStartTime = ofGetElapsedTimef();
        bIsShowing = false;
        
    }
}


void TitleBanner::update(){
    
    if(bIsShowing){
        
        float now = ofGetElapsedTimef();
        
        float pct1 = ofxeasing::map_clamp(now, showStartTime, showStartTime + easeInDuration, 0.0f, 1.0f, &ofxeasing::back::easeOut_s, 2.0);
        
        titlePos = titleHiddenPos.getInterpolated(titleDisplayedPos, pct1);
        
        float pct2 = ofxeasing::map_clamp(now, showStartTime + stagger, showStartTime + easeInDuration + stagger, 0.0f, 1.0f, &ofxeasing::back::easeOut_s, 2.3);
        subTitlePos = subTitleHiddenPos.getInterpolated(subTitleDisplayedPos, pct2);

        //wait for "displayTime" after the banner is in displayPosition to put it away
        if( now > showStartTime + easeInDuration + displayTime ){
            hide();
        }
        
        
    } else {
        
        float now = ofGetElapsedTimef();
        
        float pct = ofxeasing::map_clamp(now, hideStartTime, hideStartTime + easeOutDuration, 1.0f, 0.0f, &ofxeasing::back::easeIn_s, 1.0);
        
        titlePos = titleHiddenPos.getInterpolated(titleDisplayedPos, pct);
        subTitlePos = subTitleHiddenPos.getInterpolated(subTitleDisplayedPos, pct);
        
        if( now > hideStartTime + easeOutDuration ){
            bDrawToScreen = false;
        }
        
    }
    
    
    
}

void TitleBanner::draw(){
    
    if( bDrawToScreen ){

        ofSetColor(255);
        
        subTitleImg.draw(subTitlePos, subTitleImg.getWidth(), -subTitleImg.getHeight());
        titleImg.draw(titlePos, titleImg.getWidth(), -titleImg.getHeight());
        
    }
    
}





