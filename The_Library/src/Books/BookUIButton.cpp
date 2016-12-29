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

void BookUIButton::setup(int _type, ofVec3f bookPos, vector<ofVec3f> shelf, int _shelfNum, int _bookNum ){
    
    type = _type;
    bookNum = _bookNum;
    shelfNum = _shelfNum;
    
    //buttons draw relative to book pos (bottom right corner of spine)
    //shelf corners are ordered counter clockwise from bottom left corner
    shelfCorners = shelf;
    
    bookDisplayPos = bookPos;
    
    buttonState = false;
    
    bIsDisplayed = false;
    bIsUnavailable = true;
    bIsHidden = true;
    
    bIsHovering = false;
    
    
    
    buttonScale = 0.55;
    
    
}

void BookUIButton::setIcons(ofImage *icon, ofImage *hover){
    
    buttonIcon = icon;
    hoverIcon = hover;
    
    buttonWidth = buttonIcon -> getWidth() * buttonScale;
    buttonHeight = buttonIcon -> getHeight() * buttonScale;
    
    //set the button margins and spacing based on the self and icon dimensions
    shelfHeight = (shelfCorners[1] - shelfCorners[2]).length();
    
    float spacing = (shelfHeight - buttonHeight * 3)/4.0f;
    
    //how far the button is above the book pos
    //i.e. the bottom of the shelf
    float yButtonHeight;
    
    
    //remember, images are inverted because of ofCamera
    if(type == 0){
        //top button
        yButtonHeight = -shelfHeight + spacing + buttonHeight;
    } else if(type == 1){
        //previous button is on the bottom
        yButtonHeight = -shelfHeight + spacing * 3 + buttonHeight * 3;
        
    } else {
        //middle button
        yButtonHeight = -shelfHeight + spacing * 2 + buttonHeight * 2;
    }
    
    //button x position
    float xButtonPos = shelfCorners[1].x - spacing - buttonWidth;
    
    hiddenPos.set(bookDisplayPos.x + 30, bookDisplayPos.y + yButtonHeight, -70);
    displayedPos.set(xButtonPos, bookDisplayPos.y + yButtonHeight, -70);
    currentPos = hiddenPos;
    
}

void BookUIButton::setTag(string t, int _tagNum, ofColor c){
    
    buttonWidth = 100;
    buttonHeight = 40;
    
    tagCol = c;
    
    tagOutlineCol.setHsb(tagCol.getHue(), tagCol.getSaturation(), 180);
    
    tag = t;
    tagNum = _tagNum;
    
    //split the tag up if it has an & symbol
    vector<string> parts = ofSplitString(t, "&");
    
    if(parts.size() > 1){
        
        linesInTag = 2;
        
        //If the first word is shorter, add the "&" back into it.
        //Else, add it to the beginning of the second line
        if(parts[1].length() > parts[0].length()){
            
            tagLine1 = parts[0] + " &";
            tagLine2 = parts[1];
            
        } else {
         
            tagLine1 = parts[0];
            tagLine2 = "& " + parts[1];
            
        }
        
            
        
        
    } else {
        
        //tag only has one word
        tagLine1 = tag;
        tagLine2 = "";
        linesInTag = 1;
    }
    
    cout << "---" <<tagLine1 << "," << tagLine2 << "---" << endl;
    
    
    float spacing = 8;
    
    hiddenPos.set(bookDisplayPos.x + 30, shelfCorners[3].y + spacing, -70);
    displayedPos.set(shelfCorners[3].x + spacing, shelfCorners[3].y + spacing, -70);
    currentPos = hiddenPos;
    
}


void BookUIButton::checkForClicks(int x, int y){

    if(!bIsUnavailable && bIsDisplayed){
        
        //check for click
        if(x > currentPos.x &&
           x < currentPos.x + buttonWidth &&
           y > currentPos.y - buttonHeight &&
           y < currentPos.y ){
            
            buttonState = true;
            
            
            ButtonEvent e;
            e.type = type;
            e.tag = tag;
            e.tagNum = tagNum;
            e.bookNum = bookNum;
            e.shelfNum = shelfNum;
            
            ofNotifyEvent(newButtonClickEvt, e, this);
            
        }
        
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
    
    if(!bIsDisplayed){
        currentPos.interpolate(hiddenPos, 0.07);
        
        //if we're really close to the hidden pos, hide the button
        ofVec3f between = currentPos - hiddenPos;
        if(between.lengthSquared() < 1){
            bIsHidden = true;
        }
        
    } else {
        currentPos.interpolate(displayedPos, 0.09);
        bIsHidden = false;
    }
    
}

void BookUIButton::draw(){
    
    if(!bIsHidden){
        
        //draw button
        //flip the height because ofCamera inverts the Y axis for images
        if(bIsUnavailable){
            ofSetColor(255, 100);
        } else {
            ofSetColor(255);
        }
        
        //types 0-2 are small arrow icons
        if(type < 3){
        
            buttonIcon -> draw(currentPos.x, currentPos.y, -70, buttonWidth, -buttonHeight);
            
        } else {
            //type 3 is the tag button
            ofPushStyle();
            
            ofFill();
            ofSetColor(tagCol);
            ofDrawRectangle(currentPos.x, currentPos.y, -70, buttonWidth, buttonHeight);

            ofNoFill();
            ofSetColor(tagOutlineCol);
            ofSetLineWidth(4);
            ofDrawRectangle(currentPos.x, currentPos.y, -70, buttonWidth, buttonHeight);
            
            ofPopStyle();
        }
        
        
    }

    
    
}







