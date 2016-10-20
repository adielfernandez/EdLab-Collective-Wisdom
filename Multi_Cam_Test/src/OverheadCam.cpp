//
//  OverheadCam.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#include "OverheadCam.h"


OverheadCam::OverheadCam(){
    
}

OverheadCam::~OverheadCam(){
    
}

void OverheadCam::setup(const char* deviceName){
    
    camera.setup(deviceName);
    camera.enableRegistration(false);
    
    camera.initDepthStream();
    camera.setDepthClipping(300, 5000);
    
    
}

void OverheadCam::update(){
    
    camera.update();
    
}

void OverheadCam::threadedFunction(){
    
    while(isThreadRunning()){
        
        
        
        
        
        
    }
    
}