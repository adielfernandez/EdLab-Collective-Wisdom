//
//  Globe.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/4/17.
//
//

#include "Globe.hpp"


Globe::Globe(){
    
    
}

void Globe::setup(ofVec3f shelfPos){
    
    pos = shelfPos;
    
    pos.z = -80;
    
    model.loadModel("assets/trinkets/globe/globe.3ds", false);
    
    ofImage t;
    t.load("assets/trinkets/globe/squareTex.png");
    tex = t.getTexture();
    
    fbo.allocate(tex.getWidth(), tex.getHeight());
    

    
    
    model.setScaleNormalization(false);
    model.setScale(1.0f, 1.0f, 1.0f);
    model.setPosition(0, 0, 0);
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    model.setPausedForAllAnimations(true);
    
    
    model.disableTextures();
}

void Globe::update(){

    
    
    fbo.begin();
    ofClear(0, 0, 0, 255);
    ofSetColor(255);
    
    ofPushMatrix();
    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.01, 10);
    ofScale(x, x);
    tex.draw(0, 0);
    ofPopMatrix();
    fbo.end();

    
    model.update();
    
    modelScale = 1.0f;

    
//    cout << x << endl;
    
}

void Globe::draw(){
    
    ofPushMatrix();
    ofTranslate(pos);
    ofScale(modelScale, modelScale, modelScale);
    

    ofEnableDepthTest();
    model.disableTextures();
    fbo.getTexture().bind();
    ofSetColor(255);
    model.drawFaces();

    fbo.getTexture().unbind();
    
    fbo.draw(0, 0);
    
    
    //draw local axes
    ofDrawAxis(30);
    
    ofPopMatrix();
    
}