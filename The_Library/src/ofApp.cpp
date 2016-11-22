#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(false);
    ofSetFrameRate(200);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------WebSocket Connection----------
    connectToServer = false;
    
    if(connectToServer){
//    client.connect("54.68.243.245", 8081);
        client.connect("localhost", 8081);
        client.addListener(this);
        bSendHeartbeat = true;
    }

    lastHeartbeatTime = 0;
    heartbeatInterval = 250;
    
    
    //----------Scene Setup----------
    
    currentView = 0;
    numViews = 5;
    
    
    //set up objects without loading image data
    wallpaper.setup();
    frame.setup("frame");
    leftBookcase.setup("leftBookcase", true);
    rightBookcase.setup("rightBookcase", false);

    
    //Book models need to be loaded before other objects load their media
    //because of a bug in ofxAssimpModelLoader
    bookController.loadModels();

    //Now load their media
    wallpaper.loadMedia();
    frame.loadMedia();
    leftBookcase.loadMedia();
    rightBookcase.loadMedia();
    
    //Finally, now that the book models have been loaded
    //and the bookcases have been setup, we can setup book textures
    //and place them in the right spots
    bookController.setup(&leftBookcase, &rightBookcase);
    
    
    bgEdgeMask.load("images/interface/bgMask.png");
    
    
    
    //----------Depth Camera Setup----------
    
    
    
    
    //----------Lighting/Material/Camera Setup----------
    camera.enableOrtho();
    camera.setTarget(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
//    camera.setFarClip(10000);
//    camera.setNearClip(-100);
    
    
    
    //----------Debug Tools----------
    bShowGUIs = false;
    bShowMouseCoords = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    
    wallpaper.update();
    frame.update();
    leftBookcase.update();
    rightBookcase.update();
    
    bookController.update();
    
    //Heartbeat to Server
    if(connectToServer && bSendHeartbeat){
        if(ofGetElapsedTimeMillis() - lastHeartbeatTime > heartbeatInterval){
            client.send("HB");
            lastHeartbeatTime = ofGetElapsedTimeMillis();
        }
    }
    
    
//    float zPos = ofMap(mouseX, 0, ofGetWidth(), -2000, 2000);
//    cout << "Dist: " << zPos << endl;
//    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, zPos);
    camera.setPosition(ofGetWidth()/2, ofGetHeight()/2, -1000);
    
//    float x = ofMap(mouseX, 0, ofGetWidth(), -180, 180);
//    float y = ofMap(mouseY, 0, ofGetHeight(), -180, 180);
//    cout << "XY: " << x << ", " << y << endl;
//    camera.setOrientation(ofVec3f(x, y, 0));
    camera.setOrientation(ofVec3f(180, 0, 0));
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    
    
    if(currentView == 0 || currentView == 1){
        
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

        bgEdgeMask.draw(-1, -1, bgEdgeMask.getWidth(), bgEdgeMask.getHeight() + 2);
        
        
        camera.end();
        
        
        if(bShowGUIs){
            
            frame.drawDebug();
            frame.drawGui();
            
            wallpaper.drawGui();
            
            leftBookcase.drawDebug();
            leftBookcase.drawGui();

            rightBookcase.drawDebug();
            rightBookcase.drawGui();
            
            
        }
        
        

        
    } else if(currentView == 1){
        
        
    } else if(currentView == 2){
        
        
    } else if(currentView == 3){
        
        
    } else if(currentView == 4){
        
        
    }
    
    
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
    
    if(key == ' '){
        bShowGUIs = !bShowGUIs;
    }
    
    if(key == 'h'){
        bSendHeartbeat = !bSendHeartbeat;
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
        string message = parts[2];
        
        cout << "From: " << name << "\nMessage: " << message << endl;
        
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}


