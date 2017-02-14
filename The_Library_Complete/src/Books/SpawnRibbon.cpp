//
//  SpawnRibbon.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/7/17.
//
//

#include "SpawnRibbon.hpp"


SpawnRibbon::SpawnRibbon(){
    
    
    
}

void SpawnRibbon::setup(ofColor tagCol){
    
    col = tagCol;
    
    ribbons.clear();
    
    //so different mesh instances draw differently
    timeOffset = ofRandom(100);
    
    
    //set up the rotation axis for each of the ribbons
    for(int i = 0; i < numRibbons; i++){
        
        ofVec3f thisAxis;
        thisAxis.set(ofRandomf(), ofRandomf(), ofRandomf());
        thisAxis.normalize();
        
        float thisAngle = ofRandom(360);
        
        Ribbon r;
        r.axis = thisAxis;
        r.angle = thisAngle;
        
        ribbons.push_back(r);
        
    }
    
    numSegments = 15;
    totalLength = 135;
    ribbonWidth = 20;
    
    //baseline orbiting radius of streamer
    baseRad = 100;
    
    //amplitude of radial variation (normalized)
    noiseAmp = 0.2;
    noiseSpeed = 0.1;
    nScale = 0.01;
    colorRange = 20;
    orbitSpeed = 2.0;
    
    ribbonTaper = 0.2;
    
    numRibbonsToDraw = 5;
    shrink = false;
    shrinkTime = 0.5;
    
    staggerAmt = 10;
    
    useNoise = true;
    
    bDrawWireframe = false;
}

void SpawnRibbon::reset(){
    shrink = false;
}

void SpawnRibbon::update(float timeLeft){
    
    //noise time
    float nTime = -ofGetElapsedTimef() * noiseSpeed;
    
    //time we'll use for orbitspeed
    double orbitPos = ofGetElapsedTimef() * orbitSpeed + timeOffset;

    

    
    for(int j = 0; j < numRibbonsToDraw; j++){
        
        ribbons[j].mesh.clear();
        ribbons[j].mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        //degrees between ribbons in the same plane
        float ribbonOffset = ofDegToRad(180) * j;
        
        for(int i = 0; i < numSegments; i++){
            
            float thisLength;
            float pos;
            float thisAmp = noiseAmp;
            float thisRad;
            
            //add 0.1 seconds of padding at the end so the ribbons
            //fully have time to go away before the book stops drawing them
            float timePadding = 0.5;
            if( timeLeft < shrinkTime + timePadding ){
                thisLength = ofClamp( ofMap(timeLeft, timePadding, shrinkTime + timePadding , 0, totalLength), 0, totalLength);
                thisAmp = ofClamp( ofMap(timeLeft, timePadding, shrinkTime + timePadding, 0, noiseAmp), 0, noiseAmp);
                thisRad = ofClamp( ofMap(timeLeft, 0, shrinkTime, 0, baseRad), 0, baseRad);
            } else {
                thisLength = totalLength;
                thisRad = baseRad;
            }
            
            thisRad += staggerAmt * j;
            
            float shift = 0;
            if(useNoise){
                shift = thisAmp * ofSignedNoise(nTime + nScale * i, 0);
            }

            
            //number of degrees between segments
            float segmentOffset = ofDegToRad(thisLength / (float)numSegments ) * i;
            
            pos = orbitPos - segmentOffset - ribbonOffset;
            
            //make every other ribbon go in the opposite direction
            if(j % 2 == 0) pos *= -1;
            
            //push one side of the verts ahead (for aesthetics)
            float dt = ofDegToRad(10);
            
            //and taper the tail
            float w = ofMap(i, 0, numSegments - 1, 1.0, ribbonTaper) * ribbonWidth;
            
            //set verts for this segment
            ofVec3f v1( thisRad * cos(pos + dt), thisRad * sin(pos + dt),   w/2 + shift);
            ofVec3f v2( thisRad * cos(pos     ), thisRad * sin(pos     ),  -w/2 + shift);
            
            ribbons[j].mesh.addVertex( v1 );
            ribbons[j].mesh.addVertex( v2 );
            
            //set color at this vert
            ofColor c;
            
            //the ribbon starts at a color before the base hue
            //and ends at a color past the base hue. then fmod to wrap around 255
            int thisHue = col.getHue() + colorRange/2 - ( i * colorRange/numSegments );
            
            if(thisHue > 255) thisHue % 255;
            
            if(thisHue < 0) thisHue += 255;
                
            
            c.setHsb( thisHue , 200, 255);
            ribbons[j].mesh.addColor(c);
            ribbons[j].mesh.addColor(c);
            
            
        }
        
    }    
}

void SpawnRibbon::draw(){
    
    ofSetColor(255);
    
    //draws all the ribbons around the origin
    //the book will translate it into position
    for(int i = 0; i < numRibbonsToDraw; i++){
        
        ofPushMatrix();
        ofRotate(ribbons[i].angle, ribbons[i].axis.x, ribbons[i].axis.y, ribbons[i].axis.z);
        
        if(bDrawWireframe){
            ribbons[i].mesh.drawWireframe();
        } else {
            ribbons[i].mesh.draw();
        }
        
        
        ofPopMatrix();
        
    }
    
}








