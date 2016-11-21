//
//  Book.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/15/16.
//
//

#include "Book.hpp"



Book::Book(){
    
}

void Book::loadModel(int type){

    bookType = type;
    
//    if(bookType == 0){
//        model.loadModel("books/bookShort.fbx", false);
//    } else if(bookType == 1){
//    } else {
//        model.loadModel("books/bookTall.fbx", false);
//    }
    
    model.loadModel("books/bookMedium.fbx", false);

}


void Book::setup(ofTexture *_tex){

    tex = _tex;
    
    
    //Start working with model
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    model.setPausedForAllAnimations(true);


    
    
    
    //figure out the minimum and maximum dimensions
    int totalVerts;
    maxX = -100000;
    minX =  100000;
    maxY = -100000;
    minY =  100000;
    maxZ = -100000;
    minZ =  100000;
    
    
    
    ofMesh m0 = model.getMesh(0);
    for(int i = 0; i < m0.getNumVertices(); i++){
        
        ofVec3f thisVert = m0.getVertex(i);
        
        if(thisVert.x > maxX) maxX = thisVert.x;
        if(thisVert.x < minX) minX = thisVert.x;
        
        if(thisVert.y > maxY) maxY = thisVert.y;
        if(thisVert.y < minY) minY = thisVert.y;
        
        if(thisVert.z > maxZ) maxZ = thisVert.z;
        if(thisVert.z < minZ) minZ = thisVert.z;
        
    }
    
    
    
    cout << "Min X: " << minX << endl;
    cout << "Max X: " << maxX << endl;
    cout << "Min Y: " << minY << endl;
    cout << "Max Y: " << maxY << endl;
    cout << "Min Z: " << minZ << endl;
    cout << "Max Z: " << maxZ << endl;
    
    /*
     //When model scale = 1.0,
     The pixel dimensions are below
     
     Min X: 0.0384481
     Max X: 13.3184
     Min Y: 0.0603343
     Max Y: 1.82742
     Min Z: -10.4535
     Max Z: 9.24601
     
     */
    
    //NOTE: Assimp scaling does not affect vertices themselves. In addition
    //scaling using .setScale() does not give reliable results. So, set scale
    //normalization to false and use your own scaling with push/pop matrices.
    model.setScaleNormalization(false);
    model.setScale(1.0, 1.0, 1.0); //unnecessary
    
    float desiredBookHeight = 140.0;
    modelScale = desiredBookHeight/ ( abs(minZ) + abs(maxZ) );
    
    cout << "Model Scale for [approx.] height of " << desiredBookHeight << " px: " << modelScale << endl;
    

    
    
    realMaxX = maxX * modelScale;
    realMinX = minX * modelScale;
    realMaxY = maxY * modelScale;
    realMinY = minY * modelScale;
    realMaxZ = maxZ * modelScale;
    realMinZ = minZ * modelScale;
    
    //print real pixel dimensions
    cout << "realMin X: " << realMinX << endl;
    cout << "realMax X: " << realMaxX << endl;
    cout << "realMin Y: " << realMinY << endl;
    cout << "realMax Y: " << realMaxY << endl;
    cout << "realMin Z: " << realMinZ << endl;
    cout << "realMax Z: " << realMaxZ << endl;
    
    //Move model so it sits with the bottom of the spine at the origin
    model.setPosition(0, 0, -minZ);
    
    
    //store the real dimensions
    //Model's orientation within its own coordinate system is different
    //so Y = thickness, etc.
    thickness = abs(realMaxY) + abs(realMinY);
    depth = abs(realMaxX) + abs(realMinX);
    height = abs(realMaxZ) + abs(realMinZ);
    cout << "Thickness: " << thickness << endl;
    cout << "Depth: " << depth << endl;
    cout << "Height: " << height << endl;
    
}

void Book::update(){
    
    
    model.update();
    
    
    
}

void Book::draw(){
    
    float mouseVal = ofMap(ofGetMouseX(), 0, ofGetWidth(), -600, 600);

    
    ofPushMatrix();{
        
        
        //translate the book to the position on the shelf
        ofTranslate(pos);// + ofVec2f(ofGetMouseX(), ofGetMouseY()));
        
        
        ofPushMatrix();{
            
            //now rotate and translate book so that it has the lower left corner of the spine
            //as its origin and is oriented properly: book stored in shelf
            ofRotateX(90);
            ofRotateZ(90);
//            ofTranslate(0, 0, mouseVal);
            
            
            model.disableTextures();
            
//            if(liveTexture.isAllocated()) liveTexture.getTexture().bind();
            
            
            ofScale(modelScale, modelScale, modelScale);
            
            tex -> bind();
            
            ofSetColor(255);
            model.drawFaces();
            
            tex -> unbind();
            
            ofDisableDepthTest();
            ofDrawAxis(200);
            
//            if(liveTexture.isAllocated()) liveTexture.getTexture().unbind();
            
            
            //in Y=minY plane
//            ofSetLineWidth(1);
//            ofSetColor(255, 0, 255);
//            ofDrawLine(realMinX, realMinY, -1000, realMinX, realMinY, 1000);
//            ofDrawLine(realMaxX, realMinY, -1000, realMaxX, realMinY, 1000);
//            ofDrawLine(-1000, realMinY, realMinZ, 1000, realMinY, realMinZ);
//            ofDrawLine(-1000, realMinY, realMaxZ, 1000, realMinY, realMaxZ);
//            
//            //in Y=maxY plane
//            ofPushMatrix();
//            ofTranslate(0, realMaxY, 0);
//            ofDrawLine(realMinX, realMinY, -1000, realMinX, realMinY, 1000);
//            ofDrawLine(realMaxX, realMinY, -1000, realMaxX, realMinY, 1000);
//            ofDrawLine(-1000, realMinY, realMinZ, 1000, realMinY, realMinZ);
//            ofDrawLine(-1000, realMinY, realMaxZ, 1000, realMinY, realMaxZ);
//            ofPopMatrix();
            
            
        }ofPopMatrix();
        
        ofSetColor(0, 255, 0);
        ofDrawCircle(0, 0, 1);
        

        
        
        ofSetColor(255);
        ofDrawBitmapStringHighlight(ofToString(mouseVal), 0, -30);
        
    }ofPopMatrix();
    
    
}



