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
#include "WallTouch.hpp"

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
    

    
    void setup(string _camName, const char* deviceName);
    void update();
    void setupGui();
    
    void drawRaw(int x, int y);
    void drawThresh(int x, int y, bool bDrawShifted);
    void drawSceneProxy(int x, int y);
    
    void drawGui(int x, int y);
    
    
    float camFrameRate;
    
    const int camWidth = 640;
    const int camHeight = 480;
    
    unsigned long long lastFrameTime;
    float lastFrameRate; //for smoothing

    //for learning the background
    //X seconds after startup
    bool bLearnBackground;
    
    //Flag for triggering new
    //data to send via OSC
    bool bNewContours;
    bool bNewTouchesToSend;
    
    //calibration routine
    bool bCalibrating;
    bool bRecording;
    int calibratingPoint;
    
    vector<float> calibratingValues;
    int numCalibrationSamples;
    int lastNumSamples;
    
    float samplingDuration;
    double samplingStartTime;
    
    //members safe to access
    //from GL thread
    ofPixels rawPix;
    ofPixels threshPix;
    ofxCv::ContourFinder contours;
    
    //touch data we'll send elsewhere
    vector<WallTouch> touches;
    vector<ofVec2f> lowPoints;
    
    float thresholdPos;
    
    
    //GUI SETTINGS
    string filePath;
    string camName;
    void saveSettings();
    void loadSettings();
    
    ofxPanel gui;
    
    ofxLabel cameraLabel;
    ofxIntSlider nearClipSlider;
    ofxIntSlider farClipSlider;
    
    ofxLabel cvLabel;
    ofxIntSlider nearThreshSlider;
    ofxIntSlider farThreshSlider;
    ofxIntSlider blurAmountSlider;
    ofxIntSlider numErosionsSlider;
    ofxIntSlider numDilationsSlider;
    ofxToggle rotateImageToggle;

    ofxLabel roiLabel;
    ofxIntSlider roiDepthSlider;
    ofxIntSlider wallCutoffSlider;
    ofxIntSlider wallHitDistSlider;

    ofxLabel bgDiffLabel;
    ofxIntSlider learningTimeSlider;
    ofxButton resetBGToggle;
    ofxToggle useBgDiff;
    
    ofxLabel contoursLabel;
    ofxToggle drawContoursToggle;
    ofxToggle drawBlobInfoToggle;
    ofxIntSlider minBlobAreaSlider;
    ofxIntSlider maxBlobAreaSlider;
    ofxIntSlider persistenceSlider;
    ofxIntSlider maxDistanceSlider;
    
    ofxLabel controlPointsLabel;
    ofxButton calibrateButton;
    ofxButton cancelButton;
    
    ofxIntSlider firstDepthSlider;
    ofxIntSlider secondDepthSlider;
    ofxIntSlider thirdDepthSlider;
    ofxIntSlider fourthDepthSlider;
    
    ofxIntSlider firstXLeftSlider;
    ofxIntSlider firstXRightSlider;
    
    ofxIntSlider secondXLeftSlider;
    ofxIntSlider secondXRightSlider;
    
    ofxIntSlider thirdXLeftSlider;
    ofxIntSlider thirdXRightSlider;
    
    ofxIntSlider fourthXLeftSlider;
    ofxIntSlider fourthXRightSlider;
    
    ofxLabel touchSettingsLabel;
    ofxIntSlider touchSearchAreaSlider;
    ofxIntSlider posSmoothingSlider;
    ofxIntSlider distSmoothingSlider;
    
    unsigned long long lastFrameToThread;
    
    
    
private:
    
    
    unsigned long long frameNum;
    
    ofxOrbbecAstra camera;

    //thread management
    void closeAllChannels();
    void emptyAllChannels();
    bool isThreadCrashed;
    
    //into thread
    ofThreadChannel<vector<int>> settingsIn;
    ofThreadChannel<ofShortPixels> rawShortPixIn;
    
    //Thread output
    ofThreadChannel<ofPixels> rawPixOut;
    ofThreadChannel<ofPixels> threshPixOut;
    ofThreadChannel<ofxCv::ContourFinder> contoursOut;
    
    
    ofxCv::RunningBackground background;
    
    //for restarting the background learning
    bool needsAutoReset;
    int lastRoiDepth;
    
    //for restarting the thread
    unsigned long long lastRestartTime;
    bool firstAfterCrash;
    bool firstStop;
    
    
    void threadedFunction();
    
};
