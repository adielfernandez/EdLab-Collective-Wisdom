//
//  BookUIButton.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/29/16.
//
//

#ifndef BookUIButton_hpp
#define BookUIButton_hpp

#include <stdio.h>

#endif /* BookUIButton_hpp */

#include "ofMain.h"
#include "ofxEasing.h"

#pragma once

class BookUIButton{
    
public:
    
    BookUIButton();
    
    void setup();
    void update();
    void draw();
    
    float radius;
    
    float pos;
    
    bool bHover;
    
    
    
    
};