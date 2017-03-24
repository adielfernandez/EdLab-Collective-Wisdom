#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(200);
    ofSetVerticalSync(true);
//        ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------WebSocket Connection----------
    connectToServer = true;
    
    if(connectToServer){
//        client.connect("localhost", 8081);
        client.connect("35.165.1.38", 8081);  //"collectivewisdom.cc" also works
        client.addListener(this);
        bSendHeartbeat = true;
    }
    
    lastHeartbeatTime = 0;
    heartbeatInterval = 250;
    
    //----------SCHOLAR INFO Setup----------
    
    scholarData.setupData();
    
    
    //----------Scene Setup----------
    
    currentView = 0;
    /*
        0 = Presentation (no debug)
        1 = Centerbook
        2 = Frame
        3 = Bookcases
        4 = Book Controller
        5 = Book positioning
     */
    
    numViews = 6;
    
    
    idleTimer = 0;
    lastTouchTime = 0;
    
    
    
    //set up objects without loading image data
    wallpaper.setup();
    frame.setup("frame");
    leftBookcase.setup("leftBookcase", true);
    rightBookcase.setup("rightBookcase", false);
    bookController.setBookCaseRefs(&leftBookcase, &rightBookcase);
    bookController.setScholarData(&scholarData);
    
    //Book models need to be loaded before other objects load their media
    //because of a bug in ofxAssimpModelLoader, otherwise loading will crash
    centerBook.loadModel();
    bookController.loadModels();
    
    //Now load their media
    wallpaper.loadMedia();
    frame.loadMedia();
    leftBookcase.loadMedia();
    rightBookcase.loadMedia();
    
    
    
    //pass scholar data down the chain to where it is needed
    frame.setScholarData(&scholarData);
    
    
    //Load contributions from file
    contributionManager.loadContent(&scholarData);
    
    
    //Finally, now that the book models have been loaded
    //and the bookcases have been setup, we can setup book textures
    //and place them on the shelves
    bookController.setup(&contributionManager.contributionList);
    centerBook.setup(&scholarData, &frame);
    
    //setup the scene controller and give it references to all the objects
    sceneController.setWallpaperRef( &wallpaper );
    sceneController.setBookcaseRefs( &leftBookcase , &rightBookcase );
    sceneController.setCenterBookRef( &centerBook );
    sceneController.setFrameRef( &frame );
    
    //get a reference to the idle timer
    sceneController.setIdleTimerRef( &idleTimer );
    sceneController.setup();
    
    
    //-----EVENT LISTENERS
    
    //Scene listens to centerbook redecorate button
    ofAddListener(centerBook.redecorateEvent, &sceneController, &SceneController::onRedecorateEvent);
    
    //Scene listens to ornaments on shelves
    for(int i = 0; i < bookController.ornaments.size(); i++){
        ofAddListener(bookController.ornaments[i].sceneChangeEvent, &sceneController,
                      &SceneController::onRedecorateEvent);
    }
    
    //Scene listens to book controller for book spawn events
    ofAddListener(bookController.bookSpawnEvent, &sceneController, &SceneController::onBookSpawnEvent);
    
    //BookController listens to new contributions
    ofAddListener(contributionManager.newContributionEvt, &bookController, &BookController::onNewContribution);
    
    //Bookcontroller listens to tag button for ribbons
    ofAddListener(centerBook.newButtonClickEvt, &bookController, &BookController::onButtonClickEvt);
    
    
    
    bgEdgeMask.load("assets/interface/bgMask.png");
    
    
    //----------CAMERAS----------
    centerCam.setup("CenterCam", "device/sensor0");
    leftCam.setup("LeftCam", "device/sensor1");
    rightCam.setup("RightCam", "device/sensor2");
    
    bViewCameras = false;
    numCams = 3;
    currentCam = 2;
    
    
    font.load("bookFonts/Century-gothic.ttf", 50);
    
    
    
    
    //----------Lighting/Material/Camera Setup----------
    camera.enableOrtho();
    camera.disableMouseInput();
    camera.setTarget(ofVec3f(1920/2, 1200/2, 0));
    //    camera.setFarClip(10000);
    //    camera.setNearClip(-100);
    camera.setPosition(1920/2, 1200/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));
    
    
    //----------Debug Tools----------
    bShowFrameRate = false;
    bShowMouseCoords = false;
    bShowCursor = false;
    
    bIsFullscreen = false;

    bShowCursor = false;
    ofHideCursor();

    
    //time of the last save/load event
    //start them out earlier so they dont draw on startup
    lastLoadTime = -10.0f;
    lastSaveTime = -10.0f;
    
    oscReceivePort = 12345;
    oscReceiver.setup(oscReceivePort);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //----------CAMERAS----------
    leftCam.update();
    rightCam.update();
    centerCam.update();
    
    
    
    //transfer touches from cameras to their respective locations
    //-----------------------------------------------------------
    //-------------------LEFT AND LEFT CAM TOUCHES---------------
    //-----------------------------------------------------------

    //before copying, go through the touches and map them using data from the bookcases
    for(int i = 0; i < leftCam.touches.size(); i++){
        leftCam.touches[i].mappedPos = leftBookcase.mapToShelves(leftCam.touches[i].pos.x, leftCam.touches[i].pos.y);
    }
    
    //before copying, go through the touches and map them using data from the bookcases
    for(int i = 0; i < rightCam.touches.size(); i++){
        rightCam.touches[i].mappedPos = rightBookcase.mapToShelves(rightCam.touches[i].pos.x, rightCam.touches[i].pos.y);
    }
    
    //clear out the destination vector
    bookController.touches.clear();
    
    //resize it to receive all the touches
    bookController.touches.resize( rightCam.touches.size() + leftCam.touches.size() );
    
    //copy them over
    std::copy(rightCam.touches.begin(), rightCam.touches.end(), bookController.touches.begin());

    //copy left touches immediately after the right touches
    std::copy(leftCam.touches.begin(), leftCam.touches.end(), bookController.touches.begin() + rightCam.touches.size());
    
    
    //------------------------------------------------
    //---------------CENTER CAM TOUCHES---------------
    //------------------------------------------------
    centerBook.touches.clear();
    centerBook.touches.resize( centerCam.touches.size() );
    
    //before copying, go through the touches and map them using data from the bookcases
    for(int i = 0; i < centerCam.touches.size(); i++){

        //map the normalized value to the desk width and height
        centerCam.touches[i].mappedPos = ofVec2f( centerCam.touches[i].pos.x * centerBook.deskWidth, centerCam.touches[i].pos.y * centerBook.deskHeight );
        
        //store the x flipped version of mappedPos
        centerCam.touches[i].mappedXFlip = ofVec2f( ( 1.0f - centerCam.touches[i].pos.x ) * centerBook.deskWidth, centerCam.touches[i].pos.y * centerBook.deskHeight );

    }
    
    //copy it over
    std::copy(centerCam.touches.begin(), centerCam.touches.end(), centerBook.touches.begin());
    
    //if there are touches we're not idle
    int totalTouches = centerCam.touches.size() + leftCam.touches.size() + rightCam.touches.size();
    
    if(totalTouches != 0) lastTouchTime = ofGetElapsedTimef();
    
    idleTimer = ofGetElapsedTimef() - lastTouchTime;
    
//    cout << "System Idle time: " << idleTimer << endl;
    
    
    sceneController.update();
    
    wallpaper.update();
    frame.update();
    leftBookcase.update();
    rightBookcase.update();
    
    bookController.update();
    
    centerBook.update();
    
    
    
    
    //Heartbeat to Server
    if(connectToServer && bSendHeartbeat){
        if(ofGetElapsedTimeMillis() - lastHeartbeatTime > heartbeatInterval){
            client.send("HB");
            lastHeartbeatTime = ofGetElapsedTimeMillis();
        }
    }
    
    
    //update camera positions so it snaps back after dragging
    camera.setPosition(1920/2, 1200/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));
    
    //set the centerbook camera to in the rare case that key/mouse presses
    //offset the easycam
    centerBook.resetCamera();
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //draw scene or draw cameras
    if( !bViewCameras ){
        
        //draw the scene
        ofBackground(0);
        
        camera.begin();
        
        //draw objects on top of wall paper regardless of depth
        ofDisableDepthTest();
        
        wallpaper.draw();
        
        frame.draw();
        
        sceneController.drawLeftBanner();
        sceneController.drawRightBanner();
        
        leftBookcase.draw();
        rightBookcase.draw();
        
        
        centerBook.draw();
        
        
        
        bookController.draw();
        
        
        
        
        //camera inverts textures so draw the edge mask upside down
        ofSetColor(255);
        bgEdgeMask.draw(-1, 1200, bgEdgeMask.getWidth(), -(bgEdgeMask.getHeight() + 2));
        
        
        sceneController.drawTitleBanner();
        
        
        if(currentView == 1){
            
            ofDisableDepthTest();
            
            //Center Book debug
            centerBook.drawDebug();
            centerBook.drawGui();
            
        } else if(currentView == 2){
            
            ofDisableDepthTest();
            
            //wallpaper and frame debug
            frame.drawDebug();
            frame.drawGui();
            
            wallpaper.drawGui();
            
        } else if(currentView == 3){
            
            
            ofDisableDepthTest();
            
            //Bookcases debug
            leftBookcase.drawDebug();
            leftBookcase.drawGui();
            
            rightBookcase.drawDebug();
            rightBookcase.drawGui();
            
        } else if(currentView == 4){
            
            
            ofDisableDepthTest();
            
            //Bookcases debug
            bookController.drawGui();
            
        } else if(currentView == 5){
            
            
            ofDisableDepthTest();
            
            //Bookcases debug
            bookController.drawGui2();
            bookController.drawPositioningDebug();
            
        }
        
        
        
        
        camera.end();
        
        
        
        
        
        
        if(bShowMouseCoords){
            ofSetColor(255);
            string s = ofToString(mouseX) + ", " + ofToString(mouseY);
            ofDrawBitmapStringHighlight(s, mouseX + 10, mouseY - 10);
        }
        
        
        
        
        if(currentView != 0){
            ofSetColor(255);
            ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(), 2), 1920/2, 20);
        }
        
        

        
        
    } else {
        
        //------------------------------------------------------------
        //--------------------------CAMERAS---------------------------
        //------------------------------------------------------------
        
        string s;
        s += "Framerate: " + ofToString(ofGetFrameRate()) + "\n";
        s += "Current Camera: " + ofToString(currentCam) + "\n";
        
        
        //position content on screen
        ofVec2f guiPos(15, 60);
        ofVec2f frame1Pos(260, 100);
        ofVec2f frame2Pos(frame1Pos.x + 640 + 10, 100);
        ofVec2f frame3Pos(frame1Pos.x, frame1Pos.y + 480 + 20);
        
        
        
        
        if(currentCam == 0 || currentCam == 1) {
            
            WallCam *thisCam = currentCam == 0 ? &leftCam : &rightCam;
//            WallCam *thisCam = &leftCam;
            
            
            if(thisCam -> isThreadRunning()){
                ofBackgroundGradient(ofColor(80), ofColor(0));
//                ofBackground(255);
            } else {
                ofBackground(200, 0, 0);
            }
            
            
            string title;
            if(currentCam == 0){
                title = "Left Wall Cam";
            } else {
                title = "Right Wall Cam";
            }
            
            ofSetColor(255);
            font.drawString(title, frame1Pos.x, font.stringHeight("A") + 10);
            
            
            s += "Camera Framerate: " + ofToString(thisCam -> camFrameRate) + "\n";
            
            
            thisCam -> drawGui(guiPos.x, guiPos.y);
            thisCam -> drawRaw(frame2Pos.x, frame2Pos.y);
            thisCam -> drawThresh(frame2Pos.x, frame2Pos.y + thisCam -> camHeight + 30, false);
            thisCam -> drawSceneProxy(frame1Pos.x + 20, frame1Pos.y);
            
            
            
            
        } else if(currentCam == 2) {
            
            
            if(centerCam.isThreadRunning()){
                ofBackgroundGradient(ofColor(80), ofColor(0));
            } else {
                ofBackground(200, 0, 0);
            }

            ofSetColor(255);
            font.drawString("Center Desk Cam", frame1Pos.x, font.stringHeight("A") + 10);
            
            centerCam.drawGui(guiPos.x, guiPos.y);
            centerCam.drawRaw(frame1Pos.x, frame1Pos.y);
            centerCam.drawThresh(frame2Pos.x, frame2Pos.y);
            centerCam.drawDeskProxy(frame2Pos.x, frame2Pos.y + 480 + 40);
            
            s += "Camera Framerate: " + ofToString(centerCam.camFrameRate) + "\n";
            
            
            
        }
        
        
        ofSetColor(255);
        ofDrawBitmapString(s, 10, 15);
        
        
        
    }
    
    
    ofVec2f textPos(1920 - 175, 20);
    
    if(ofGetElapsedTimef() - lastSaveTime < 2.0f){
        ofSetColor(180, 0, 0);
        ofDrawRectangle(textPos.x, textPos.y, 125, 25);
        
        ofSetColor(255);
        ofDrawBitmapString("Settings Saved", textPos.x + 5, textPos.y + 18);
        
        
    }
    
    if(ofGetElapsedTimef() - lastLoadTime < 2.0f){
        ofSetColor(0, 150, 0);
        ofDrawRectangle(textPos.x, textPos.y, 130, 25);
        
        ofSetColor(255);
        ofDrawBitmapString("Settings loaded", textPos.x + 5, textPos.y + 18);
    }
    
    
    
}




//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' '){
        bViewCameras = !bViewCameras;
    }
    
    if(key == OF_KEY_LEFT){
        
        if( !bViewCameras ){
            
            currentView--;
            if(currentView < 0){
                currentView = (numViews - 1);
            }

        } else {
        
            currentCam--;
            if(currentCam < 0){
                currentCam = (numCams - 1);
            }
            
        }
        
        
        
    } else if(key == OF_KEY_RIGHT){
        
        if( !bViewCameras ){
            
            currentView++;
            if(currentView == numViews){
                currentView = 0;
            }
            
        } else {
            
            currentCam++;
            if(currentCam > numCams - 1){
                currentCam = 0;
            }
            
        }
        
    } else if(key == OF_KEY_UP){
        
    } else if(key == OF_KEY_DOWN){
        sceneController.titleBanner.show();
    }
    
    if(key == OF_KEY_LEFT_SHIFT){
        bShowMouseCoords = true;
    }
    
    if(key == '0'){
        currentView = 0;
    } else if(key == '1'){
        currentView = 1;
    } else if(key == '2'){
        currentView = 2;
    } else if(key == '3'){
        currentView = 3;
    } else if(key == '4'){
        currentView = 4;
    } else if(key == '5'){
        currentView = 5;
    }
    
    if(key == 'f'){
        bIsFullscreen = !bIsFullscreen;
        
        ofSetFullscreen(bIsFullscreen);
    }
    
    
    if(key == 'h'){
        bSendHeartbeat = !bSendHeartbeat;
    }
    
    if(key == 'c'){
        
        //reset camera
        camera.setPosition(1920/2, 1200/2, -1000);
        camera.setOrientation(ofVec3f(180, 0, 0));
        
        centerBook.resetCamera();
        
    }
    
    
    if(key == 's'){
        
        //go through all the GUIs and save all their settings
        frame.saveSettings();
        leftBookcase.saveSettings();
        rightBookcase.saveSettings();
        wallpaper.saveSettings();
        bookController.saveSettings();
        centerBook.saveSettings();
        
        centerCam.saveSettings();
        leftCam.saveSettings();
        rightCam.saveSettings();
        
        lastSaveTime = ofGetElapsedTimef();
    }
    
    if(key == 'l'){
        
        //go through all the GUIs and save all their settings
        frame.loadSettings();
        leftBookcase.loadSettings();
        rightBookcase.loadSettings();
        wallpaper.loadSettings();
        bookController.loadSettings();
        
        centerCam.loadSettings();
        leftCam.loadSettings();
        rightCam.loadSettings();
        
        lastLoadTime = ofGetElapsedTimef();
    }
    
    if(key == 'c'){
        
        bShowCursor = !bShowCursor;
        
        if(bShowCursor){
            ofShowCursor();
        } else {
            ofHideCursor();
        }
        
    }
    
    
    if(key == 'm'){
        
        //make a message with a random tag
        //get the tag list from the first book
        if(bookController.books.size() > 0){
            //            int thisTag = floor(ofRandom(scholarData.tagList.size()));
            
            string name = "JaneDoe " + ofToString(ofGetElapsedTimef(), 2);
//            string s = scholarData.tagList[messageTagNum];
            
            
            if(messageTagNum == 10) messageTagNum = 0;
            
            contributionManager.logNewContribution(name, messageTagNum, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam vehicula purus ante, eu condimentum sapien ultrices nec. Aenean enim ipsum, condimentum id pellentesque et, sollicitudin eget ipsum. Cras sit amet auctor ex. Phasellus ac finibus metus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam vehicula purus ante, eu condimentum sapien ultrices nec. Aenean enim ipsum, condimentum id pellentesque et, sollicitudin eget ipsum. Cras sit amet auctor ex. Phasellus ac finibus metus.");
            
            messageTagNum++;
            
            //            cout << "New manual contribution with tag: " << s << endl;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key == OF_KEY_LEFT_SHIFT){
        bShowMouseCoords = false;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if( !bViewCameras ){
        
    
        if(button == 2){
        
            int randomTexNum = floor(ofRandom(10));
            
            
            if(x < leftBookcase.bookcaseCorners[1].x || x > rightBookcase.bookcaseCorners[0].x){
            
                //make bookcase wave start from center X of bookcase
                leftBookcase.triggerWave( randomTexNum, ofVec2f(leftBookcase.bookcaseCorners[0].x + (leftBookcase.bookcaseCorners[1].x - leftBookcase.bookcaseCorners[0].x)/2.0 , y));
                rightBookcase.triggerWave( randomTexNum, ofVec2f(rightBookcase.bookcaseCorners[0].x + (rightBookcase.bookcaseCorners[1].x - rightBookcase.bookcaseCorners[0].x)/2.0, y));
            
            } else if(y < frame.frameCorners[3].y){
                frame.triggerWave( randomTexNum, ofVec2f(x, y));
            } else {
                wallpaper.triggerWave( randomTexNum, ofVec2f(x, y));
            }
            
        }

    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"[Web socket] on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"[Web socket] on open"<<endl;
    client.send("init-of");
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"[Web socket] on close"<<endl;
    client.connect("localhost", 8081);
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    //    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    
    vector<string> parts = ofSplitString(args.message, "***");
    
    //    cout<<args.message<<endl;
    
    if(parts.size() > 1){
        string messageTag = parts[0];
        string name = parts[1];
        int tagNum = ofToInt( parts[2] );
        string message = parts[3];
        
        cout << "From: " << name << "\nTag: " << tagNum << "\nMessage: " << message << "\n" << endl;
        
        contributionManager.logNewContribution(name, tagNum, message);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"[Web socket] got broadcast "<<args.message<<endl;
}


