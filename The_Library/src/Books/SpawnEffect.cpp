//
//  SpawnEffect.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/7/17.
//
//

#include "SpawnEffect.hpp"


SpawnEffect::SpawnEffect(){
    
    
    
}

void SpawnEffect::setup(ofColor tagCol){
    
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
    radAmp = 0.2;
    noiseSpeed = 0.1;
    nScale = 0.01;
    colorRange = 20;
    orbitSpeed = 2.0;
    
    ribbonTaper = 0.2;
    
    numRibbonsToDraw = 5;
    shrink = false;
    shrinkTime = 0.5;
    
    useNoise = true;
}

void SpawnEffect::reset(){
    shrink = false;
}

void SpawnEffect::update(float timeLeft){
    
    //noise time
    float nTime = -ofGetElapsedTimef() * noiseSpeed;
    
    //time we'll use for orbitspeed
    double orbitPos = ofGetElapsedTimef() * orbitSpeed + timeOffset;

    
    float thisLength;
    float thisAmp;
    float rad1, rad2;
    float pos;
    
    for(int j = 0; j < numRibbonsToDraw; j++){
        
        ribbons[j].mesh.clear();
        ribbons[j].mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        //degrees between ribbons in the same plane
        float ribbonOffset = ofDegToRad(180) * j;
        
        for(int i = 0; i < numSegments; i++){
            
            float amp = radAmp * baseRad;
            
            if( timeLeft < shrinkTime ){
                thisLength = ofMap(timeLeft, 0.0f, shrinkTime, 0, totalLength);
                thisAmp = ofMap(timeLeft, 0.0f, shrinkTime, 0, amp);
            } else {
                thisLength = totalLength;
                thisAmp = amp;
            }
            

            if(useNoise){
                rad1 = baseRad + thisAmp * ofSignedNoise(nTime + nScale * i, 0);
                rad2 = baseRad + thisAmp * ofSignedNoise(nTime + nScale * i, ribbonWidth);
            } else {
                rad1 = baseRad + thisAmp;
                rad2 = baseRad + thisAmp;
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
            ofVec3f v1( rad1 * cos(pos + dt), rad1 * sin(pos + dt),   w/2);
            ofVec3f v2( rad1 * cos(pos     ), rad1 * sin(pos     ),  -w/2);
            
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

void SpawnEffect::draw(){
    
    ofSetColor(255);
    
    //draws all the ribbons around the origin
    //the book will translate it into position
    for(int i = 0; i < numRibbonsToDraw; i++){
        
        ofPushMatrix();
        ofRotate(ribbons[i].angle, ribbons[i].axis.x, ribbons[i].axis.y, ribbons[i].axis.z);
        
        ribbons[i].mesh.draw();
        
        ofPopMatrix();
        
    }
    
}








