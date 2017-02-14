//
//  Ornament.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/9/17.
//
//

#ifndef Ornament_hpp
#define Ornament_hpp

#include <stdio.h>

#endif /* Ornament_hpp */

#include "ofMain.h"
#include "../SceneController.hpp"

#pragma once


class Ornament{
    
public:
    
    Ornament();
    
    void setup(int _type);
    void setShelfPosition(ofVec3f shelfPos);
    void update();
    
    void triggerAnimation();
    
    void checkForClicks(int x, int y, bool state);
    
    void draw();

    int type;
    float gapWidth;
    float gapHeight;
    
    ofImage img;
    float imgWidth, imgHeight;
    
    float leftPadding, rightPadding;
    
    ofVec3f pos;
    int numBooksWide;
    
    //place the item after book
    //number X on the shelf
    int placeOnShelf;
    
    int shelfNum;
    
    //thickness of one book on the shelf
    const float bookThickness = 24.2906;
    
    ofEvent<SceneEvent> sceneChangeEvent;
    
    double triggerTime;
    bool triggered;
    float scale;
    
};



