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
        placeOnShelf = 3;
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
        numBooksWide = 5;
        placeOnShelf = 8;
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
        placeOnShelf = 13;
        shelfNum = 4;
        
        img.load("assets/ornaments/rightBookend.png");
        
        //against book on left side
        leftPadding = 0;
        rightPadding = bookThickness * 0.5;
        
    } else if(type == 5){              //-----SIXTH SHELF-----
        
        numBooksWide = 5;
        placeOnShelf = 3;
        shelfNum = 5;
        
        
        img.load("assets/ornaments/vase.png");
        
        //full book space on either side
        leftPadding = bookThickness;
        rightPadding = bookThickness;
        
    }
    
    
    
    
    
    gapWidth = bookThickness * numBooksWide;
    gapHeight = 140;


    
    imgWidth = gapWidth - leftPadding - rightPadding;
    imgHeight = imgWidth * (img.getHeight()/img.getWidth());
    
}



void Ornament::setShelfPosition(ofVec3f shelfPos){
    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), -40, -100);
    pos = ofVec3f(shelfPos.x, shelfPos.y, x);
}

void Ornament::triggerAnimation(){
    
}

void Ornament::update(){
    
}

void Ornament::draw(){
    
    pos = ofVec3f(pos.x, pos.y, -55);
    
    ofPushStyle();

    ofSetColor(255);
    img.draw(pos.x + leftPadding, pos.y, pos.z, imgWidth, -imgHeight);


//    ofNoFill();
//    ofSetLineWidth(3);
//    ofSetColor(0, 255, 0);
//    ofDrawRectangle(pos.x, pos.y, pos.z, gapWidth, -gapHeight);
  

    
    ofPopStyle();
    
}