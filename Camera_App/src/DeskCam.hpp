//
//  DeskCam.hpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 12/15/16.
//
//

#ifndef DeskCam_hpp
#define DeskCam_hpp

#include <stdio.h>

#endif /* DeskCam_hpp */


#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOrbbecAstra.h"
#include "ofxGui.h"
#include "DeskTouch.hpp"

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

class DeskCam: public ofThread{
    
public:
    
    DeskCam();
    ~DeskCam();
    
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    
    void setup(string _camName, const char* deviceName);
    void update();
    void setupGui();
    
    void drawRaw(int x, int y);
    void drawThresh(int x, int y);
    
    
    void drawGui(int x, int y);
    
    
    float camFrameRate;
    
    const int camWidth = 640;
    const int camHeight = 480;
    
    unsigned long long lastFrameTime;
    float lastFrameRate; //for smoothing
    
    //Masking
    vector<ofVec2f> maskPoints;
    vector<bool> maskPointMouseLock;
    float maskPointRad;
    ofVec2f adjustedMouse;
    ofPath mask;
    
    //members safe to access
    //from GL thread
    ofPixels rawPix;
    ofPixels threshPix;
    ofxCv::ContourFinder contours;
    
    //touch data we'll send elsewhere
    vector<DeskTouch> touches;
    
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
    ofxToggle flipImageToggle;
    
    ofxLabel maskingLabel;
    ofxVec2Slider maskPt0;
    ofxVec2Slider maskPt1;
    ofxVec2Slider maskPt2;
    ofxVec2Slider maskPt3;
    
    ofxLabel bgDiffLabel;
    ofxIntSlider learningTimeSlider;
    ofxButton resetBG;
    ofxToggle useBgDiff;
    
    ofxLabel contoursLabel;
    ofxToggle drawContoursToggle;
    ofxToggle drawBlobInfoToggle;
    ofxIntSlider minBlobAreaSlider;
    ofxIntSlider maxBlobAreaSlider;
    ofxIntSlider persistenceSlider;
    ofxIntSlider maxDistanceSlider;
    
    
    unsigned long long lastFrameToThread;
    
    
    
private:
    
    
    unsigned long long frameNum;
    
    ofxOrbbecAstra camera;
    
    static void getQuadSubPixels(ofPixels& inPix, ofPixels& outPix, vector <ofVec2f>& quad);
    
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