//
//  TagRibbon.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/11/17.
//
//

#include "TagRibbon.hpp"


TagRibbon::TagRibbon(){
    
}

void TagRibbon::setup(ofVec3f src, ofVec3f dst, int nPts, ofColor col, float stagger){
    
    srcPos = src;
    dstPos = dst;
    numPoints = nPts;
    tagCol = col;
    staggerDelay = stagger;
    
    //initialize all points to be at source
    for(int i = 0; i < numPoints; i++){
        points.push_back(src);
    }
    
    startTime = ofGetElapsedTimef();
    
    duration = 1.5;
    
    //What pct into the duration
    //do we want to hit the apex
    pctApex = 0.5;
    
    
    arcHeight = 100;
    
    dt = 0.1 ;
    
    
    bIsDead = false;
    bTouchDown = false;
    bDrawWireframe = false;
    
    bUseNoise = true;
    noiseAmp = 10;
    noiseSpeed = 0.01;
    noiseScale = 0.01;
    
    ribbonWidth = 40;
    ribbonTaper = 0.1;
    colorRange = 25;

}

void TagRibbon::update(){
    
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    
    //Move this here from setup it is calculated later after GUI settings are added
    float highestY = srcPos.y < dstPos.y ? srcPos.y : dstPos.y;
    apexPos.set( srcPos.x + (dstPos.x - srcPos.x) * pctApex , highestY - arcHeight);

    //all segments move through the same easing, but subsequent segments
    //are shifted in time so they are behind the head
    double now = ofGetElapsedTimef() - startTime - staggerDelay;
    
    for(int i = 0; i < points.size(); i++){
    
    
        
        double segmentNow = now - (dt * i);
        
        float x = ofxeasing::map_clamp(segmentNow, 0, duration, srcPos.x, dstPos.x, &ofxeasing::linear::easeOut);
        float y;
        
        //rising motion of the arc
        if( segmentNow < duration * pctApex ) {
            
            y = ofxeasing::map_clamp(segmentNow, 0, duration * pctApex, srcPos.y, apexPos.y, &ofxeasing::quad::easeOut);
            
            points[i].x = x;
            points[i].y = y;
            
        } else if( segmentNow < duration ){
            
            //dropping motion of the arc
            
            y = ofxeasing::map_clamp(segmentNow, duration * pctApex, duration, apexPos.y, dstPos.y, &ofxeasing::quad::easeIn);
            
            points[i].x = x;
            points[i].y = y;
            
        } else {
            bTouchDown = true;
        }
        
        
        //build the mesh around each of the points
        //start at the head to get an arrow kind of shape, then add
        //two verts at each segment in a triangle strip formation
        if(i == 0){
            
            mesh.addVertex(points[i]);
            mesh.addColor(tagCol);
        } else {
            
            //To add verts perpendicular to the ribbon's direction of motion
            //get the vector from the current point towards the last point
            //then rotate 90 deg left and right
            ofVec3f v = points[ i - 1 ] - points[ i ];
            
            v.normalize();
            v.rotate(90, ofVec3f(0, 0, 1));
            
            float w;
            
            if(segmentNow < duration + 0.2){
                w = ribbonWidth * ofMap(i, 1, points.size(), 1.0, ribbonTaper);
            } else {
                w = 0;
            }
            
            ofVec2f noiseVec(0);
            if(bUseNoise){
                float t = ofGetElapsedTimef() * noiseSpeed;
                float vN = noiseAmp * ofSignedNoise( t - noiseScale * i );
                
                noiseVec = v.getNormalized() * vN;
                
            }
            
            mesh.addVertex(points[ i ] + (v * w/2) + noiseVec );
            mesh.addVertex(points[ i ] - (v * w/2) + noiseVec );
            
            //set color at this vert
            ofColor c;
            
            //the ribbon starts at the tag color
            //and ends at a color past the base hue.
            int thisHue = tagCol.getHue() + ( i * colorRange/points.size() );
            
            //wrap hue value
            if(thisHue > 255) thisHue % 255;
            if(thisHue < 0) thisHue += 255;
            
            
            c.setHsb( thisHue , 200, 255);
            mesh.addColor(c);
            mesh.addColor(c);
            
            
        }
        
        
        
    }
    
    //total time for head (and tail) to reach dst
    float endTime = duration + dt * points.size();
    
    if( now > endTime ){
        bIsDead = true;
        
//        cout << "Marked dead" << endl;
    }
    

    
}

void TagRibbon::draw(){
    
    //only draw the last few points that haven't reached the destination
    
    for(int i = 0; i < points.size() - 1; i++){

        
        ofSetColor(255);
        if(bDrawWireframe){
            mesh.drawWireframe();
        } else {
            mesh.draw();
        }
        
        
//        ofSetColor(255, 0, 0);
//        ofDrawCircle(points[ i     ], 4);
//        ofDrawCircle(points[ i + 1 ], 4);
//        ofDrawLine(points[i], points[i + 1]);
        
    }
    
    
}
