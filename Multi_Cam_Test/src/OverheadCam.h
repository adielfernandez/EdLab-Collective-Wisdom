//
//  OverheadCam.hpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#ifndef OverheadCam_h
#define OverheadCam_h

#include <stdio.h>

#endif /* OverheadCam_h */

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOrbbecAstra.h"

#pragma once

class OverheadCam: public ofThread{
    
public:
    
    OverheadCam();
    ~OverheadCam();
    
    void setup(const char* deviceName);
    void update();
    
private:
    
    
    ofxOrbbecAstra camera;
    
    void threadedFunction();
    
    
};
