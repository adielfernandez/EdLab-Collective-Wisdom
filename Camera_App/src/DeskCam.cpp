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
    
    firstStop = true;
    
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
        firstStop = true;
        
    }
    
    //if it has been 2 seconds since last restart AND we're still crashed
    //then stop and prepare for the next restart
    //make sure to wait longer since we have waitForThread(4000)
    if(isThreadCrashed && ofGetElapsedTimeMillis() - lastRestartTime > 3000 && firstStop){
        
        cout << "Stopping Thread" << endl;
        stopThread();
        
        emptyAllChannels();
        closeAllChannels();
        
        background.reset();
        
        firstStop = false;
        
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
        settings.resize(22);
        
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
        
        settingsIn.send(settings);
        
        lastFrameToThread = ofGetElapsedTimeMillis();
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

//bDrawShifted = false draws at X and Y
//true draws as if it were overlaying on top of the raw image
void DeskCam::drawThresh(int x, int y){
    
    ofImage threshImg;
    threshImg.setFromPixels(threshPix);

    threshImg.draw(x, y);
    
    
    float w = threshPix.getWidth();
    float h = threshPix.getHeight();
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y);
    
    
    //draw a light frame
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, w, h);
    
    
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
        
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    //draw the raw depth value at mouse position
    if(ofGetMouseX() > x && ofGetMouseX() < x + w && ofGetMouseY() > y && ofGetMouseY() < y + h){
        ofSetColor(255);
        
        float depth = rawPix[rawPix.getPixelIndex(ofGetMouseX() - x, ofGetMouseY() - y)];
        ofDrawBitmapStringHighlight("Raw Depth: " + ofToString(depth), ofGetMouseX() - 20, ofGetMouseY() - 20);
        
    }
    
}



void DeskCam::drawGui(int x, int y){
    
    gui.setPosition(x, y);
    gui.draw();
    
    
}

void DeskCam::setupGui(){
    
    
    
    gui.setup(camName, filePath + camName + ".xml", 0, 0);
    
    gui.add(cameraLabel.setup("   CAMERA SETTINGS", ""));
    gui.add(nearClipSlider.setup("Cam Near Clip", 500, 0, 8000));
    gui.add(farClipSlider.setup("Cam Far Clip", 3000, 0, 8000));
    
    gui.add(cvLabel.setup("   IMAGE PROCESSING", ""));
    gui.add(nearThreshSlider.setup("Near Threshold", 255, 0, 255));
    gui.add(farThreshSlider.setup("Far Threshold", 0, 0, 255));
    gui.add(blurAmountSlider.setup("Blur", 1, 0, 40));
    gui.add(numErosionsSlider.setup("Number of erosions", 0, 0, 10));
    gui.add(numDilationsSlider.setup("Number of dilations", 0, 0, 10));
    
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
    gui.add(learningTimeSlider.setup("Frames to learn BG", 100, 0, 2000));
    gui.add(resetBG.setup("Reset Background"));
    
    gui.add(contoursLabel.setup("   CONTOUR FINDING", ""));
    gui.add(drawContoursToggle.setup("Draw Contours", true));
    gui.add(drawBlobInfoToggle.setup("Draw Blob info", true));
    gui.add(minBlobAreaSlider.setup("Min Blob Area", 0, 0, 5000));
    gui.add(maxBlobAreaSlider.setup("Max Blob Area", 1000, 0, 20000));
    gui.add(persistenceSlider.setup("Persistence", 15, 0, 100));
    gui.add(maxDistanceSlider.setup("Max Distance", 32, 0, 100));
    
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    cameraLabel.setBackgroundColor(ofColor(255));
    cvLabel.setBackgroundColor(ofColor(255));
    maskingLabel.setBackgroundColor(ofColor(255));
    contoursLabel.setBackgroundColor(ofColor(255));
    bgDiffLabel.setBackgroundColor(ofColor(255));
    
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
            
            //Local data inside thread
            ofPixels rawPix_thread;      
            ofPixels threshPix_thread, blurredPix_thread;
            ofPixels mappedBlurredPix_thread;
            ofxCv::ContourFinder contours_thread;
            
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
            
            
            rawPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            mappedBlurredPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            blurredPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            threshPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            
            
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
            contours.setMinArea(minBlobArea);
            contours.setMaxArea(maxBlobArea);
            contours.setThreshold(254);  //only detect white
            
            // wait before forgetting something
            contours.getTracker().setPersistence(persistence);
            
            // an object can move up to ___ pixels per frame
            contours.getTracker().setMaximumDistance(maxBlobDist);
            
            //find dem blobs
            contours.findContours(threshPix_thread);
            
            contoursOut.send(std::move(contours));
            
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
            inIndex = xinput + (yinput * inW);
            outIndex = x + y * outW;
            
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

