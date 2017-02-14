#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(200);
    ofSetVerticalSync(true);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------WebSocket Connection----------
    connectToServer = false;
    
    if(connectToServer){
//        client.connect("localhost", 8081);
        client.connect("35.165.1.38", 8081);  //"collectivewisdom.cc" also works
        client.addListener(this);
        bSendHeartbeat = true;
    }

    lastHeartbeatTime = 0;
    heartbeatInterval = 200;
    
    //----------SCHOLAR INFO Setup----------
    
    scholarData.setupData();
    
    
    //----------Scene Setup----------
    
    currentView = 1;
    
    //0 = Presentation (no debug)
    //1 = Centerbook
    //2 = Frame
    //3 = Bookcases
    //4 = Book Controller
    numViews = 5;
    
    
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
    contributionManager.loadContent();
    
    
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
    
    
    //BookController listens to new contributions
    ofAddListener(contributionManager.newContributionEvt, &bookController, &BookController::onNewContribution);
    
    //Bookcontroller listens to tag button for ribbons
    ofAddListener(centerBook.newButtonClickEvt, &bookController, &BookController::onButtonClickEvt);
    
    
    
    bgEdgeMask.load("assets/interface/bgMask.png");
    

    
    //----------Lighting/Material/Camera Setup----------
    camera.enableOrtho();
//    camera.disableMouseInput();
    camera.setTarget(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
//    camera.setFarClip(10000);
//    camera.setNearClip(-100);
    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));
    
    
    //----------Debug Tools----------
    bShowFrameRate = false;
    bShowMouseCoords = false;
    
    bIsFullscreen = false;
    


    //time of the last save/load event
    //start them out earlier so they dont draw on startup
    lastLoadTime = -10.0f;
    lastSaveTime = -10.0f;
    
    oscReceivePort = 12345;
    oscReceiver.setup(oscReceivePort);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    //Listen for OSC data to send touches to the right places
    if(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        /*
         ADDRESSES:
         /CenterCam/touch
         /RightCam/touch
         /LeftCam/touch
         
         */
        
        if(m.getAddress() == "/CenterCam/touch"){
            
            //package the touch and send it to the center cam
            Touch t;
            t.ID                =  m.getArgAsInt(0);
            t.pos               =  ofVec2f( m.getArgAsFloat(1), m.getArgAsFloat(2) );
            t.dist              =  m.getArgAsFloat(3);
            t.bIsTouching       =  m.getArgAsBool(4);
            
            centerBook.receiveTouch(t);
            
        }
        
        if(m.getAddress() == "/LeftCam/touch"){
            
            //package the touch and send it to the center cam
            Touch t;
            t.ID                =  m.getArgAsInt(0);
            t.pos               =  ofVec2f( m.getArgAsFloat(1), m.getArgAsFloat(2) );
            t.mappedPos         =  leftBookcase.mapToShelves( m.getArgAsFloat(1), m.getArgAsFloat(2) );
            t.dist              =  m.getArgAsFloat(3);
            t.bIsTouching       =  m.getArgAsBool(4);
            
            
            //Before sending the touch data to the
            bookController.receiveTouch(t);
            
        }

     
        if(m.getAddress() == "/RightCam/touch"){
            
            //package the touch and send it to the center cam
            Touch t;
            
            //add a big number so touches don't overlap with the other camera's
            //touches that are put into the same touches vector in bookController
            t.ID                =  m.getArgAsInt(0) + 10000;
            t.pos               =  ofVec2f( m.getArgAsFloat(1), m.getArgAsFloat(2) );
            t.mappedPos         =  rightBookcase.mapToShelves( m.getArgAsFloat(1), m.getArgAsFloat(2) );
            t.dist              =  m.getArgAsFloat(3);
            t.bIsTouching       =  m.getArgAsBool(4);
            
            
            //Before sending the touch data to the
            bookController.receiveTouch(t);
            
        }
        
        
        lastTouchTime = ofGetElapsedTimef();
        
    }
    
    idleTimer = ofGetElapsedTimef() - lastTouchTime;
    
    
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
    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));

    
    
    

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    
    //draw the scene
    ofBackground(0);
    
    camera.begin();
    
    //draw objects on top of wall paper regardless of depth
    ofDisableDepthTest();
    
    wallpaper.draw();

    frame.draw();
    
    leftBookcase.draw();
    rightBookcase.draw();

    
    centerBook.draw();
    
    

    bookController.draw();
    
    
    
    
    //camera inverts textures so draw the edge mask upside down
    ofSetColor(255);
    bgEdgeMask.draw(-1, ofGetHeight(), bgEdgeMask.getWidth(), -(bgEdgeMask.getHeight() + 2));
    
    
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
        
    }
    
    

    
    camera.end();
    
    
    
    if(bShowMouseCoords){
        ofSetColor(255);
        string s = ofToString(mouseX) + ", " + ofToString(mouseY);
        ofDrawBitmapStringHighlight(s, mouseX + 10, mouseY - 10);
    }
    
    
    
    
    if(currentView != 0){
        ofSetColor(255);
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(), 2), ofGetWidth()/2, 20);
    }
    
    
    ofVec2f textPos(ofGetWidth() - 175, 20);
    
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

    if(key == OF_KEY_LEFT){

        currentView--;
        if(currentView < 0){
            currentView = (numViews - 1);
        }
        
    } else if(key == OF_KEY_RIGHT){

        currentView++;
        if(currentView == numViews){
            currentView = 0;
        }
        
    } else if(key == OF_KEY_UP){

    } else if(key == OF_KEY_DOWN){

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
        camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
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
        
        lastSaveTime = ofGetElapsedTimef();
    }

    if(key == 'l'){
        
        //go through all the GUIs and save all their settings
        frame.loadSettings();
        leftBookcase.loadSettings();
        rightBookcase.loadSettings();
        wallpaper.loadSettings();
        bookController.loadSettings();
        
        lastLoadTime = ofGetElapsedTimef();
    }
    
    
    if(key == 'm'){
        
        //make a message with a random tag
        //get the tag list from the first book
        if(bookController.books.size() > 0){
//            int thisTag = floor(ofRandom(scholarData.tagList.size()));
            
            string name = "JaneDoe" + ofToString(ofGetElapsedTimef(), 2);
            string s = scholarData.tagList[messageTagNum];
            
            messageTagNum++;
            
            if(messageTagNum == 10) messageTagNum = 0;
            
            contributionManager.logNewContribution(name, s, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam vehicula purus ante, eu condimentum sapien ultrices nec. Aenean enim ipsum, condimentum id pellentesque et, sollicitudin eget ipsum. Cras sit amet auctor ex. Phasellus ac finibus metus.");
            
            
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
    
    //if we click in the frame, trigger the fact sheet
//    if(button == 0 && x > frame.frameCorners[0].x && x < frame.frameCorners[1].x && y < frame.frameCorners[3].y){
//        
//        if(frame.bShowFactSheet){
//            frame.hideFactSheet();
//        } else {
//            frame.showFactSheet();
//        }
//        
//    }
    
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
        string tag = parts[2];
        string message = parts[3];
        
        cout << "From: " << name << "\nTag: " << tag << "\nMessage: " << message << "\n" << endl;
        
        contributionManager.logNewContribution(name, tag, message);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"[Web socket] got broadcast "<<args.message<<endl;
}


