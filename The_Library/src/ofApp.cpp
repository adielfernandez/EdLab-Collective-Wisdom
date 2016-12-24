#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(false);
    ofSetFrameRate(200);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------WebSocket Connection----------
    connectToServer = true;
    
    if(connectToServer){
//        client.connect("localhost", 8081);
        client.connect("35.165.1.38", 8081);
        client.addListener(this);
        bSendHeartbeat = true;
    }

    lastHeartbeatTime = 0;
    heartbeatInterval = 200;
    
    
    //----------Scene Setup----------
    
    currentView = 3;
    numViews = 4;
    
    
    //set up objects without loading image data
    wallpaper.setup();
    frame.setup("frame");
    leftBookcase.setup("leftBookcase", true);
    rightBookcase.setup("rightBookcase", false);
    bookController.setBookCaseRefs(&leftBookcase, &rightBookcase);

    
    //Book models need to be loaded before other objects load their media
    //because of a bug in ofxAssimpModelLoader, otherwise loading will crash
    centerBook.loadModel();
    bookController.loadModels();
    
    //Now load their media
    wallpaper.loadMedia();
    frame.loadMedia();
    leftBookcase.loadMedia();
    rightBookcase.loadMedia();
    
    
    //Load contributions from file
    contentManager.loadContent();
    
    
    //Finally, now that the book models have been loaded
    //and the bookcases have been setup, we can setup book textures
    //and place them on the shelves
    bookController.setup(&contentManager.contributionList);
    centerBook.setup();
    
    //add a listener in the Book controller for events
    //in the content manager when it gets new messages
    ofAddListener(contentManager.newContributionEvt, &bookController, &BookController::onNewContribution);
    
    
    bgEdgeMask.load("images/interface/bgMask.png");
    
    
    
    //----------Depth Camera Setup----------
    
    
    
    
    //----------Lighting/Material/Camera Setup----------
    camera.enableOrtho();
//    camera.disableMouseInput();
    camera.setTarget(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
//    camera.setFarClip(10000);
//    camera.setNearClip(-100);
    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));
    
    
    //----------Debug Tools----------
    bShowGUIs = false;
    bShowMouseCoords = false;
    
    
    lastChangeTime = 0;
    waitTime = 10000;

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    
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
    

    //Uncomment to debug camera position and orientation
    
//    float zPos = ofMap(mouseX, 0, ofGetWidth(), -2000, 2000);
//    cout << "Dist: " << zPos << endl;
//    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, zPos);

//    float x = ofMap(mouseX, 0, ofGetWidth(), -180, 180);
//    float y = ofMap(mouseY, 0, ofGetHeight(), -180, 180);
//    cout << "XY: " << x << ", " << y << endl;
//    camera.setOrientation(ofVec3f(x, y, 0));
    
    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
    camera.setOrientation(ofVec3f(180, 0, 0));

    
    
    
//    if(ofGetElapsedTimeMillis() - lastChangeTime > waitTime){
//        
//        float rand = ofRandom(3);
//        
//        float x = ofRandom(ofGetWidth());
//        float y = ofRandom(ofGetHeight());
//        
//        if(rand < 1.0){
//            
//            leftBookcase.triggerWave(ofVec2f(leftBookcase.bookcaseCorners[0].x + (leftBookcase.bookcaseCorners[1].x - leftBookcase.bookcaseCorners[0].x)/2.0 , y));
//            rightBookcase.triggerWave(ofVec2f(rightBookcase.bookcaseCorners[0].x + (rightBookcase.bookcaseCorners[1].x - rightBookcase.bookcaseCorners[0].x)/2.0, y));
//            
//        } else if(rand < 2.0){
//            frame.triggerWave(ofVec2f(x, y));
//
//        } else {
//            wallpaper.triggerWave(ofVec2f(x, y));
//        }
//        
//        waitTime = ofRandom(5000, 10000);
//        lastChangeTime = ofGetElapsedTimeMillis();
//    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    
    //draw the scene
    ofBackground(0);
    
    camera.begin();
    
    //draw objects on top of wall paper regardless of depth
    ofDisableDepthTest();
    
    //push wallpaper back a tiny bit so it's always in the background
    ofPushMatrix();
    ofTranslate(0, 0, -1);
    
    wallpaper.draw();
    ofPopMatrix();
    
    //draw object shadows before we enable depth testing again
    leftBookcase.drawShadow();
    rightBookcase.drawShadow();
    frame.drawShadow();
    
    //now enable depth testing again so books and
    //bookcases draw in their proper places
    ofEnableDepthTest();
    
    frame.draw();
    leftBookcase.draw();
    rightBookcase.draw();
    
    bookController.draw();
    
    centerBook.draw();
    
    
    bgEdgeMask.draw(-1, ofGetHeight(), bgEdgeMask.getWidth(), -(bgEdgeMask.getHeight() + 2));
    
    
    

    
    
    
    
    
    
    if(currentView == 1){
        
        //wallpaper and frame debug
        
        
        ofDisableDepthTest();
        
        frame.drawDebug();
        frame.drawGui();
        
        wallpaper.drawGui();
        
        
    } else if(currentView == 2){
    
        ofDisableDepthTest();
        
        //Bookcases debug
        leftBookcase.drawDebug();
        leftBookcase.drawGui();
        
        rightBookcase.drawDebug();
        rightBookcase.drawGui();
        
        
    } else if(currentView == 3){
        
        ofDisableDepthTest();
        
        //Center Book debug
        centerBook.drawDebug();
        centerBook.drawGui();
        
    }
    
    
    camera.end();
    
    
    
    if(bShowMouseCoords){
        ofSetColor(255);
        string s = ofToString(mouseX) + ", " + ofToString(mouseY);
        ofDrawBitmapStringHighlight(s, mouseX + 10, mouseY - 10);
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
    }
    
    
    if(key == 'h'){
        bSendHeartbeat = !bSendHeartbeat;
    }
    
    if(key == 'c'){
        
        //reset camera
        camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
        camera.setOrientation(ofVec3f(180, 0, 0));
        
    }
    
    if(key == 'm'){
        contentManager.logNewContribution("Name", "Tag" , "Message. Message. Message. Message.");
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
        
        if(x < leftBookcase.bookcaseCorners[1].x || x > rightBookcase.bookcaseCorners[0].x){
            
            //make bookcase wave start from center X of bookcase
            leftBookcase.triggerWave(ofVec2f(leftBookcase.bookcaseCorners[0].x + (leftBookcase.bookcaseCorners[1].x - leftBookcase.bookcaseCorners[0].x)/2.0 , y));
            rightBookcase.triggerWave(ofVec2f(rightBookcase.bookcaseCorners[0].x + (rightBookcase.bookcaseCorners[1].x - rightBookcase.bookcaseCorners[0].x)/2.0, y));
            
        } else if(y < frame.frameCorners[3].y){
            frame.triggerWave(ofVec2f(x, y));
        } else {
            wallpaper.triggerWave(ofVec2f(x, y));
        }
        
    }
    
    
    if(button == 0){
        bookController.checkMouseEvents(x, y);
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
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
    client.send("init-of");
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
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
        
        contentManager.logNewContribution(name, tag, message);
        
    }
    
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}


