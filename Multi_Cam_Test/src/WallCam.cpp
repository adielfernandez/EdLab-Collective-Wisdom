//
//  WallCam.cpp
//  Multi_Cam_Test
//
//  Created by Adiel Fernandez on 10/17/16.
//
//

#include "WallCam.h"

using namespace ofxCv;

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
    
    filePath = "settings/";
    camName = _camName;

    setupGui();
    loadSettings();
    
    camera.setup(deviceName);
    camera.enableRegistration(false);
    
    
    camera.initDepthStream();
    
    
    camera.enableDepthImage(false);
    
    
    frameNum = 0;
    
    camFrameRate = 0;
    lastFrameTime = 0;
    
    
    
    rawPix.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);

    startThread();
    
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
    
    ofxCv::ContourFinder c;
    if(contoursOut.tryReceive(c)){
        contours = c;
    }


    
    //---------------------------------------------------------------
    //--------------------MAP DEPTH TO WALL SPACE--------------------
    //---------------------------------------------------------------
    //clear previous touch data
    touches.clear();
    lowPoints.clear();
    
    if(contours.size() > 0){
        
        //go through all the contours
        for(int i = 0; i < contours.size(); i++){
            
            //get the position of this blob in the threshold
//            ofVec2f pixelPos = toOf(contours.getCenter(i));
//            int index = threshPix.getPixelIndex(pixelPos.x, pixelPos.y);
//            float depth = threshPix[index];

            vector<ofPoint> points = contours.getPolyline(i).getVertices();
            
            //go through all the vertices and find the point with the lowest y value
            //i.e. closest to wall
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
            
            float distFromWall = threshPix.getHeight() - points[index].y;
            
            //we now have the lowest point, we don't really need to store it except when
            //calibrating the space. Can comment out later.
            lowPoints.push_back( ofVec2f(points[index].x, points[index].y) );
            
            //get the pixel brightness but from the RAW image, so we have depth
            //remember we have to shift it down since threshPix is just a slice
            int lowPixelIndex = rawPix.getPixelIndex(points[index].x, points[index].y + thresholdHeight);
            float depth = rawPix[lowPixelIndex];
            
            //ignore the first blob
            if(i > 0)
            cout << "X: " << points[index].x << ", Y: " << points[index].y << ", Depth: " << depth <<endl;
            
            //we have a position from left to right and a depth.
            //Now we need to map them to wall space
            
            //these are the pixel brightness values at the close, middle and far end of the
            //region of interest, i.e. the top and bottom of bookshelf
            int nearDepthVal = 246;
            int middleDepthVal = 157;
            int farDepthVal = 68;
            
            //these will eventually be defined by the bookcase dimensions (top of 1st shelf, bottom of 3rd shelf)
            int nearScreenYval = 702;
            int farScreenYval = 90;
            
            //Y is easy, as pixel brightness maps linearly to distance
            float mappedY = ofMap(depth, nearDepthVal, farDepthVal, nearScreenYval, farScreenYval);
            
            //X is more difficult since travel perpendicular to the camera FOV changes with distance
            //Basically we will map the X differently from left to right using control points
            //at different depths. Control point: The x component of the left and right screen
            //pixels that correspond to a touch at each control point
            
            //These will be defined by the camera's x value when touching 6 points around
            //the far edges of the bookcase interaction area: top, middle and bottom
            //of the left and right sides
            
            //TO DO: Diagram this out and place in documentation
            
            int nearLeft = 37;
            int nearRight = 596;
            int middleLeft = 131;
            int middleRight = 499;
            int farLeft = 178;
            int farRight = 452;
            
            //map left and right distance from the centerline on each side
            //depending on distance
            float xLeft, xRight;
            
            //we're in the closer half to the camera
            if(depth > middleDepthVal){
                xLeft = ofMap(depth, nearDepthVal, farDepthVal, nearLeft, middleLeft);
                xRight = ofMap(depth, nearDepthVal, farDepthVal, nearRight, middleRight);
            } else {
                xLeft = ofMap(depth, nearDepthVal, farDepthVal, middleLeft, farLeft);
                xRight = ofMap(depth, nearDepthVal, farDepthVal, middleRight, farRight);
            }
            
            //now we know the left and right bounds at any depth: xLeft and xRight
            //so let's finally map them to the screen coordinates (eventually the bookcase width)

            float interactionAreaLeft = 273;
            float interactionAreaRight = 853;
            
            float mappedX = ofMap(points[index].x, xLeft, xRight, interactionAreaRight, interactionAreaLeft);
            
            Touch t;
            t.setTouchData(ofVec2f(mappedX, mappedY), distFromWall);
            
            touches.push_back(t);
            
        }

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
        settings.resize(13);

        settings[0] = nearClipSlider;
        settings[1] = farClipSlider;
        settings[2] = nearThreshSlider;
        settings[3] = farThreshSlider;
        settings[4] = blurAmountSlider;
        settings[5] = numErosionsSlider;
        settings[6] = numDilationsSlider;

        settings[7] = roiDepthSlider;
        settings[8] = wallCutoffSlider;
        
        settings[9] = minBlobAreaSlider;
        settings[10] = maxBlobAreaSlider;
        settings[11] = persistenceSlider;
        settings[12] = maxDistanceSlider;
        
        settingsIn.send(settings);
        
        lastFrameToThread = ofGetElapsedTimeMillis();
    }
    
    //set values according to GUI
    
    camera.setDepthClipping(nearClipSlider, farClipSlider);
    
    thresholdHeight = wallCutoffSlider - roiDepthSlider;
    
}

void WallCam::drawRaw(int x, int y){
    
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
    ofPopMatrix();
    
    
    ofPopStyle();
    
}

void WallCam::drawThresh(int x, int y, bool bDrawShifted){
 
    ofImage threshImg;
    threshImg.setFromPixels(threshPix);
    
    if(bDrawShifted){
        threshImg.draw(x, y + thresholdHeight);
    } else {
        threshImg.draw(x, y);
    }
    
    float w = threshPix.getWidth();
    float h = threshPix.getHeight();
    
    ofPushStyle();
    ofPushMatrix();

    if(bDrawShifted){
        ofTranslate(x, y + thresholdHeight);
    } else {
        ofTranslate(x, y);
    }
    
    //draw a light frame
    ofNoFill();
    ofSetLineWidth(1);
    ofSetColor(255, 100);
    ofDrawRectangle(0, 0, w, h);
    
    //draw crosshairs for calibration
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
    
                string msg = ofToString(label) + " : " + ofToString(c.x) + "," + ofToString(c.y);
                
                ofSetColor(0, 100, 255);
                ofDrawBitmapString(msg, c.x, c.y);
                
                //draw the low points
                ofSetColor(255, 0, 255);
                for(int j = 0; j < lowPoints.size(); j++){
                    ofDrawCircle(lowPoints[j], 5);
                    ofSetColor(0, 255, 0);
                    ofDrawBitmapString(ofToString(lowPoints[j]), c.x , c.y);
                }
                
            }
        
        }
        
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
    gui.add(nearThreshSlider.setup("Near Threshold", 255, 0, 255));
    gui.add(farThreshSlider.setup("Far Threshold", 0, 0, 255));
    gui.add(blurAmountSlider.setup("Blur", 1, 0, 40));
    gui.add(numErosionsSlider.setup("Number of erosions", 0, 0, 10));
    gui.add(numDilationsSlider.setup("Number of dilations", 0, 0, 10));
    
    gui.add(roiLabel.setup("   ROI SETTINGS", ""));
    gui.add(roiDepthSlider.setup("Region Depth", 20, 0, camHeight));
    gui.add(wallCutoffSlider.setup("Wall Level Cutoff", 400, 0, camHeight));

    gui.add(contoursLabel.setup("   CONTOUR FINDING", ""));
    gui.add(drawContoursToggle.setup("Draw Contours", true));
    gui.add(drawBlobInfoToggle.setup("Draw Blob info", true));
    gui.add(minBlobAreaSlider.setup("Min Blob Area", 0, 0, 1000));
    gui.add(maxBlobAreaSlider.setup("Max Blob Area", 1000, 0, 20000));
    gui.add(persistenceSlider.setup("Persistence", 15, 0, 100));
    gui.add(maxDistanceSlider.setup("Max Distance", 32, 0, 100));
    
    
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    cameraLabel.setBackgroundColor(ofColor(255));
    cvLabel.setBackgroundColor(ofColor(255));
    contoursLabel.setBackgroundColor(ofColor(255));
    roiLabel.setBackgroundColor(ofColor(255));
    
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
        
            //Local data inside thread
//            ofShortPixels rawShortPix_thread = camera.getRawDepth(); //short pix coming in
            ofPixels rawPix_thread;      //regular pix going out
            
            ofPixels threshPix_thread, blurredPix_thread;
            ofxCv::ContourFinder contours_thread;

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
            
            int minBlobArea = settings_thread[9];
            int maxBlobArea = settings_thread[10];
            int persistence = settings_thread[11];
            int maxBlobDist = settings_thread[12];
            
            
//            cout << "Frame Num: " << frameNum << ", ROI Depth" << roiDepth << ", Wall Cutoff" << wallCutoff << endl;
            
            rawPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            blurredPix_thread.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
            threshPix_thread.allocate(camWidth, roiDepth, OF_IMAGE_GRAYSCALE);
            
            
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

            
            //get the thresholded image: a horizontal slice of the raw image
            //bound at the bottom by the wallCutoff with a height of roiDepth
            int startingRow = wallCutoff - roiDepth;
            int startPixel = startingRow * camWidth;
            int endPixel = startPixel + (roiDepth * camWidth);

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



