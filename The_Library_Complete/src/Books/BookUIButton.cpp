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



void BookUIButton::setup(int _type, ofVec3f bookPos ){
    
    type = _type;

//    if(type == 3){
//        bIsCenterBookButton = true;
//    } else {
//        bIsCenterBookButton = false;
//    }
    
    bookDisplayPos = bookPos;
    
    buttonState = false;
    
    bIsDisplayed = false;
    bIsUnavailable = true;
    bIsHidden = true;
    
    bIsHovering = false;
    
    tagHelpTrans = 0.0f;
    tagHelpScale = 0.0f;
    
    buttonScale = 0.55;
    
    lastButtonPress = 0;
    debounceTime = 1000; //in milliseconds
    
    hoverTrans = 0.0;
    lastHoverTime = 0;
    
    pushButtonScale = 1.0f;
    

        
    buttonLerpSpeed = 0.08;
}

void BookUIButton::setLibraryInfo(vector<ofVec3f> shelf, int _shelfNum, int _bookNum){
    
    bookNum = _bookNum;
    shelfNum = _shelfNum;
    
    //buttons draw relative to book pos (bottom right corner of spine)
    //shelf corners are ordered counter clockwise from bottom left corner
    shelfCorners = shelf;
    
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
    
    
    //images are drawn from the center
    if(type == 0){
        //top button
        yButtonHeight = -shelfHeight + spacing + buttonHeight * 0.5;
    } else if(type == 1){
        //previous button is on the bottom
        yButtonHeight = -shelfHeight + spacing * 3 + buttonHeight * 2.5;
        
    } else {
        //middle button
        yButtonHeight = -shelfHeight + spacing * 2 + buttonHeight * 1.5;
    }
    
    //button x position
    float xButtonPos = shelfCorners[1].x - spacing - buttonWidth * 0.5;
    
    hiddenPos.set(bookDisplayPos.x + 30, bookDisplayPos.y + yButtonHeight, -70);
    displayedPos.set(xButtonPos, bookDisplayPos.y + yButtonHeight, -70);
    currentPos = hiddenPos;
    
}

void BookUIButton::setFont(ofTrueTypeFont *f){
    
    font = f;
    
}

void BookUIButton::setTag(string t1, string t2, int _tagNum, ofColor c){
    
    if(bIsCenterBookButton){
        buttonWidth = 210;
        buttonHeight = 100;
    } else {
        buttonWidth = 110;
        buttonHeight = 50;
    }
    
    tagCol = c;
    tagOutlineCol.setHsb(tagCol.getHue(), tagCol.getSaturation(), 100);
    
//    tag = t;
    tagNum = _tagNum;
    
    tagLine1 = t1;
    tagLine2 = t2;
    linesInTag = 2;
    
    //split the tag up if it has an & symbol
//    vector<string> parts = ofSplitString(t, "&");
//    
//    if(parts.size() > 1){
//        
//        linesInTag = 2;
//        
//        //If the first word is shorter, add the "&" back into it.
//        //Else, add it to the beginning of the second line
//        if(parts[1].length() < parts[0].length()){
//            
//            tagLine1 = parts[0];
//            tagLine2 = "& " + parts[1];
//            
//        } else {
//         
//            tagLine1 = parts[0] + " &";
//            tagLine2 = parts[1];
//            
//        }
//        
//    } else {
//        
//        //tag only has one word
//        tagLine1 = tag;
//        tagLine2 = "";
//        linesInTag = 1;
//    }

    
    //Button and text positioning
    float spacing = 5;
    
    //only set the positions if this button isn't in the center book
    if(!bIsCenterBookButton){
        hiddenPos.set(bookDisplayPos.x + 30, shelfCorners[3].y + spacing, -70);
        displayedPos.set(shelfCorners[3].x + spacing, shelfCorners[3].y + spacing, -70);
        currentPos = hiddenPos;
    }
    
    //text positioning is relative to currentPos
    if(linesInTag < 2){
        //start at button center then shift up and down to center text
        tagLine1Pos.set(buttonWidth/2 - font -> stringWidth(tagLine1)/2, buttonHeight/2 + font -> stringHeight(tagLine1)/2);
    } else {
        
        //set the spacing between the lines
        //then make one line above and another below
        float betweenLines = 3;
        float fudgeFactor = 2; //shift a few pixels upwards
        tagLine1Pos.set(buttonWidth/2 - font -> stringWidth(tagLine1)/2, buttonHeight/2 - betweenLines/2.0f - fudgeFactor);
        tagLine2Pos.set(buttonWidth/2 - font -> stringWidth(tagLine2)/2, buttonHeight/2 + betweenLines/2.0f + font -> stringHeight(tagLine1) - fudgeFactor);
        
    }
    
    tagHelp =  "Touch tag to\n";
    tagHelp += "highlight other\n";
    tagHelp += "books in this\n";
    tagHelp += "category.";
    
    helpTextCol.set(224, 218, 203);
    
    //will draw relative to "currentPos"
    float helpTextSpace = 8;
    tagHelpPos.set(0, buttonHeight + helpTextSpace + font -> stringHeight("A"), 0);
    
}


void BookUIButton::checkForClicks(int x, int y, bool touchState){
    
    //make sure it has been long enough AND the button
    //is in the proper state to be interacted with
    if( ofGetElapsedTimeMillis() - lastButtonPress > debounceTime &&  !bIsUnavailable && bIsDisplayed){
        
        //check if we're in the button
        bool isInside;
        
        //different bounds for different buttons
        if(type < 3){
            
            isInside = x > currentPos.x - buttonWidth/2 &&
                       x < currentPos.x + buttonWidth/2 &&
                       y > currentPos.y - buttonHeight/2 &&
                       y < currentPos.y + buttonHeight/2;
            
        } else {
            isInside = x > currentPos.x && x < currentPos.x + buttonWidth &&
                       y > currentPos.y && y < currentPos.y + buttonHeight;
            
        }
        
        
        if( isInside ){
            
            //if the touch state is pressed or not pressed:
            if(touchState){
                
                //TOUCHING
                
                buttonState = true;
                lastButtonPress = ofGetElapsedTimeMillis();
                
                if(type < 3){
                    pushButtonScale = 0.8;
                } else {
                    //no need to shrink the tag button as much
                    pushButtonScale = 0.9;
                }
                
                if(type == 0 || type == 3){
                    
                    ButtonEvent e;
                    e.type = type;
//                    e.tag = tag;
                    e.tagNum = tagNum;
                    e.bookNum = bookNum;
                    e.shelfNum = shelfNum;
                    e.tagCol = tagCol;
                    e.bIsCenterBookButton = bIsCenterBookButton;
                    e.bScholarButton = bScholarButton;
                    e.src = ofVec3f(currentPos.x + buttonWidth/2, currentPos.y + buttonHeight/2, -90);
                    
                    ofNotifyEvent(newButtonClickEvt, e, this);
                    
                    
                    
                }

            } else {
                
                //HOVERING
                hoverTrans = ofLerp(hoverTrans, 255.0f, 0.1f);
                lastHoverTime = ofGetElapsedTimeMillis();
                bIsHovering = true;
            }

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
    
        
        currentPos.interpolate(hiddenPos, buttonLerpSpeed);
        tagHelpTrans = ofLerp(tagHelpTrans, 0.0f, 0.06);
        tagHelpScale = ofLerp(tagHelpScale, 0.0f, 0.1);
        
        //if we're really close to the hidden pos, hide the button
        ofVec3f between = currentPos - hiddenPos;
        if(between.lengthSquared() < 1){
            bIsHidden = true;
        }
        
    } else {
        
        
        currentPos.interpolate(displayedPos, buttonLerpSpeed);
        bIsHidden = false;
        
        //nav buttons get logic for hovering
        //tag button gets scale and transparency logic
        if(type < 3){
            
            //if it has been a little while since the last hover
            //start lerping back to transparent
            if(ofGetElapsedTimeMillis() - lastHoverTime > 50){
                hoverTrans = ofLerp(hoverTrans, 0.0, 0.1);
            }
            
            //if we're back to zero transparency, we're not hovering anymore
            if(hoverTrans < 0.01){
                bIsHovering = false;
            }
            

        } else {
            
            tagHelpTrans = ofLerp(tagHelpTrans, 255.0f, 0.1);
            tagHelpScale = ofLerp(tagHelpScale, 1.0f, 0.1);
        }
    
        //if the button's been pushed, start to make it bigger again
        if(pushButtonScale < 1.0){
            pushButtonScale += 0.02;
        }

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
        
            ofPushMatrix();
            ofTranslate(currentPos.x, currentPos.y, -70);
            ofScale(pushButtonScale, pushButtonScale);
            
            //draw image flipped so, shift it down first
            buttonIcon -> draw(0, 0, 0, buttonWidth, -buttonHeight);
            
            //draw the hovered image on top
            
            if(!bIsUnavailable && bIsHovering){
                ofSetColor(255, hoverTrans);
                hoverIcon -> draw(0, 0, -1, buttonWidth, -buttonHeight);
            }
            
//            ofSetColor(255, 0, 255);
//            ofDrawCircle(0, 0, 0);
            
            ofPopMatrix();
            
        } else {
            //type 3 is the tag button
            ofPushMatrix();{
                ofTranslate(currentPos.x, currentPos.y, -70);
                ofScale(tagHelpScale, 1);
                
                //buttons draw from center so translate to center then draw centered rects
                ofPushStyle();
                ofPushMatrix();{
                   
                    ofTranslate(buttonWidth/2, buttonHeight/2);
                    ofScale(pushButtonScale, pushButtonScale);
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    
                    ofFill();
                    ofSetColor(tagCol, tagHelpTrans);
                    ofDrawRectangle(0, 0, buttonWidth, buttonHeight);
                    
                    ofNoFill();
                    ofSetColor(tagOutlineCol, tagHelpTrans);
                    ofSetLineWidth(4);
                    ofDrawRectangle(0, 0, buttonWidth, buttonHeight);
                    
                    ofFill();
                    
                }ofPopMatrix();
                
                ofPopStyle();
                //draw the first line of text
                
                
                ofPushMatrix();
                ofTranslate(tagLine1Pos.x, tagLine1Pos.y, -5);
                if(!bIsCenterBookButton) ofScale(1, -1);
                ofSetColor(tagOutlineCol, tagHelpTrans);
                font -> drawString(tagLine1, 0, 0);
                ofPopMatrix();
                
                //then the second if needed
                if(linesInTag > 1){
                    ofPushMatrix();
                    ofTranslate(tagLine2Pos.x, tagLine2Pos.y, -5);
                    if(!bIsCenterBookButton) ofScale(1, -1);
                    
                    font -> drawString(tagLine2, 0, 0);
                    
                    ofPopMatrix();
                }
                
                //draw tag help text
                if(!bIsCenterBookButton){

                    ofPushMatrix();
                    ofTranslate(tagHelpPos.x, tagHelpPos.y, -8);

                    //draw a rect where the help text will be but pushed back a bit so the text draws in front
                    //this is needed to prevent weirdness between shapes drawn as textures
                    //over the semi transparent shelf overlay
                    ofSetColor(0, tagHelpTrans);
                    ofDrawRectangle(0, -15, 2, buttonWidth, 80);
                    
                    ofScale(0.7, -0.7);
                    ofSetColor(helpTextCol, tagHelpTrans);
                    font -> drawString(tagHelp, 0, 0);
                    
                    ofPopMatrix();

                }
                
            }ofPopMatrix();
            
        }
        
        
    }

    
    
}







