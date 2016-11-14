#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetFrameRate(60);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    //----------WebSocket Connection----------
//    client.connect("localhost", 8081);
    //    client.connect("54.68.243.245", 8081);
    client.addListener(this);

    lastHeartbeatTime = 0;
    heartbeatInterval = 500;
    
    
    //----------Scene Setup----------
    
    currentView = 0;
    numViews = 5;
    
    wallpaper.setup();
    
    frame.setup("frame");

    leftBookcase.setup("leftBookcase", true);
    rightBookcase.setup("rightBookcase", false);
    //----------Camera Setup----------
    
    
    bShowGUIs = false;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));

    
    wallpaper.update();
    frame.update();
    leftBookcase.update();
    rightBookcase.update();
    
    
    //Heartbeat to Server
    if(ofGetElapsedTimeMillis() - lastHeartbeatTime > heartbeatInterval){
        
//        cout << "Heartbeat sent" << endl;
        client.send("HB");
        
        lastHeartbeatTime = ofGetElapsedTimeMillis();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofEnableDepthTest();
    
    if(currentView == 0 || currentView == 1){
        
        ofBackground(0);
        
        //push wallpaper back a tiny bit so it's always in the background
        ofPushMatrix();
        ofTranslate(0, 0, -1);

        wallpaper.draw();
        ofPopMatrix();
        
        
        //draw objects on top of wall paper regardless of depth
        ofDisableDepthTest();
        
        frame.draw();
        
        leftBookcase.draw();
        rightBookcase.draw();

        
        if(bShowGUIs){
            
            frame.drawGui();
            frame.drawDebug();
            
            wallpaper.drawGui();
            
            leftBookcase.drawGui();
            leftBookcase.drawDebug();

            rightBookcase.drawGui();
            rightBookcase.drawDebug();
            
            
        }
        
        
        ofEnableDepthTest();
        
    } else if(currentView == 1){
        
        
    } else if(currentView == 2){
        
        
    } else if(currentView == 3){
        
        
    } else if(currentView == 4){
        
        
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
    
    
    if(key == ' '){
        bShowGUIs = !bShowGUIs;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
        
        //trigger one of the objects to animate at random
        float r = ofRandom(1.0);
        
        if(r < 0.35){
            
            wallpaper.triggerWave(ofVec2f(x, y));
            
        } else if(r < 0.7){
            
            frame.triggerWave(ofVec2f(x, y));
            
        } else {
            
            leftBookcase.triggerWave(ofVec2f(ofGetWidth()/2, y));
            rightBookcase.triggerWave(ofVec2f(ofGetWidth()/2, y));
            
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


