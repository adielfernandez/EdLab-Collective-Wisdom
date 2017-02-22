//
//  DeskCam.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 12/15/16.
//
//

#include "DeskCam.hpp"



using namespace ofxCv;

DeskCam::DeskCam(){
    
}

DeskCam::~DeskCam(){
    
    closeAllChannels();
    
    waitForThread(true, 4000);
    
}

void DeskCam::closeAllChannels(){
    
    settingsIn.close();
    rawShortPixIn.close();
    rawPixOut.close();
    threshPixOut.close();
    contoursOut.close();
    
}


void DeskCam::emptyAllChannels(){
    
    //close thread channels
    settingsIn.empty();
    rawShortPixIn.empty();
    rawPixOut.empty();
    threshPixOut.empty();
    contoursOut.empty();
    
}

void DeskCam::setup(string _camName, const char* deviceName){
    
    filePath = "settings/";
    camName = _camName;
    
    
    
    setupGui();
    loadSettings();

    camera.setup(deviceName);
    camera.enableRegistration(false);
    camera.initDepthStream();
    camera.enableDepthImage(false);
    
    
    maskPoints.resize(4);
    maskPoints[0] = maskPt0;
    maskPoints[1] = maskPt1;
    maskPoints[2] = maskPt2;
    maskPoints[3] = maskPt3;
    
    maskPointMouseLock.assign(4, false);
    maskPointRad = 8;
    //give this instance the ability to use mouse events
    ofRegisterMouseEvents(this);
    
    rawPix.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    
    frameNum = 0;
    
    camFrameRate = 0;
    lastFrameTime = 0;
    
    needsAutoReset = true;
    isThreadCrashed = false;
    firstAfterCrash = true;
    
    firstRestart = true;
    
    lastRestartTime = 0;
    
    
    
    startThread();
    
}



void DeskCam::update(){
    
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
        
        bNewContours = true;
    }
    
    ofPixels fg;
    if(foregroundPixOut.tryReceive(fg)){
        foregroundPix = fg;
    }
    
    
    
    
    //---------------------------------------------------------------
    //---------------THREAD MANAGEMENT AND RESTARTING----------------
    //---------------------------------------------------------------
    
    if(ofGetElapsedTimeMillis() - lastFrameTime > 6000){
        isThreadCrashed = true;
        
        if(firstAfterCrash){
            cout << "Stopping Thread" << endl;
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
        
        cout << "Attempting to start thread..." << endl;
        startThread();
        
        lastRestartTime = ofGetElapsedTimeMillis();
        firstRestart = true;
        
    }
    
    //if it has been 2 seconds since last restart AND we're still crashed
    //then stop and prepare for the next restart
    //make sure to wait longer since we have waitForThread(4000)
    if(isThreadCrashed && ofGetElapsedTimeMillis() - lastRestartTime > 3000 && firstRestart){
        
        cout << "Stopping Thread" << endl;
        stopThread();
        
        emptyAllChannels();
        closeAllChannels();
        
        background.reset();
        
        firstRestart = false;
        
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
        settings.resize(24);
        
        settings[0] = nearClipSlider;
        settings[1] = farClipSlider;
        settings[2] = nearThreshSlider;
        settings[3] = farThreshSlider;
        settings[4] = blurAmountSlider;
        settings[5] = numErosionsSlider;
        settings[6] = numDilationsSlider;
        
        settings[7] = learningTimeSlider;
        settings[8] = useBgDiff;    //bool casted as int into vector
        settings[9] = resetBG;    //bool casted as int into vector
        
        settings[10] = minBlobAreaSlider;
        settings[11] = maxBlobAreaSlider;
        settings[12] = persistenceSlider;
        settings[13] = maxDistanceSlider;
        
        settings[14] = maskPt0 -> x;
        settings[15] = maskPt0 -> y;
        settings[16] = maskPt1 -> x;
        settings[17] = maskPt1 -> y;
        settings[18] = maskPt2 -> x;
        settings[19] = maskPt2 -> y;
        settings[20] = maskPt3 -> x;
        settings[21] = maskPt3 -> y;
        
        settings[22] = flipImageToggle;
        settings[23] = preThresholdToggle;
        
        settingsIn.send(settings);
        
        lastFrameToThread = ofGetElapsedTimeMillis();
    }

    
    
    //process new data from thread if there are contours to find
    
    if( bNewContours ){
        
        bNewContours = false;
        
        //Go through previous touch data and set all touches to not updated
        for(int i = 0; i < touches.size(); i++){
            touches[i].bUpdated = false;
            
            touches[i].distForTouch = touchThresholdSlider;
            touches[i].numPosSmoothingPts = posSmoothingSlider;
            touches[i].numDistSmoothingPts = distSmoothingSlider;
            
        }
        
        
        for(int i = 0; i < contours.size(); i++){
            
            
            //get all the points in the blob so we can find the lowest point
            //(highest on screen) i.e. the closest point to the wall
            vector<ofPoint> points = contours.getPolyline(i).getVertices();
            
            //occasionally, contours will return polylines with zero vertices
            //so check for verts to avoid seg-fault
            if(points.size() > 0){
                
                //go through all the vertices and find the point with the lowest y value
                int yVal = 100000;
                int index = 0;
                for(int j = 0; j < points.size(); j++){
                    //if we find a point lower than yVal, store it
                    if(points[j].y < yVal){
                        yVal = points[j].y;
                        
                        //also store this index;
                        index = j;
                    }
                }
                
                ofVec2f fingerTipPoint(points[index].x, points[index].y);
            
                //since the depth at the edge pixel might not always be the true depth (at boundary of contour),
                //look around the area just above the lowest point and pick the average depth val
                //of the blob as the depth reading, this should be the hand rather than table noise
                //get how far above the desk the fingertip is
                float heightAboveDesk = -1;
                int numSamples = 0;
                
                //search pixels above and to the sides, wider area gives more stable touch data
                int pixelArea = touchSearchAreaSlider;
                
                for(int x = fingerTipPoint.x - pixelArea; x < fingerTipPoint.x + pixelArea; x++){
                    for(int y = fingerTipPoint.y; y < fingerTipPoint.y + pixelArea; y++){
                        
                        //make sure the pixel is in bounds
                        if(x > 0 && x < foregroundPix.getWidth() && y > 0 && y < foregroundPix.getHeight()){
                            
                            int thisIndex = foregroundPix.getPixelIndex(x, y);
                            
                            
                            if(foregroundPix[thisIndex] > 0){
                                
                                heightAboveDesk += foregroundPix[thisIndex];
                                numSamples++;
                            }
                        }
                        
                    }
                }
                
                heightAboveDesk /= numSamples;
                
                //now we know the left and right bounds at any depth: xLeft and xRight
                //so let's finally map them to a normalized scale
                float mappedX = ofMap(fingerTipPoint.x, 0, camWidth, 0.0, 1.0);
                
                //map the Y so that points reach 1.0 when they hit the desk bound line
                float mappedY = ofMap(fingerTipPoint.y, 0, camHeight * deskBoundLineSlider, 0.0, 1.0);
                
                //scale the Y as a minor correction
                mappedY *= 1.0f - perceivedYShiftSlider;
                
                //Vertically shift the Y based on height to try to correct for the difference
                //between the camera's persepective vs the user's perspective
                //Gist: shift the touch vertically (-Y) the higher it goes
                //100 value is arbitrary height above table to map to
                // 1.0f - val is so that slider val of 0 means no shift
                mappedY *= ofMap(heightAboveDesk, 0, 100, 1.0, 1.0f - perceivedHeightShiftSlider);
                
                
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
                    touches[existingIndex].renewTouch(ofVec2f(mappedX, mappedY), heightAboveDesk);
                    
                    touches[existingIndex].rawCamPos = fingerTipPoint;
                    touches[existingIndex].rawDepth = heightAboveDesk;
                    
                } else {
                    
                    //add the new touch
                    DeskTouch t;
                    t.setNewTouch(contours.getLabel(i), ofVec2f(mappedX, mappedY), heightAboveDesk);

                    t.rawCamPos = fingerTipPoint;
                    t.rawDepth = heightAboveDesk;
                    
                    touches.push_back(t);
                    
                }

                
                
            }
        
        
        
        
        }
        
        
        //remove all the touches that haven't been updated recently
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
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    
    
    
    //set values according to GUI

        
        camera.setDepthClipping(nearClipSlider, farClipSlider);
        
        maskPoints.clear();
        maskPoints.resize(4);
        maskPoints[0] = maskPt0;
        maskPoints[1] = maskPt1;
        maskPoints[2] = maskPt2;
        maskPoints[3] = maskPt3;
        
        //check for mouse handling of the mapping points
        //and update the gui too
        for(int i = 0; i < maskPoints.size(); i++){
            if(maskPointMouseLock[i]){
                
                maskPoints[i] = adjustedMouse;
                
                switch (i) {
                    case 0:
                        maskPt0 = maskPoints[i];
                        break;
                    case 1:
                        maskPt1 = maskPoints[i];
                        break;
                    case 2:
                        maskPt2 = maskPoints[i];
                        break;
                    case 3:
                        maskPt3 = maskPoints[i];
                        break;
                    default:
                        break;
                }
                
                break;
            }
        }
        

        
    
    
}

void DeskCam::drawRaw(int x, int y){
    
    ofSetColor(255);
    ofDrawBitmapString("Raw Image (640 x 480)", x, y - 5);
    
    ofImage rawImg;
    rawImg.setFromPixels(rawPix);
    rawImg.draw(x, y);
    
    //draw frame
    float w = rawPix.getWidth();
    float h = rawPix.getHeight();
    
    
    //store the position of the adjusted mouse so we can see
    //if the mouse is clicking the mapping points
    adjustedMouse.set(ofGetMouseX() - x, ofGetMouseY() - y);
    
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 100);
    ofDrawRectangle(x, y, w, h);
    
    ofPushMatrix();
    ofTranslate(x, y);
    

    //draw where the mask is in green
    ofPath mask;
    mask.rectangle(0, 0, camWidth, camHeight);
    mask.close();
    mask.moveTo((ofPoint)maskPoints[0]);
    mask.lineTo((ofPoint)maskPoints[1]);
    mask.lineTo((ofPoint)maskPoints[2]);
    mask.lineTo((ofPoint)maskPoints[3]);
    mask.lineTo((ofPoint)maskPoints[0]);
    mask.setColor(ofColor(0, 255, 0, 40));
    
    mask.draw();
    
    //draw each of the Mask Points
    for(int i = 0; i < maskPoints.size(); i++){
        
        
        ofColor c = maskPointMouseLock[i] ? ofColor(255, 0, 0) : ofColor(0, 255, 0);
        ofSetColor(c);
        ofDrawCircle(maskPoints[i], maskPointRad);
        
        float xShift = (i == 0 || i == 3) ? -15 : 15;
        ofDrawBitmapStringHighlight(ofToString(i), maskPoints[i].x + xShift, maskPoints[i].y - 10);
        
    }
    
    //draw the desk bound line by interpolating between the mask points
    ofVec2f boundPt1, boundPt2;
    
    //         start         + (end           - start        ) * pct
    boundPt1 = maskPoints[0] + (maskPoints[3] - maskPoints[0]) * deskBoundLineSlider;
    boundPt2 = maskPoints[1] + (maskPoints[2] - maskPoints[1]) * deskBoundLineSlider;
    
    ofSetColor(255, 200, 0);
    ofDrawLine(boundPt1, boundPt2);

    //draw a line around the boundary
    ofSetColor(0, 255, 0);
    ofPolyline p;
    p.addVertex(maskPoints[0]);
    p.addVertex(maskPoints[1]);
    p.addVertex(maskPoints[2]);
    p.addVertex(maskPoints[3]);
    p.close();
    p.draw();
    
    
    
    //draw crosshairs for calibration
    ofSetColor(255, 100);
    ofDrawLine(w/2, 0, w/2, h);
    ofDrawLine(0, h/2, w, h/2);

    
    ofPopMatrix();
    ofPopStyle();
    
    

    
    
    //draw the raw depth value at that position
    if(ofGetMouseX() > x && ofGetMouseX() < x + w && ofGetMouseY() > y && ofGetMouseY() < y + h){
        ofSetColor(255);
        
        float depth = rawPix[rawPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y)];
        ofDrawBitmapStringHighlight("D: " + ofToString(depth), ofGetMouseX() + 30, ofGetMouseY());
        
    }
    
    
}


void DeskCam::drawThresh(int x, int y){
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
    
    if(drawForegroundToggle){
        
        ofImage fgImg;
        fgImg.setFromPixels(foregroundPix);
        fgImg.draw(0, 0);

        ofSetColor(255);
        ofDrawBitmapString("Foreground (640 x 480): after BG subtraction", x, y - 5);
        
    } else {
        
        ofImage threshImg;
        threshImg.setFromPixels(threshPix);
        threshImg.draw(0, 0);
        
        ofSetColor(255);
        ofDrawBitmapString("Processed (640 x 480): crop, threshold, contours", x, y - 5);
        
    }
    
    float w = threshPix.getWidth();
    float h = threshPix.getHeight();
    
    
    
    //draw a light frame
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, w, h);
    
    
    //draw crosshairs for calibration
    ofSetColor(255, 100);
    ofDrawLine(w/2, 0, w/2, h);
    ofDrawLine(0, h/2, w, h/2);
    
    //draw desk bound line
    ofSetColor(255, 200, 0);
    ofSetLineWidth(2);
    ofDrawLine(0, h * deskBoundLineSlider, w, h * deskBoundLineSlider);
    
    
    if(drawContoursToggle){
        
        //draw contours
        ofSetLineWidth(2.0);
        ofSetColor(255, 0, 0);
        contours.draw();
        
        
        
        if(drawBlobInfoToggle){
            
            string contourData = "";
            
            //go through and draw blob data too
            for(int i = 0; i < contours.size(); i++) {
                
                int label = contours.getLabel(i);
                ofPoint c = toOf(contours.getCenter(i));
                
                ofFill();
                ofSetColor(0, 255, 0);
                ofDrawCircle(c.x, c.y, 5, 5);
                ofDrawBitmapString(ofToString(label), c.x + 10, c.y);
                
                contourData += ofToString(label) + " : " + ofToString(c.x) + ", " + ofToString(c.y) + "\n";
                
                
            }
            
            ofSetColor(255);
            ofDrawBitmapString(contourData, 0, h + 20);

        }
        
        //draw the low points of the touches
        ofSetColor(0, 0, 255);
        for(int j = 0; j < touches.size(); j++){
            ofDrawCircle(touches[j].rawCamPos, 5);
            ofDrawBitmapStringHighlight(ofToString(touches[j].id) + " : " + ofToString(touches[j].rawCamPos) + "\n" + ofToString(touches[j].dist), touches[j].rawCamPos.x + 20, touches[j].rawCamPos.y);
            
            //draw the touch search area for the finger point
            ofNoFill();
            ofSetColor(0, 255, 255);
            ofDrawRectangle(touches[j].rawCamPos.x - touchSearchAreaSlider, touches[j].rawCamPos.y, touchSearchAreaSlider * 2, touchSearchAreaSlider);
            
        }
        
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    //draw the raw depth value at mouse position
    if(ofGetMouseX() > x && ofGetMouseX() < x + w && ofGetMouseY() > y && ofGetMouseY() < y + h){
        ofSetColor(255);
        
        float depth = rawPix[rawPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y)];
        ofDrawBitmapStringHighlight("Raw Depth: " + ofToString(depth), ofGetMouseX() + 10, ofGetMouseY());
        
        float fgVal = foregroundPix[foregroundPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y)];
        ofDrawBitmapStringHighlight("Diff. val: " + ofToString(fgVal), ofGetMouseX() + 10, ofGetMouseY() + 20);
        
    }
    
}

void DeskCam::drawDeskProxy(int x, int y){
    
    ofSetColor(255);
    ofDrawBitmapString("Desk Proxy with Normalized Coordinates", x, y - 5);
    
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x, y);
    
    //draw desk outline
    float w = 400;
    float h = 175;
    
    ofNoFill();
    
    ofSetColor(255, 200, 0);
    ofDrawRectangle(0, 0, w, h);
    
    //draw touch data
    for(int i = 0; i < touches.size(); i++){
        
        ofVec2f p = touches[i].pos;
        
        //convert from normalized
        p.x *= w;
        p.y *= h;
        
        float rad = ofMap(touches[i].dist, 0, 70, 7, 100, true);
        
        if(touches[i].bIsTouching){
            
            //draw a fat X when touching
            ofSetColor(0, 255, 0);
            
            ofSetLineWidth(6);
            
            //width of X
            int w = 20;
            ofDrawLine(p.x - w/2, p.y - w/2, -100, p.x + w/2, p.y + w/2, -100);
            ofDrawLine(p.x - w/2, p.y + w/2, -100, p.x + w/2, p.y - w/2, -100);
            
            //            ofDrawCircle(p.x, p.y, -100, rad * 2);
            
            
        } else {
            
            ofSetColor( 0, 128, 255 );
            
            ofSetLineWidth(4);
            ofNoFill();
            ofDrawCircle(p.x, p.y, -150, rad);
            ofFill();
        }
        
        
        ofDrawBitmapStringHighlight(ofToString(touches[i].pos) + "\n" + ofToString(touches[i].dist), p);
        
    }
    
    
    string touchData = "Num Touches: " + ofToString(touches.size()) + "\n";
    
    for(int i = 0; i < touches.size(); i++){
        
        touchData += "touch[" + ofToString(i) + "] - ID: " + ofToString(touches[i].id) + ", X: " + ofToString(touches[i].pos.x) + ", Y: " + ofToString(touches[i].pos.y) + ", Height: " + ofToString(touches[i].dist) + "\n";
        
    }
    
    ofSetColor(255);
    ofDrawBitmapString(touchData, 0, h + 20);
    

    ofPopStyle();
    ofPopMatrix();
    
    
    
}





void DeskCam::drawGui(int x, int y){
    
    gui.setPosition(x, y);
    gui.draw();
    
    
    
}

void DeskCam::setupGui(){
    
    
    
    gui.setup(camName, filePath + camName + ".xml", 0, 0);
    
    gui.add(cameraLabel.setup("   CAMERA SETTINGS", ""));
    gui.add(nearClipSlider.setup("Cam Near Clip", 500, 0, 3000));
    gui.add(farClipSlider.setup("Cam Far Clip", 1500, 0, 3000));
    
    gui.add(cvLabel.setup("   IMAGE PROCESSING", ""));
    gui.add(preThresholdToggle.setup("Simple Threshold", true));
    gui.add(nearThreshSlider.setup("Near Threshold", 255, 0, 255));
    gui.add(farThreshSlider.setup("Far Threshold", 0, 0, 255));
    gui.add(blurAmountSlider.setup("Blur", 1, 0, 40));
    gui.add(numErosionsSlider.setup("Number of erosions", 0, 0, 10));
    gui.add(numDilationsSlider.setup("Number of dilations", 0, 0, 10));
    gui.add(flipImageToggle.setup("Flip Image", false));
    
    ofVec2f min(0, 0);
    ofVec2f max(camWidth, camHeight);

    
    gui.add(maskingLabel.setup("   MASK SETTINGS", ""));
    //the current values will be overwritten quickly by the gui's loaded values
    gui.add(maskPt0.setup("Mask Pt 0", ofVec2f(0,0), min, max));
    gui.add(maskPt1.setup("Mask Pt 1", ofVec2f(0,0), min, max));
    gui.add(maskPt2.setup("Mask Pt 2", ofVec2f(0,0), min, max));
    gui.add(maskPt3.setup("Mask Pt 3", ofVec2f(0,0), min, max));
    
    gui.add(bgDiffLabel.setup("   BG SUBTRACTION", ""));
    gui.add(useBgDiff.setup("Use BG Diff", false));
    gui.add(learningTimeSlider.setup("Frames to learn BG", 100, 0, 500));
    gui.add(resetBG.setup("Reset Background"));
    
    gui.add(contoursLabel.setup("   CONTOUR FINDING", ""));
    gui.add(drawForegroundToggle.setup("Draw Foreground", false));
    gui.add(drawContoursToggle.setup("Draw Contours", true));
    gui.add(drawBlobInfoToggle.setup("Draw Blob info", true));
    gui.add(minBlobAreaSlider.setup("Min Blob Area", 0, 0, 5000));
    gui.add(maxBlobAreaSlider.setup("Max Blob Area", 1000, 0, 100000));
    gui.add(persistenceSlider.setup("Persistence", 15, 0, 100));
    gui.add(maxDistanceSlider.setup("Max Distance", 32, 0, 100));
    
    gui.add(touchSettingsLabel.setup("   TOUCH SETTINGS", ""));
    gui.add(deskBoundLineSlider.setup("Desk Bottom Bound", 1.0, 0.0, 1.0));
    gui.add(perceivedHeightShiftSlider.setup("Perceived Height Shift", 0.0, 0.0, 1.0));
    gui.add(perceivedYShiftSlider.setup("Perceived Y Shift", 0.0, 0.0, 1.0));
    gui.add(touchThresholdSlider.setup("Touch Threshold", 5, 1, 30));
    gui.add(touchSearchAreaSlider.setup("Touch Search Area", 5, 1, 30));
    gui.add(posSmoothingSlider.setup("Pos Smooth Amt", 10, 1, 30));
    gui.add(distSmoothingSlider.setup("Dist Smooth Amt", 6, 1, 30));

    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    cameraLabel.setBackgroundColor(ofColor(255));
    cvLabel.setBackgroundColor(ofColor(255));
    maskingLabel.setBackgroundColor(ofColor(255));
    contoursLabel.setBackgroundColor(ofColor(255));
    bgDiffLabel.setBackgroundColor(ofColor(255));
    touchSettingsLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels for some reason
    cvLabel.setDefaultTextColor(ofColor(0));
    
    
}

void DeskCam::loadSettings(){
    
    gui.loadFromFile( filePath + camName + ".xml");
    
    
}

void DeskCam::saveSettings(){
    
    gui.saveToFile(filePath + camName + ".xml");
    
}

void DeskCam::threadedFunction(){
    
    while(isThreadRunning()){
        
        //        camera.update();
        
        vector<int> settings_thread;
        ofShortPixels rawShortPix_thread; //short pix coming in
        
        if(rawShortPixIn.receive(rawShortPix_thread) && settingsIn.receive(settings_thread)){
            
            int camWidth_thread = 640;
            int camHeight_thread = 480;
            
            //Local data inside thread
            ofPixels rawPix_thread;      
            ofPixels threshPix_thread, blurredPix_thread;
            ofPixels mappedBlurredPix_thread;
            ofPixels foregroundPix_thread;
            
            
            //get all the values from the settings vector
            
            int nearClip = settings_thread[0];
            int farClip = settings_thread[1];
            int nearThresh = settings_thread[2];
            int farThresh = settings_thread[3];
            
            int blurAmount = settings_thread[4];
            int numErosions = settings_thread[5];
            int numDilations = settings_thread[6];
            
            int learningTime = settings_thread[7];
            bool useBgDiff = settings_thread[8];
            bool resetBG = settings_thread[9];
            
            int minBlobArea = settings_thread[10];
            int maxBlobArea = settings_thread[11];
            int persistence = settings_thread[12];
            int maxBlobDist = settings_thread[13];
            
            vector<ofVec2f> meshPts;
            meshPts.resize(4);
            meshPts[0] = ofVec2f(settings_thread[14], settings_thread[15]);
            meshPts[1] = ofVec2f(settings_thread[16], settings_thread[17]);
            meshPts[2] = ofVec2f(settings_thread[18], settings_thread[19]);
            meshPts[3] = ofVec2f(settings_thread[20], settings_thread[21]);
            
            bool flipImage = settings_thread[22];
            bool preThresh = settings_thread[23];
            
            rawPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            blurredPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            mappedBlurredPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            threshPix_thread.allocate(camWidth_thread, camHeight_thread, OF_IMAGE_GRAYSCALE);
            
            
            //Convert from ofShortPixels to ofPixels and put into rawPix_thread
            for(int i = 0; i < rawShortPix_thread.size(); i++){
                
                if(rawShortPix_thread.getColor(i).r == 0){
                    rawPix_thread[i] = 0;
                } else {
                    rawPix_thread[i] = ofMap( rawShortPix_thread.getColor(i).r , nearClip, farClip, 255, 0, true);
                }
                
            }
            
            if(flipImage){
                rawPix_thread.mirror(true, false);
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
            
            
            //take the rawPixels and get the bi-linear mapped subset from it
            getQuadSubPixels(blurredPix_thread, mappedBlurredPix_thread, meshPts);
            
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
                
                background.update(mappedBlurredPix_thread, threshPix_thread);
                
                //get the background pixels
                ofxCv::toOf( background.getForeground() , foregroundPix_thread );
                
                //send it out
                foregroundPixOut.send(std::move(foregroundPix_thread));
                
            } else {
                
                //threshold the blurred image if we want it
                for(int i = 0; i < camWidth * camHeight; i++){
                    
                    if(mappedBlurredPix_thread[i] > farThresh && mappedBlurredPix_thread[i] < nearThresh){
                        
                        //put this pixel in the threshold but in the right place
                        //since threshPix is a narrow horizontal slice
                        threshPix_thread[i] = 255;
                        
                    } else {
                        threshPix_thread[i] = 0;
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


//This method is an altered version of code borrowed from the
//OpenTSPS project created by Brett Renfer and the Lab @ Rockwell
//https://github.com/labatrockwell/openTSPS/blob/master/addons/ofxTSPS/libs/ofxTSPS/include/ofxTSPS/utils/Utils.h#L13

//Converts gets a subset of an ofPixels instance by doing a bi-linear mapping
//of the pixels between 4 control vertices
void DeskCam::getQuadSubPixels(ofPixels& inPix, ofPixels& outPix, vector <ofVec2f>& quad) {
    if ( quad.size() < 4 ){
        ofLog( OF_LOG_ERROR, "You must pass a vector of four points to this function");
        return;
    } // weird thing that could happen...
    
    int inW, inH, outW, outH;
    inW = inPix.getWidth();
    inH = inPix.getHeight();
    outW = outPix.getWidth();
    outH = outPix.getHeight();
    

    int xinput =0;
    int yinput = 0;
    int inIndex = 0;
    int outIndex = 0;
    
    float xlrp = 0.0;
    float ylrp = 0.0;
    
    ofPoint p1, p2, p3, p4;
    p1 = quad[0];
    p2 = quad[1];
    p3 = quad[2];
    p4 = quad[3];
    
    for(int x = 0; x < outW; x++) {
        for(int y = 0; y < outH; y++) {
            xlrp = x/(float)outW;
            ylrp = y/(float)outH;
            xinput = (p1.x * (1-xlrp) + p2.x * xlrp) * (1-ylrp) + (p4.x * (1-xlrp) + p3.x * xlrp) * ylrp;
            yinput = ((p1.y * (1-ylrp)) + (p4.y * ylrp)) * (1-xlrp) + (p2.y * (1-ylrp) + p3.y * ylrp) * xlrp;
            inIndex = ofClamp( xinput + (yinput * inW) , 0, inW * inH - 1);
            outIndex = ofClamp( x + y * outW, 0, outW * outH - 1);
            
            outPix[outIndex] = inPix[inIndex];
            
//            memcpy((outPix + outIndex), (inPix + inIndex), sizeof(unsigned char) );
            
        }
    }

}


void DeskCam::mousePressed(ofMouseEventArgs &args){
    
    //handle mouse interaction with Map points
    for(int i = 0; i < maskPointMouseLock.size(); i++){
        
        //need to adjust mouse position since we're
        //translating the raw camera view from the origin with pushmatrix
        float distSq = ofDistSquared(adjustedMouse.x, adjustedMouse.y, maskPoints[i].x, maskPoints[i].y);
        
        if(distSq < maskPointRad * maskPointRad){
            maskPointMouseLock[i] = true;
            
            //exit the for loop, prevents us from
            //grabbing multiple handles at once
            break;
            
        } else {
            maskPointMouseLock[i] = false;
        }
        
        
    }
    
}

void DeskCam::mouseReleased(ofMouseEventArgs &args){
    
    for(int i = 0; i < maskPointMouseLock.size(); i++){
        maskPointMouseLock[i] = false;
    }
    

    
}

void DeskCam::mouseMoved(ofMouseEventArgs &args){}
void DeskCam::mouseDragged(ofMouseEventArgs & args){}
void DeskCam::mouseScrolled(ofMouseEventArgs & args){}
void DeskCam::mouseEntered(ofMouseEventArgs & args){}
void DeskCam::mouseExited(ofMouseEventArgs & args){}

