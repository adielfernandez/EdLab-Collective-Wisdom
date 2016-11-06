//
//  Wireframe.hpp
//  Visuals_Test
//
//  Created by Adiel Fernandez on 10/23/16.
//
//

#ifndef Wireframe_hpp
#define Wireframe_hpp

#include <stdio.h>

#endif /* Wireframe_hpp */

#include "ofMain.h"
#pragma once

class Wireframe{
    
public:
    
    Wireframe();
    
    void setup();
    void update();
    void draw();
    
    float transparency;
    ofPolyline lines;
    
    
    
};