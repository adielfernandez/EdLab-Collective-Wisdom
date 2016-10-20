//
//  WallCam.hpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#ifndef WallCam_h
#define WallCam_h

#include <stdio.h>

#endif /* WallCam_h */

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOrbbecAstra.h"
#include "ofxGui.h"

#pragma once

/*
 *  Thread Channels:
 *  
 *  INPUT:
 *      -CV variables:
 *          -Blur amt, threshold, etc.
 *
 *  OUTPUT:
 *      -Raw pixels (depth map converted from shortPixels)
 *      -Thresholded ofPixels
 *      -Contours
 */

class WallCam: public ofThread{
    
public:
    
    WallCam();
    ~WallCam();
    
    void closeAllChannels();
    
    void setup(string _camName, const char* deviceName);
    void update();
    void setupGui();
    void drawGui(int x, int y);
    void sendSettingsToThread();
    
    //members safe to access
    //from GL thread
    ofPixels rawPix;
    ofPixels threshPix;
    ofxCv::ContourFinder contours;
    
    float camFrameRate;
    
    int camWidth, camHeight;
    
    unsigned long long lastFrameTime;
    float lastFrameRate; //for smoothing

    string camName;
    
    ofxPanel gui;
    
    ofxLabel cvLabel;
    ofxIntSlider farThreshSlider;
    ofxIntSlider nearThreshSlider;
    ofxIntSlider blurAmountSlider;
    ofxToggle useThresholdsToggle;
    ofxIntSlider minBlobAreaSlider;
    ofxIntSlider maxBlobAreaSlider;
    
    unsigned long long lastFrameToThread;
    
    
    
private:
    
    int nearClip, farClip;
    
    long frameNum;
    
    ofxOrbbecAstra camera;
    
    //into thread
    ofThreadChannel<vector<int>> settingsIn;
    ofThreadChannel<ofShortPixels> rawShortPixIn;
    
    //Thread output
    ofThreadChannel<ofPixels> rawPixOut;
    ofThreadChannel<ofPixels> threshPixOut;
    ofThreadChannel<ofxCv::ContourFinder> contoursOut;
    

    
    
    void threadedFunction();
    
};
