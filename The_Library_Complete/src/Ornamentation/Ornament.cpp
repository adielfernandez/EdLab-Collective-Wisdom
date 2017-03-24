//
//  Ornament.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/9/17.
//
//

#include "Ornament.hpp"


Ornament::Ornament(){
    
}

void Ornament::setup(int _type){
    
    type = _type;
    
    if(type == 0){              //-----FIRST SHELF-----
        numBooksWide = 7;
        placeOnShelf = 2;
        shelfNum = 0;
        
        img.load("assets/ornaments/clock.png");
        
        leftPadding = bookThickness * 0.5;
        rightPadding = bookThickness * 0.5;
        
    } else if(type == 1){              //-----SECOND SHELF-----
        numBooksWide = 3;
        placeOnShelf = 0;
        shelfNum = 1;
        
        img.load("assets/ornaments/leftBookend.png");
        
        //against book on right side
        leftPadding = bookThickness * 0.5;
        rightPadding = 0;
        
    } else if(type == 2){              //-----THIRD SHELF-----
        numBooksWide = 6;
        placeOnShelf = 10;
        shelfNum = 2;
        
    
        img.load("assets/ornaments/globe.png");
        
        //full book space on either side
        leftPadding = bookThickness;
        rightPadding = bookThickness;
        
    } else if(type == 3){              //-----FOURTH SHELF-----
        
        numBooksWide = 8;
        placeOnShelf = 5;
        shelfNum = 3;
        
        img.load("assets/ornaments/ship.png");
        
        //full book space on either side
        leftPadding = bookThickness * 0.5;
        rightPadding = bookThickness * 0.5;
        
    } else if(type == 4){              //-----FIFTH SHELF-----
        numBooksWide = 3;
        placeOnShelf = 14;
        shelfNum = 4;
        
        img.load("assets/ornaments/rightBookend.png");
        
        //against book on left side
        leftPadding = 0;
        rightPadding = bookThickness * 0.5;
        
    } else if(type == 5){              //-----SIXTH SHELF-----
        
        numBooksWide = 5;
        placeOnShelf = 2;
        shelfNum = 5;
        
        
        img.load("assets/ornaments/vase.png");
        
        //full book space on either side
        leftPadding = bookThickness;
        rightPadding = bookThickness;
        
    }
    
    img.setAnchorPercent(0.5f, 0.5f);
    
    
    
    gapWidth = bookThickness * numBooksWide;
    gapHeight = 140;


    
    imgWidth = gapWidth - leftPadding - rightPadding;
    imgHeight = imgWidth * (img.getHeight()/img.getWidth());
    
    triggered = false;
    scale = 1.0f;
}



void Ornament::setShelfPosition(ofVec3f shelfPos){
    float z = -40; 
    
    pos = ofVec3f(shelfPos.x, shelfPos.y, z);
}


void Ornament::checkForClicks(int x, int y, bool state){
    
    //region is defined from lower left corner then
    //"gapWidth" to the right and "gapHeight" above
    if(x > pos.x + leftPadding && x < pos.x + gapWidth - rightPadding && y < pos.y && y > pos.y - imgHeight){

        //if we're being clicked, not hovered over
        if( state ){
            
            //only trigger on bookends (for now}
//            if(type == 1 || type == 4){
            
                SceneEvent se;
                
                //pos at center of ornament
                se.pos = pos + ofVec2f(gapWidth/2, -gapHeight/2);
                
                //determine the event type from which ornament it is
                switch (type) {
                    case 0:
                        se.type = SceneEvent::WALLPAPER;
                        break;
                    case 1:
                        se.type = SceneEvent::BOOKCASE_LEFT;
                        break;
                    case 2:
                        se.type = SceneEvent::FRAME;
                        break;
                    case 3:
                        se.type = SceneEvent::WALLPAPER;
                        break;
                    case 4:
                        se.type = SceneEvent::BOOKCASE_RIGHT;
                        break;
                    case 5:
                        se.type = SceneEvent::FRAME;
                        break;
                    default:
                        break;
                }
                
                ofNotifyEvent(sceneChangeEvent, se, this);
                
                triggered = true;
                triggerTime = ofGetElapsedTimef();
//            }
            
        } else {
            
            //hover state
            
            
        }
        
    }


    
    
    
    
    
    
}


void Ornament::triggerAnimation(){
    
}

void Ornament::update(){
    
    if( triggered ){
        
        float duration = 0.75;
        float now = ofGetElapsedTimef();
        float maxScale = 1.4;
        
        //ease bigger for half the time then ease back to normal
        if( now - triggerTime < duration/2.0f){
            
            scale = ofxeasing::map_clamp(now, triggerTime, triggerTime + duration, 1.0, maxScale, &ofxeasing::quint::easeOut);
            
        } else if( now - triggerTime < duration){

            scale = ofxeasing::map_clamp(now, triggerTime, triggerTime + duration, maxScale, 1.0, &ofxeasing::quint::easeIn);
            
        } else {
            triggered = false;
        }
        
        
    }
    
    
}

void Ornament::draw(){
    
    //push forward
    ofPushMatrix();
    ofTranslate(pos.x + leftPadding + imgWidth/2, pos.y - imgHeight/2, -55);
    ofScale(scale, -scale);
    
    
    ofSetColor(255);
    img.setAnchorPercent(0.5, 0.5);
    img.draw(0, 0, imgWidth, imgHeight);
//    img.draw(pos.x + leftPadding, pos.y, pos.z, imgWidth, -imgHeight);

    
    ofPopMatrix();
    
    //draw outline for debug
//    ofPushStyle();
//    ofNoFill();
//    ofSetLineWidth(3);
//    ofSetColor(0, 255, 0);
//    ofDrawRectangle(pos.x, pos.y, pos.z, gapWidth, -gapHeight);
//    ofPopStyle();
  

    
    
}