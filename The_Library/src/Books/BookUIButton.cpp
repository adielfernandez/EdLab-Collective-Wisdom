//
//  BookUIButton.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#include "BookUIButton.hpp"



BookUIButton::BookUIButton(){
    
}

void BookUIButton::setup(int _type, ofVec3f startPos, float yPos){
    
    type = _type;
    
    //buttons draw relative to book pos (bottom right corner of spine)
    
    hiddenPos.set(startPos.x, startPos.y + yPos, -20);
    displayedPos.set(startPos.x + 150, startPos.y + yPos, -20);
    currentPos = hiddenPos;
    
    
    state = false;
    
    buttonRad = 30;
    ringRad = buttonRad * 0.8;
    
    buttonScale = 1.0;
    
    availableButtonCol.set(74);
    unavailableButtonCol.set(43);
    currentButtonCol.set(availableButtonCol);
    
    availableRingCol.set(255);
    unavailableRingCol.set(74);
    currentRingCol.set(availableButtonCol);

    
    
    symbolDim = 10;
    
    if(type == 0){
        ringHoverCol.set(255, 0, 0);
        
    } else {
        
        ringHoverCol.set(0, 255, 0);
        
        
        if(type == 1){
            //left button
            symbolLine.addVertex(ofVec3f(-symbolDim, 0));
        } else {
            //right button
            symbolLine.addVertex(ofVec3f(symbolDim, 0));
        }
        symbolLine.addVertex(ofVec3f(0, symbolDim));
        
        
    }
    
    ringWeight = 4;
    colorLerpSpeed = 0.01;
    

    //just for debug, color the backgrounds
    if(type == 0 ) {
        currentButtonCol.set(255, 0, 0);
    } else if(type == 1){
        currentButtonCol.set(0, 0, 255);
    } else {
        currentButtonCol.set(0, 255, 0);
    }
    
    
}

void BookUIButton::checkForClicks(int x, int y){

    if(!bIsUnavailable && bIsDisplayed){
        
        //check for click
        if(ofDist(x, y, currentPos.x, currentPos.y) < buttonRad){
            state = true;
        }
        
        cout << "Button Pressed: " << type << endl;
        
    }
    
}

void BookUIButton::show(){

    bIsUnavailable = false;
    bIsDisplayed = true;
    bIsHidden = false;
    
}

void BookUIButton::hide(){

    bIsUnavailable = true;
    bIsDisplayed = false;
    
}

void BookUIButton::update(){
    
//    if(bIsUnavailable){
//        currentRingCol.lerp(unavailableRingCol, colorLerpSpeed);
//        currentButtonCol.lerp(unavailableButtonCol, colorLerpSpeed);
//    } else {
//        currentRingCol.lerp(availableRingCol, colorLerpSpeed);
//        currentButtonCol.lerp(availableButtonCol, colorLerpSpeed);
//    }
//    
//    if(bIsHovering){
//        currentRingCol.lerp(ringHoverCol, colorLerpSpeed);
//    } else {
//        currentRingCol.lerp(availableRingCol, colorLerpSpeed);
//    }
    
    if(!bIsDisplayed){
        currentPos.interpolate(hiddenPos, 0.01);
        
        //if we're really close to the hidden pos, hide the button
        ofVec3f between = currentPos - hiddenPos;
        if(between.lengthSquared() < 0.01){
            bIsHidden = true;
        }
        
    } else {
        currentPos.interpolate(displayedPos, 0.05);
        
        bIsHidden = false;
    }
    
}

void BookUIButton::draw(){
    
    if(!bIsHidden){
        
//        cout << "We are drawing at: " << currentPos << endl;
        
        ofPushMatrix();
        ofTranslate(currentPos);
        
        ofPushStyle();
        
        //draw button
        ofSetColor(currentButtonCol);
        ofDrawCircle(0, 0, buttonRad);
        
        //draw ring
        ofSetColor(currentRingCol);
        ofNoFill();
        ofSetLineWidth(ringWeight);
        ofDrawCircle(0, 0, ringRad);
        
        if(type == 0){
            
            ofSetLineWidth(5);
            //draw X for exit button
            ofDrawLine(-symbolDim, -symbolDim, symbolDim, symbolDim);
            ofDrawLine(symbolDim, -symbolDim, -symbolDim, symbolDim);
            
        } else {
//            ofSetColor(255);
            symbolLine.draw();
        }
        
        //draw a long line through the Z to debug
//        ofSetColor(0, 255, 0);
//        ofDrawLine(0, 0, -5000, 0, 0, 5000);
        
        
        ofPopStyle();
        
        ofPopMatrix();
        
    }
    
}







