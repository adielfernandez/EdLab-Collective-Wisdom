//
//  Tile.cpp
//  Visuals_Test
//
//  Created by Adiel Fernandez on 10/23/16.
//
//

#include "Tile.hpp"


Tile::Tile(){
    
    
    
}

void Tile::setup(vector<ofVec3f> verts, float _tileWidth){
    
    tileWidth = _tileWidth;
    tileCenter.set(tileWidth/2, tileWidth/2);
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    //Tile will center at origin for ease of manipulation
    //but store the top left corner so we know where tile needs to be placed on wallpaper
    ofVec3f topLeftCorner(verts[0]);

    positionOnWall = topLeftCorner + tileCenter;
    
    for(int i = 0; i < verts.size(); i++){
        
        //add the vert but subtract the origin and tile center
        //so the mesh draws around (0,0)
        mesh.addVertex(verts[i] - topLeftCorner - tileCenter);
        
        //but maintain verts relative to 1920x1200 image
        //for texcoords
        mesh.addTexCoord(verts[i]);
        
    }
    
    startTrans = 0.0;
    endTrans = 0.0;
    currentTrans = 0.0;

    bRotating = false;
    
    bFlipping = false;
    flipSpeed = 0.02;
    
    rotateStart.set(0, 0, 0);
    rotateEnd.set(0, 0, 0);
    
    effectStartTime = 0.0;
    effectStagger = 0.0;
    effectEndTime = 0.0;
    effectDuration = 1.0;
    
}

void Tile::setNewTexture(ofTexture *_tex){
    
    tex = _tex;
    
}

void Tile::triggerEffect(Tile::Effect e, float stagger ){
    
    
    
    switch (e) {
        case FLIP_IN_HORIZ:
            
            rotateStart.set(0, 180, 0);
            rotateEnd.set(0, 0, 0);
            currentRotation = rotateStart;
            
            bFlipping = true;
            
            startTrans = 0.0;
            endTrans = 255.0;
            
            break;
        case FLIP_OUT_HORIZ:
            
            rotateStart.set(0, 0, 0);
            rotateEnd.set(0, 180, 0);
            currentRotation = rotateStart;
            
            bFlipping = true;

            startTrans = 255.0;
            endTrans = 0.0;
            
            break;
            
        default:
            break;
    }
    
    
    effectStagger = stagger;
    effectStartTime = ofGetElapsedTimef() + effectStagger;
    effectEndTime = effectStartTime + effectDuration;
    
    
}

void Tile::update(){
    
    if(bFlipping){
        
        float now = ofGetElapsedTimef();
        
        //interpolate the transparency linearly
        //but end it a little early by shaving a bit off the end time
        float earlyFinish = 0.5 * effectDuration;
        currentTrans = ofMap(now, effectStartTime, effectEndTime - earlyFinish, startTrans, endTrans, true);
        
        //bounce ease the angle
        //flipping only happens in X and Y
        currentRotation.x = ofxeasing::map_clamp(now, effectStartTime, effectEndTime, rotateStart.x, rotateEnd.x, &ofxeasing::back::easeOut_s, 1.0);
        currentRotation.y = ofxeasing::map_clamp(now, effectStartTime, effectEndTime, rotateStart.y, rotateEnd.y, &ofxeasing::back::easeOut_s, 2.0);
        
        if(now > effectStartTime + effectDuration) bFlipping = false;
        
    }
    

    
    
}

void Tile::draw(){

    ofPushMatrix();
    
    //Mesh is positioned around it's origin so
    //translate to the center of the tile position
    ofTranslate(positionOnWall);
    
    ofRotateX(currentRotation.x);
    ofRotateY(currentRotation.y);
    ofRotateZ(currentRotation.z);
    
    tex -> bind();
    
    ofSetColor(255, currentTrans);
    mesh.draw();
    
    tex -> unbind();
    
    ofPopMatrix();
    
}





