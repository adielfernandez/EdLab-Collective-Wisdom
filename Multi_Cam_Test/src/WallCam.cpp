//
//  WallCam.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#include "WallCam.h"


WallCam::WallCam(){
    
}

WallCam::~WallCam(){
    
    closeAllChannels();
    
    waitForThread(true, 4000);
    
}

void WallCam::closeAllChannels(){
    
    rawPixOut.close();
    threshPixOut.close();
    contoursOut.close();
    
}

void WallCam::setup(string _camName, const char* deviceName){
    
    setupGui();
    
    camName = _camName;
    
    camera.setup(deviceName);
    camera.enableRegistration(false);
    
    camWidth = 640;
    camHeight = 480;
    
    camera.initDepthStream();
    nearClip = 500;
    farClip = 3000;
    
    camera.setDepthClipping(nearClip, farClip);
    camera.enableDepthImage(false);
    
    rawPix.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    frameNum = 0;
    
    camFrameRate = 0;
    lastFrameTime = 0;
    
    startThread();
    
}

void WallCam::setupGui(){

    gui.setup(camName, camName + ".xml", 0, 0);
    
    gui.add(cvLabel.setup("   IMAGE PROCESSING", ""));
    gui.add(blurAmountSlider.setup("Blur", 1, 0, 40));
    gui.add(nearThreshSlider.setup("Near Threshold", 255, 0, 255));
    gui.add(farThreshSlider.setup("Far Threshold", 0, 0, 255));
    gui.add(useThresholdsToggle.setup("Use threshold", true));
    

    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    cvLabel.setBackgroundColor(ofColor(255));

    
    //this changes the color of all the labels for some reason
    cvLabel.setDefaultTextColor(ofColor(0));
    
}

void WallCam::update(){
    
    //attempt to receive data from thread
    ofPixels p;
    if(rawPixOut.tryReceive(p)){
        rawPix = p;
    }
    
    ofPixels t;
    if(threshPixOut.tryReceive(t)){
        threshPix = t;
        
        float thisFrameRate = 1.0/( (ofGetElapsedTimeMillis() - lastFrameTime) / 1000.0 );
        
        //average this framerate with the last one to smooth out numbers
        //and get a better reading.
        camFrameRate = (thisFrameRate + lastFrameRate)/2;
        
        lastFrameRate = thisFrameRate;
        
        lastFrameTime = ofGetElapsedTimeMillis();

    }
    
    
    


    
    
    //send settings into thread

    camera.update();
//    if(camera.isFrameNew()){
    
    //hack to not flood thread with duplicate images until
    //we get isFrameNew() working
    if(ofGetElapsedTimeMillis() - lastFrameToThread > 34){  //34 ms between frames = ~30fps
        
        rawShortPixIn.send(camera.getRawDepth());
        
        //get settings from gui
        vector<int> settings;
        settings.resize(4);
        
        settings[0] = farThreshSlider;
        settings[1] = nearThreshSlider;
        settings[2] = useThresholdsToggle;
        settings[3] = blurAmountSlider;
        
        settingsIn.send(settings);
        
        lastFrameToThread = ofGetElapsedTimeMillis();
    }
    
    
    
    
}

void WallCam::drawGui(int x, int y){
    
    gui.setPosition(x, y);
    gui.draw();
    
    
}


void WallCam::threadedFunction(){
    
    while(isThreadRunning()){
        
//        camera.update();
        
        vector<int> settings_thread;
        ofShortPixels rawShortPix_thread; //short pix coming in
        
        if(rawShortPixIn.receive(rawShortPix_thread) && settingsIn.receive(settings_thread)){
        
            //Local data inside thread
//            ofShortPixels rawShortPix_thread = camera.getRawDepth(); //short pix coming in
            ofPixels rawPix_thread;      //regular pix going out
            
            ofPixels threshPix_thread, blurredPix_thread;
            ofxCv::ContourFinder contours_thread;

            //get all the values from the settings vector
            int farThresh = settings_thread[0];
            int nearThresh = settings_thread[1];
            bool useThresholds = settings_thread[2];
            int blurAmount = settings_thread[3];
            
//            cout << "Frame Num: " << frameNum << ", " << farThresh << ", " << nearThresh << ", " << useThresholds << ", " << blurAmount << endl;
            
            rawPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            threshPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            blurredPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            
            
            //Convert from ofShortPixels to ofPixels and put into rawPix_thread
            for(int i = 0; i < rawShortPix_thread.size(); i++){
                
                if(rawShortPix_thread.getColor(i).r == 0){
                    rawPix_thread[i] = 0;
                } else {
                    rawPix_thread[i] = ofMap( rawShortPix_thread.getColor(i).r , nearClip, farClip, 255, 0, true);
                }
                
            }
            
            //blur the new raw image
            ofxCv::GaussianBlur(rawPix_thread, blurredPix_thread, blurAmount);

            //threshold the blurred image
            if(useThresholds){
             
                for(int i = 0; i < blurredPix_thread.size(); i++){
                    
                    if(blurredPix_thread[i] > farThresh && blurredPix_thread[i] < nearThresh){
                        threshPix_thread[i] = 255;
                    } else {
                        threshPix_thread[i] = 0;
                    }
                    
                }
                
            } else {
                threshPix_thread = blurredPix_thread;
            }
            
            rawPixOut.send(std::move(blurredPix_thread));
            threshPixOut.send(std::move(threshPix_thread));
           
            frameNum++;
            
        }
    }
    
}



