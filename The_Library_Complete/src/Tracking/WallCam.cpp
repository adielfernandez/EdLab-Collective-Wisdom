//
//  WallCam.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#include "WallCam.hpp"

using namespace ofxCv;

WallCam::WallCam(){
    
}

WallCam::~WallCam(){
    
    closeAllChannels();
    
    waitForThread(true, 4000);
    
}

void WallCam::closeAllChannels(){
    
    settingsIn.close();
    rawShortPixIn.close();
    rawPixOut.close();
    threshPixOut.close();
    contoursOut.close();
    
}


void WallCam::emptyAllChannels(){
    
    //close thread channels
    settingsIn.empty();
    rawShortPixIn.empty();
    rawPixOut.empty();
    threshPixOut.empty();
    contoursOut.empty();
    
}

void WallCam::setup(string _camName, const char* deviceName){
    
    filePath = "settings/";
    camName = _camName;

    setupGui();
    loadSettings();
    
    camera.setup(deviceName);
    camera.enableRegistration(false);
    camera.initDepthStream();
    camera.enableDepthImage(false);
    
    
    rawPix.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    
    frameNum = 0;
    
    camFrameRate = 0;
    lastFrameTime = 0;
    
    needsAutoReset = true;
    isThreadCrashed = false;
    firstAfterCrash = true;
    
    firstRestart = true;
    
    lastRestartTime = 0;

    bCalibrating = false;
    numCalibrationSamples = 0;
    samplingDuration = 1.0; //in seconds
    samplingStartTime = 0;
    calibratingPoint = 0;

    bLearnBackground = true;
    
    bNewContours = false;
    bNewTouchesToSend = false;
    
    startThread();
    
    //Pretend we just got a frame so the thread doesn't stop
    //thinking it crashed since ofGetElapsedTimeMillis starts
    //long before the window opens (assets take a long time to load)
    lastFrameTime = ofGetElapsedTimeMillis();
    
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
        
        //
        lastFrameTime = ofGetElapsedTimeMillis();

    }
    
    ofxCv::ContourFinder c;
    if(contoursOut.tryReceive(c)){
        contours = c;
        
        //flag to analyze new contours
        bNewContours = true;
    }

    
    //---------------------------------------------------------------
    //---------------THREAD MANAGEMENT AND RESTARTING----------------
    //---------------------------------------------------------------
    
    if(ofGetElapsedTimeMillis() - lastFrameTime > 6000){
        isThreadCrashed = true;
        
        if(firstAfterCrash){
            cout << "[" << camName << " thread] Stopping Thread" << endl;
            stopThread();
            
            emptyAllChannels();
            
            background.reset();
            
            firstAfterCrash = false;
        }
        
    } else {
        
        isThreadCrashed = false;
        firstAfterCrash = true;
        
    }
    
    
    //only try to restart the thread every 4 seconds
    if(isThreadCrashed && ofGetElapsedTimeMillis() - lastRestartTime > 4000){
        
        cout << "[" << camName << " thread] Attempting to start thread..." << endl;
        startThread();
        
        lastRestartTime = ofGetElapsedTimeMillis();
        firstRestart = true;
        
    }
    
    //if it has been 2 seconds since last restart AND we're still crashed
    //then stop and prepare for the next restart
    //make sure to wait longer since we have waitForThread(4000)
    if(isThreadCrashed && ofGetElapsedTimeMillis() - lastRestartTime > 3000 && firstRestart){
        
        cout << "[" << camName << " thread] Stopping Thread" << endl;
        stopThread();
        
        emptyAllChannels();
        closeAllChannels();
        
        background.reset();
        
        firstRestart = false;
        
    }

    
    
    

    
    //---------------------------------------------------------------
    //--------------------MAP DEPTH TO WALL SPACE--------------------
    //---------------------------------------------------------------
    
    //only need to analyze data if we've have
    //newly updated contours from the thread
    if(bNewContours){
        
        bNewContours = false;
      
        thresholdPos = wallCutoffSlider - roiDepthSlider;
        
        //Go through previous touch data and set all touches to not updated
        for(int i = 0; i < touches.size(); i++){
            touches[i].bUpdated = false;
            touches[i].distForTouch = wallHitDistSlider;
            touches[i].numPosSmoothingPts = posSmoothingSlider;
            touches[i].numDistSmoothingPts = distSmoothingSlider;
        }
        
        
        if(contours.size() > 0){
            
            //go through all the contours
            for(int i = 0; i < contours.size(); i++){
                
                //get all the points in the blob so we can find the highest point
                //(lowest on screen) i.e. the closest point to the wall
                vector<ofPoint> points = contours.getPolyline(i).getVertices();
                
                //occasionally, contours will return polylines with zero vertices
                //so check for verts to avoid seg-fault
                if(points.size() > 0){
                    
                    //go through all the vertices and find the point with the lowest y value
                    int yVal = 0;
                    int index = 0;
                    for(int j = 0; j < points.size(); j++){
                        //if we find a point lower than yVal, store it
                        if(points[j].y > yVal){
                            yVal = points[j].y;
                            
                            //also store this index;
                            index = j;
                        }
                    }
                    
                    ofVec2f closestPoint(points[index].x, points[index].y);
                    
                    //since the depth at the edge pixel might not always be the true depth (at boundary of contour),
                    //look around the area just above the lowest point and
                    //pick the highest (closest) depth value as the depth reading
                    float touchDepth = -1;
                    
                    //search pixels above and to the sides, wider area gives more stable touch data
                    int pixelArea = touchSearchAreaSlider;
                    for(int x = closestPoint.x - pixelArea; x < closestPoint.x + pixelArea; x++){
                        for(int y = closestPoint.y - pixelArea; y < closestPoint.y; y++){
                            
                            //make sure the pixel is in bounds
                            if(x > 0 && x < camWidth && y > 0 && y < roiDepthSlider){
                                int thisIndex = rawPix.getPixelIndex(x, y + thresholdPos);
                                if(rawPix[thisIndex] > touchDepth) touchDepth = rawPix[thisIndex];
                            }
                            
                        }
                    }
                    
                    //we have a position from left to right and a depth.
                    //Now we need to map them to wall space
                    
                    //This mapping outputs normalized coordinates.
                    float nearYval = 0.0;
                    float farYval = 1.0;
                    
                    //Y is easy, as pixel brightness maps linearly to distance
                    float mappedY = ofMap(touchDepth, firstDepthSlider, fourthDepthSlider, nearYval, farYval);
                    
                    //X is more difficult since travel perpendicular to the camera FOV changes with distance
                    //Basically we will map the X differently from left to right using control points
                    //at different depths. Control point: The x component of the left and right screen
                    //pixels that correspond to a touch at each control point
                    
                    //These will be defined by the camera's x value when touching 8 points around
                    //the far edges of the bookcase interaction area: top, middle and bottom
                    //of the left and right sides
                    
                    //TO DO: Diagram this out and place in documentation
                    
                    
                    //Get the x value of the left and right bounds of the bookcase
                    //at this distance from the camera
                    float xLeftEdge, xRightEdge;
                    
                    //check which shelf we're in, based on distance from the camera
                    //first depth it top of first shelf
                    //fourth depth is bottom of last shelf
                    //(higher depth val = closer)
                    
                    //top shelf
                    if(touchDepth > secondDepthSlider){
                        xLeftEdge = ofMap(touchDepth, firstDepthSlider, secondDepthSlider, firstXLeftSlider, secondXLeftSlider);
                        xRightEdge = ofMap(touchDepth, firstDepthSlider, secondDepthSlider, firstXRightSlider, secondXRightSlider);
                        
                        //second shelf
                    } else if(touchDepth > thirdDepthSlider){
                        xLeftEdge = ofMap(touchDepth, secondDepthSlider, thirdDepthSlider, secondXLeftSlider, thirdXLeftSlider);
                        xRightEdge = ofMap(touchDepth, secondDepthSlider, thirdDepthSlider, secondXRightSlider, thirdXRightSlider);
                        
                        //third shelf
                    } else{
                        xLeftEdge = ofMap(touchDepth, thirdDepthSlider, fourthDepthSlider, thirdXLeftSlider, fourthXLeftSlider);
                        xRightEdge = ofMap(touchDepth, thirdDepthSlider, fourthDepthSlider, thirdXRightSlider, fourthXRightSlider);
                    }
                    
                    
                    
                    
                    //now we know the left and right bounds at any depth: xLeft and xRight
                    //so let's finally map them to a normalized scale
                    float mappedX = ofMap(closestPoint.x, xLeftEdge, xRightEdge, 0.0, 1.0);
                    
                    //get how far the touch is from the wall itself
                    float distFromWall = threshPix.getHeight() - closestPoint.y;
                    
                    //how far are we from the touch boundary
                    float distFromTouch = distFromWall - wallHitDistSlider;
                    
                    //check if a touch exists with the ID, if it does, update it, if not add it.
                    bool touchExists = false;
                    int existingIndex;
                    
                    for(int j = 0; j < touches.size(); j++){
                        if(touches[j].id == contours.getLabel(i)){
                            touchExists = true;
                            existingIndex = j;
                            break;
                        }
                    }
                    
                    if(touchExists){
                        //update the touch
                        touches[existingIndex].renewTouch(ofVec2f(mappedX, mappedY), distFromTouch);
                        
                        //store the raw coordinates of the low point for debug
                        touches[existingIndex].rawCamPos = closestPoint;
                        touches[existingIndex].rawDepth = touchDepth;
                        
                        //                    cout << "Touch[" << touches[existingIndex].id << "] updated" << endl;
                        
                    } else {
                        //add the new touch
                        WallTouch t;
                        t.setNewTouch(contours.getLabel(i), ofVec2f(mappedX, mappedY), distFromTouch);
                        
                        //store the raw coordinates of the low point for debug
                        t.rawCamPos = closestPoint;
                        t.rawDepth = touchDepth;
                        
                        touches.push_back(t);
                        
                        //                    cout << "Touch[" << t.id << "] added" << endl;
                        
                    }
                    
                    
                    
                    
                }
                
            }
            
        }
        
        //remove all the touches that haven't been update recently
        //starting from the end of the vector and moving to the front
        for(int i = touches.size() - 1; i >= 0; i--){
            
            if(!touches[i].bUpdated){
                touches.erase( touches.begin() + i );
            }
            
        }
        
        if( touches.size() > 0 ){
            bNewTouchesToSend = true;
        }
        
    }
    
    
    


    
    
    //send settings into thread

    camera.update();
    
    //hack to not flood thread with duplicate images
    //ofxOrbbecAstra isFrameNew() does not work with multiple cameras
    
//    if(camera.isFrameNew()){
    if(ofGetElapsedTimeMillis() - lastFrameToThread > 34){  //34 ms between frames = ~30fps
        
        rawShortPixIn.send(camera.getRawDepth());
        
        //get settings from gui
        vector<int> settings;
        settings.resize(18);

        settings[0] = nearClipSlider;
        settings[1] = farClipSlider;
        settings[2] = nearThreshSlider;
        settings[3] = farThreshSlider;
        settings[4] = blurAmountSlider;
        settings[5] = numErosionsSlider;
        settings[6] = numDilationsSlider;

        settings[7] = roiDepthSlider;
        settings[8] = wallCutoffSlider;
        
        settings[9] = learningTimeSlider;
        settings[10] = useBgDiff;    //bool casted as int into vector
        
        bool reset;
        
        if(ofGetFrameNum() > 30 && bLearnBackground){
            
            reset = true;
            bLearnBackground = false;
            
            cout << "Background reset!" << endl;
            
        } else {
            reset = resetBGToggle;
        }
        
        settings[11] = reset;    //bool casted as int into vector
        
        settings[12] = minBlobAreaSlider;
        settings[13] = maxBlobAreaSlider;
        settings[14] = persistenceSlider;
        settings[15] = maxDistanceSlider;
        
        settings[16] = rotateImageToggle;
        settings[17] = preThresholdToggle;
        
        settingsIn.send(settings);
        
        lastFrameToThread = ofGetElapsedTimeMillis();
    }
    
    //set values according to GUI
    
    camera.setDepthClipping(nearClipSlider, farClipSlider);
    
    if(calibrateButton && bCalibrating == false){
        bCalibrating = true;
        
        //initialize relevant vars
        calibratingPoint = 0;
        calibratingValues.clear();
        calibratingValues.assign(12, 0);
        numCalibrationSamples = 0;
        bRecording = false;
        
        
    }
    
    if(cancelButton){
        bCalibrating = false;
    }
    
    
}

void WallCam::drawRaw(int x, int y){
    
    ofSetColor(255);
    ofDrawBitmapString("Raw & Blurred Depth", x, y - 5);
    
    ofImage rawImg;
    rawImg.setFromPixels(rawPix);
    rawImg.draw(x, y);
    
    //draw frame
    float w = rawPix.getWidth();
    float h = rawPix.getHeight();
    
    
    
    ofPushStyle();

    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 100);
    ofDrawRectangle(x, y, w, h);
    
    //draw crosshairs for calibration
    ofPushMatrix();
    ofTranslate(x, y);
    
    ofDrawLine(w/2, 0, w/2, h);
    ofDrawLine(0, h/2, w, h/2);
    
    //draw green line at wall hit boundary
    ofSetColor(0, 255, 0);
    ofDrawLine(0, wallCutoffSlider - wallHitDistSlider, w, wallCutoffSlider - wallHitDistSlider);

    ofPopMatrix();
    
    
    float threshY = wallCutoffSlider - roiDepthSlider;
    //draw a red outline of where the ROI is
    ofSetLineWidth(1);
    ofSetColor(255, 0, 0);
    ofDrawRectangle(x, y + threshY, w, roiDepthSlider);
    
    ofPopStyle();
    
    //draw the raw depth value at that position
    if(ofGetMouseX() > x && ofGetMouseX() < x + w && ofGetMouseY() > y && ofGetMouseY() < y + h){
        ofSetColor(255);
        
        float depth = rawPix[rawPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y)];
        ofDrawBitmapStringHighlight("D: " + ofToString(depth), ofGetMouseX() + 20, ofGetMouseY() - 20);
        
        
    }
    
    
}

//bDrawShifted = false draws at X and Y
//true draws as if it were overlaying on top of the raw image
void WallCam::drawThresh(int x, int y, bool bDrawShifted){
 
    ofSetColor(255);
    ofDrawBitmapString("Cropped and thresholded", x, y - 5);
    
    ofImage threshImg;
    threshImg.setFromPixels(threshPix);
    
    if(bDrawShifted){
        threshImg.draw(x, y + thresholdPos);
    } else {
        threshImg.draw(x, y);
    }
    
    float w = threshPix.getWidth();
    float h = threshPix.getHeight();
    
    ofPushStyle();
    ofPushMatrix();

    if(bDrawShifted){
        ofTranslate(x, y + thresholdPos);
    } else {
        ofTranslate(x, y);
    }
    
    //draw a light frame
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, w, h);
    
    //draw green line at wall hit boundary
    ofSetColor(0, 255, 0);
    ofDrawLine(0, h - wallHitDistSlider, w, h - wallHitDistSlider);
    
    //draw crosshairs for calibration
    ofSetColor(255, 100);
    ofDrawLine(w/2, 0, w/2, h);
    ofDrawLine(0, h/2, w, h/2);


    
    if(drawContoursToggle){
        
        //draw contours
        ofSetLineWidth(1.0);
        ofSetColor(255, 0, 0);
        contours.draw();
        
        
        
        if(drawBlobInfoToggle){
            
            //go through and draw blob data too
            for(int i = 0; i < contours.size(); i++) {
                
                int label = contours.getLabel(i);
                ofPoint c = toOf(contours.getCenter(i));
                
                ofFill();
                ofSetColor(0, 255, 0);
                ofDrawCircle(c.x, c.y, 5, 5);
    
                string msg = ofToString(label) + " : " + ofToString(c.x) + ", " + ofToString(c.y);
                ofDrawBitmapStringHighlight(msg, c.x + 5, c.y);
            }
        
            
        }
        
        //draw the low points of the touches
        ofSetColor(0, 0, 255);
        for(int j = 0; j < touches.size(); j++){
            ofDrawCircle(touches[j].rawCamPos, 5);
            ofDrawBitmapStringHighlight(ofToString(touches[j].id) + " : " + ofToString(touches[j].rawCamPos), touches[j].rawCamPos.x + 20, touches[j].rawCamPos.y);
            
            //draw the touch search area for the finger point
            ofNoFill();
            ofSetColor(0, 255, 255);
            ofDrawRectangle(touches[j].rawCamPos.x - touchSearchAreaSlider, touches[j].rawCamPos.y, touchSearchAreaSlider * 2, -touchSearchAreaSlider);
            
        }
        
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    //draw the raw depth value at mouse position
    if(ofGetMouseX() > x && ofGetMouseX() < x + w && ofGetMouseY() > y && ofGetMouseY() < y + h){
        ofSetColor(255);
        
        float depth = rawPix[rawPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y + roiDepthSlider)];
        ofDrawBitmapStringHighlight("Raw Depth: " + ofToString(depth), ofGetMouseX() - 20, ofGetMouseY() - 20);
        
    }
    
}

void WallCam::drawSceneProxy(int x, int y){
    
    //touches are normalized, choose random rect dimensions
    //that more or less look like the bookcase
    float rectW = 500;
    float rectH = 520;
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x, y);
    
    //big rect
    ofSetColor(255, 200, 0);
    ofNoFill();
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0, rectW, rectH);
    
    

    
    //draw control point positions
    vector<ofVec2f> controlPoints;
    controlPoints.resize(12);
    
    //down the center
    controlPoints[0] = ofVec2f(rectW * 0.5, rectH * 0.0);
    controlPoints[1] = ofVec2f(rectW * 0.5, rectH * 0.3333);
    controlPoints[2] = ofVec2f(rectW * 0.5, rectH * 0.6666);
    controlPoints[3] = ofVec2f(rectW * 0.5, rectH * 1.0);

    //left side
    controlPoints[4] = ofVec2f(rectW * 0.0, rectH * 0.0);
    controlPoints[5] = ofVec2f(rectW * 0.0, rectH * 0.3333);
    controlPoints[6] = ofVec2f(rectW * 0.0, rectH * 0.6666);
    controlPoints[7] = ofVec2f(rectW * 0.0, rectH * 1.0);
    
    //right side
    controlPoints[8] = ofVec2f(rectW * 1.0, rectH * 0.0);
    controlPoints[9] = ofVec2f(rectW * 1.0, rectH * 0.3333);
    controlPoints[10] = ofVec2f(rectW * 1.0, rectH * 0.6666);
    controlPoints[11] = ofVec2f(rectW * 1.0, rectH * 1.0);
    
    ofFill();
    for(int i = 0; i < controlPoints.size(); i++){
        
        if(bCalibrating && i == calibratingPoint){
            ofSetColor(255, 0, 0);
            ofDrawCircle(controlPoints[i], 8);
        } else {
            ofSetColor(255, 200, 0);
            ofDrawCircle(controlPoints[i], 4);
        }
        
        
        
    }
    
    //draw thin rectangles to represent the shelf dividers
    
    int shelfThickness = 20;
    
    ofPolyline shelf1;
    shelf1.addVertex(controlPoints[5]);
    shelf1.addVertex(controlPoints[9]);
    shelf1.addVertex(controlPoints[9] + ofVec2f(0, shelfThickness));
    shelf1.addVertex(controlPoints[5] + ofVec2f(0, shelfThickness));
    shelf1.close();
    shelf1.draw();

    ofPolyline shelf2;
    shelf2.addVertex(controlPoints[6]);
    shelf2.addVertex(controlPoints[10]);
    shelf2.addVertex(controlPoints[10] + ofVec2f(0, shelfThickness));
    shelf2.addVertex(controlPoints[6] + ofVec2f(0, shelfThickness));
    shelf2.close();
    shelf2.draw();
    
    //draw touch data
    for(int i = 0; i < touches.size(); i++){
        
        ofVec2f p = touches[i].pos;
        
        //convert from normalized
        p.x *= rectW;
        p.y *= rectH;
        
        float rad = ofMap(touches[i].dist, 0, 70, 7, 50, true);
        
        if(touches[i].bIsTouching){
            ofSetColor(0, 255, 0);
            
        } else {
            ofSetColor(0, 128, 255);
        }
        
        ofSetLineWidth(3);
        ofNoFill();
        ofDrawCircle(p, rad);
        
        ofDrawBitmapStringHighlight(ofToString(touches[i].pos), p);
        
    }
    
    if(bCalibrating){
    
        //Calibration routine text
        string msg = "";
        
        msg += "Calibration Routine\n";
        msg += "-------------------\n";
        msg += "(Make sure no other blobs are present)\n";
        msg += "\n";

        if(calibratingPoint > 0){
            
            msg += "Point[" + ofToString(calibratingPoint - 1) + "] set to: " + ofToString(calibratingValues[calibratingPoint - 1]) + " from " + ofToString(lastNumSamples) + " samples\n";
            msg += "\n";
            
        }
        
        msg += "Calibrating Point: " + ofToString(calibratingPoint) + "\n";
        msg += "Place finger at point, press any key\n";
        msg += "and HOLD finger in place until finished averaging.\n";

        

        if(!bRecording && ofGetKeyPressed()){
            bRecording = true;
            samplingStartTime = ofGetElapsedTimef();
        }
        
        if(bRecording){
            
            if(ofGetElapsedTimef() - samplingStartTime < samplingDuration){

                //first four points are the center of the bookcase
                //the value we store is the depth
                if(calibratingPoint < 4){
                    
                    calibratingValues[calibratingPoint] += touches[0].rawDepth;
                    numCalibrationSamples++;
                    
                } else {
                    //all other points are the x coordinate of the touch's raw camera position
                    calibratingValues[calibratingPoint] += touches[0].rawCamPos.x;
                    numCalibrationSamples++;
                    
                }
                
            } else {
                
                //done recording, store the value then move to the next point
                
                //get average value
                calibratingValues[calibratingPoint] /= numCalibrationSamples;
                
                cout << "Final value set: " << calibratingValues[calibratingPoint] << " from " << lastNumSamples << " samples" << endl;
                
                calibratingPoint++;
                
                lastNumSamples = numCalibrationSamples;
                numCalibrationSamples = 0;
                
                if(calibratingPoint == 12){
                    
                    //send this recorded value to the right gui elements
                    for(int i = 0; i < calibratingValues.size(); i++){
                        
                        float val = calibratingValues[i];
                        
                        switch (i) {
                                
                                //center points
                            case 0:
                                firstDepthSlider = val;
                                break;
                            case 1:
                                secondDepthSlider = val;
                                break;
                            case 2:
                                thirdDepthSlider = val;
                                break;
                            case 3:
                                fourthDepthSlider = val;
                                break;
                                
                                //left side points
                            case 4:
                                firstXLeftSlider = val;
                                break;
                            case 5:
                                secondXLeftSlider = val;
                                break;
                            case 6:
                                thirdXLeftSlider = val;
                                break;
                            case 7:
                                fourthXLeftSlider = val;
                                break;
                                
                                //right side points
                            case 8:
                                firstXRightSlider = val;
                                break;
                            case 9:
                                secondXRightSlider = val;
                                break;
                            case 10:
                                thirdXRightSlider = val;
                                break;
                            case 11:
                                fourthXRightSlider = val;
                                break;
                            default:
                                break;
                        }
                    
                        cout << "Storing Val: " << val << endl;
                        
                    }
                    
                    cout << "Values stored" << endl;
                    bCalibrating = false;
                    
                    
                    //reset for next time
                    calibratingPoint = 0;
                    numCalibrationSamples = 0;
                    
                }
                
                //stop recording process
                bRecording = false;
                
                
            }
            
        }
        
        
        msg += "\n";
        



        
        //draw a loading bar
        if(bRecording){
            
            float w = 200;
            float h = 20;
            
            ofSetColor(255, 0, 0);
            ofDrawBitmapString("RECORDING...", 0, rectH + 150);
            
            //draw moving rect
            float pct = (ofGetElapsedTimef() - samplingStartTime)/samplingDuration;
            ofFill();
            ofSetColor(255, 0, 0);
            ofDrawRectangle(0, rectH + 154, w * pct, h);
        
            //draw outline
            ofNoFill();
            ofSetColor(255);
            ofDrawRectangle(0, rectH + 154, w, h);

            ofFill();
            
        } else {
            
            //if we're not recording, display the current value
            
            if( touches.size() > 0 ){
                if(calibratingPoint < 4){
                    msg+= "Current DEPTH value for touch[0]: " + ofToString(touches[0].rawDepth);
                } else {
                    msg+= "Current X value for touch[0]: " + ofToString(touches[0].rawCamPos.x);
                }
            }
            
        }
        
        
        
        ofSetColor(100, 255, 100);
        ofDrawBitmapString(msg, 0, rectH + 20);
        
    
    }  else {
        
        //draw out touch data
        
        string touchData = "Num Touches: " + ofToString(touches.size()) + "\n";
        
        for(int i = 0; i < touches.size(); i++){
            
            touchData += "touch[" + ofToString(i) + "] - ID: " + ofToString(touches[i].id) + ", X: " + ofToString(touches[i].pos.x) + ", Y: " + ofToString(touches[i].pos.y) + "\n";
            
        }
        
        ofSetColor(255);
        ofDrawBitmapString(touchData, 0, rectH + 20);
        
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    
    
}



void WallCam::drawGui(int x, int y){
    
    gui.setPosition(x, y);
    gui.draw();
    
    
}

void WallCam::setupGui(){
    
    
    
    gui.setup(camName, filePath + camName + ".xml", 0, 0);

    gui.add(cameraLabel.setup("   CAMERA SETTINGS", ""));
    gui.add(nearClipSlider.setup("Cam Near Clip", 500, 0, 8000));
    gui.add(farClipSlider.setup("Cam Far Clip", 3000, 0, 8000));
    
    gui.add(cvLabel.setup("   IMAGE PROCESSING", ""));
    gui.add(preThresholdToggle.setup("Simple Threshold", true));
    gui.add(nearThreshSlider.setup("Near Threshold", 255, 0, 255));
    gui.add(farThreshSlider.setup("Far Threshold", 0, 0, 255));
    gui.add(blurAmountSlider.setup("Blur", 1, 0, 40));
    gui.add(numErosionsSlider.setup("Number of erosions", 0, 0, 10));
    gui.add(numDilationsSlider.setup("Number of dilations", 0, 0, 10));
    gui.add(rotateImageToggle.setup("Rotate Image", false));
    
    gui.add(roiLabel.setup("   ROI SETTINGS", ""));
    gui.add(roiDepthSlider.setup("Region Depth", 20, 0, camHeight));
    gui.add(wallCutoffSlider.setup("Wall Level Cutoff", 400, 0, camHeight));
    gui.add(wallHitDistSlider.setup("Dist for touch", 10, 0, 50));

    gui.add(bgDiffLabel.setup("   BG SUBTRACTION", ""));
    gui.add(useBgDiff.setup("Use BG Diff", false));
    gui.add(learningTimeSlider.setup("Frames to learn BG", 100, 0, 2000));
    gui.add(resetBGToggle.setup("Reset Background"));
    
    gui.add(contoursLabel.setup("   CONTOUR FINDING", ""));
//    gui.add(findContoursToggle.setup("Find Contours", true));
    gui.add(drawContoursToggle.setup("Draw Contours", true));
    gui.add(drawBlobInfoToggle.setup("Draw Blob info", true));
    gui.add(minBlobAreaSlider.setup("Min Blob Area", 0, 0, 1000));
    gui.add(maxBlobAreaSlider.setup("Max Blob Area", 1000, 0, 20000));
    gui.add(persistenceSlider.setup("Persistence", 15, 0, 100));
    gui.add(maxDistanceSlider.setup("Max Distance", 32, 0, 100));
    
    gui.add(controlPointsLabel.setup("   CONTROL POINTS", ""));
    gui.add(calibrateButton.setup("Calibration Routine"));
    gui.add(cancelButton.setup("Cancel Calibration"));
    gui.add(firstDepthSlider.setup("First Depth Pt", 0, 0, 255));
    gui.add(secondDepthSlider.setup("Second Depth Pt", 85, 0, 255));
    gui.add(thirdDepthSlider.setup("Third Depth Pt", 170, 0, 255));
    gui.add(fourthDepthSlider.setup("Fourth Depth Pt", 255, 0, 255));
    
    gui.add(firstXLeftSlider.setup("First Left X", 0, 0, 640));
    gui.add(firstXRightSlider.setup("First Right X", 255, 0, 640));

    gui.add(secondXLeftSlider.setup("Second Left X", 0, 0, 640));
    gui.add(secondXRightSlider.setup("Second Right X", 255, 0, 640));

    gui.add(thirdXLeftSlider.setup("Third Left X", 0, 0, 640));
    gui.add(thirdXRightSlider.setup("Third Right X", 255, 0, 640));

    gui.add(fourthXLeftSlider.setup("Fourth Left X", 0, 0, 640));
    gui.add(fourthXRightSlider.setup("Fourth Right X", 255, 0, 640));
    
    gui.add(touchSettingsLabel.setup("   TOUCH SETTINGS", ""));
    gui.add(touchSearchAreaSlider.setup("Touch Search Area", 5, 1, 30));
    gui.add(posSmoothingSlider.setup("Pos Smooth Amt", 10, 1, 30));
    gui.add(distSmoothingSlider.setup("Dist Smooth Amt", 6, 1, 30));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    cameraLabel.setBackgroundColor(ofColor(255));
    cvLabel.setBackgroundColor(ofColor(255));
    contoursLabel.setBackgroundColor(ofColor(255));
    roiLabel.setBackgroundColor(ofColor(255));
    bgDiffLabel.setBackgroundColor(ofColor(255));
    controlPointsLabel.setBackgroundColor(ofColor(255));
    touchSettingsLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels for some reason
    cvLabel.setDefaultTextColor(ofColor(0));
    
}

void WallCam::loadSettings(){
    
    gui.loadFromFile( filePath + camName + ".xml");
    
    
}

void WallCam::saveSettings(){
    
    gui.saveToFile(filePath + camName + ".xml");
    
}

void WallCam::threadedFunction(){
    
    while(isThreadRunning()){
        
//        camera.update();
        
        vector<int> settings_thread;
        ofShortPixels rawShortPix_thread; //short pix coming in
        
        if(rawShortPixIn.receive(rawShortPix_thread) && settingsIn.receive(settings_thread)){
        
            //make a thread safe copy of the cam dimensions
            int camWidth_thread = 640;
            int camHeight_thread = 480;
            
            //Local data inside thread
//            ofShortPixels rawShortPix_thread = camera.getRawDepth(); //short pix coming in
            ofPixels rawPix_thread;      //regular pix going out
            
            ofPixels threshPix_thread, blurredPix_thread;
            

            //get all the values from the settings vector

            int nearClip = settings_thread[0];
            int farClip = settings_thread[1];
            int nearThresh = settings_thread[2];
            int farThresh = settings_thread[3];
            
            int blurAmount = settings_thread[4];
            int numErosions = settings_thread[5];
            int numDilations = settings_thread[6];

            int roiDepth = settings_thread[7];
            int wallCutoff = settings_thread[8];
            
            int learningTime = settings_thread[9];
            bool useBgDiff = settings_thread[10];
            bool resetBG = settings_thread[11];
            
            int minBlobArea = settings_thread[12];
            int maxBlobArea = settings_thread[13];
            int persistence = settings_thread[14];
            int maxBlobDist = settings_thread[15];
            
            bool rotateImage = settings_thread[16];
            bool preThresh = settings_thread[17];
            
            
//            cout << "Frame Num: " << frameNum << ", ROI Depth" << roiDepth << ", Wall Cutoff" << wallCutoff << endl;
            
            rawPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            blurredPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            threshPix_thread.allocate(camWidth_thread, roiDepth, OF_IMAGE_GRAYSCALE);
            
            //check if the roi depth is the same as last time,
            //if not we'll need to reset the background or the thread will crash
            if(lastRoiDepth != roiDepth){
                needsAutoReset = true;
                lastRoiDepth = roiDepth;
            }
            
            
            //Convert from ofShortPixels to ofPixels and put into rawPix_thread
            for(int i = 0; i < rawShortPix_thread.size(); i++){
                
                if(rawShortPix_thread.getColor(i).r == 0){
                    rawPix_thread[i] = 0;
                } else {
                    rawPix_thread[i] = ofMap( rawShortPix_thread.getColor(i).r , nearClip, farClip, 255, 0, true);
                }
                
            }

            if(rotateImage){
                
                ofPixels rotatedRaw;
                rotatedRaw.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
                
                ofxCv::rotate(rawPix_thread, rotatedRaw, 180);
                
                rawPix_thread = rotatedRaw;
                
                
            }
            
            if( preThresh ){
                
                //PRE threshold the raw image: blackout pixels out of range
                //but leave the in-range pixels with original values
                for(int i = 0; i < rawPix_thread.size(); i++){
                    
                    if(rawPix_thread[i] < farThresh || rawPix_thread[i] > nearThresh){
                        rawPix_thread[i] = 0;
                    }
                    
                }
                
            }
            
            //blur the new raw image
            ofxCv::GaussianBlur(rawPix_thread, blurredPix_thread, blurAmount);

            
            
            //get the thresholded image (or use BG Diff): a horizontal slice of the raw image
            //bound at the bottom by the wallCutoff with a height of roiDepth
            int startingRow = wallCutoff - roiDepth;
            int startPixel = startingRow * camWidth_thread;
            int endPixel = startPixel + (roiDepth * camWidth_thread);
            
            //either use BG differencing or just straight up threshold it
            if(useBgDiff){
                
                //if we're going from not using the background to using it again
                //then reset the background (needsAutoReset == true).
                //Also reset it if requested by the GUI (resetBG == true)
                if(needsAutoReset || resetBG){
                    
                    background.reset();
                    background.setDifferenceMode(ofxCv::RunningBackground::BRIGHTER);
                    needsAutoReset = false;
                    
                }
                
                background.setLearningTime(learningTime);
                background.setThresholdValue(farThresh);
                
                //first we need to chop down blurredPix to the right size
                ofPixels blurredROIPix;
                blurredPix_thread.cropTo(blurredROIPix, 0, startingRow, camWidth_thread, roiDepth);
                
                background.update(blurredROIPix, threshPix_thread);
                
                
            } else {
                
                //threshold the blurred image if we want it
                for(int i = startPixel; i < endPixel; i++){
                    
                    if(blurredPix_thread[i] > farThresh && blurredPix_thread[i] < nearThresh){
                        
                        //put this pixel in the threshold but in the right place
                        //since threshPix is a narrow horizontal slice
                        threshPix_thread[i - startPixel] = 255;
                        
                    } else {
                        threshPix_thread[i - startPixel] = 0;
                    }
                    
                }

                
                //signal for an auto reset if we're decide to use BG diff again
                needsAutoReset = true;
                
            }
            
            
            


            
            //ERODE it
            for(int e = 0; e < numErosions; e++){
                erode(threshPix_thread);
            }
            
            //DILATE it
            for(int d = 0; d < numDilations; d++){
                dilate(threshPix_thread);
            }
            
            
            
            //-----Done with image altering-----
            //------Now do contour finding------
            
            
            //Define contour finder
            contours_thread.setMinArea(minBlobArea);
            contours_thread.setMaxArea(maxBlobArea);
            contours_thread.setThreshold(254);  //only detect white
            
            // wait before forgetting something
            contours_thread.getTracker().setPersistence(persistence);
            
            // an object can move up to ___ pixels per frame
            contours_thread.getTracker().setMaximumDistance(maxBlobDist);
            
            //find dem blobs
            contours_thread.findContours(threshPix_thread);
            
            contoursOut.send(std::move(contours_thread));
            
            rawPixOut.send(std::move(blurredPix_thread));
            threshPixOut.send(std::move(threshPix_thread));
            
            frameNum++;
            
        }
    }
    
}



