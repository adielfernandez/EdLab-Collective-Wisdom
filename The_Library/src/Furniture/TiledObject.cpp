//
//  TiledObject.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/6/16.
//
//

#include "TiledObject.hpp"


TiledObject::TiledObject(){
    
    
    
}

void TiledObject::setupTiledObject(bool isBookcase){
    
    bIsBookcase = isBookcase;
    
    //if we're a book case, we'll go through the images one by one
    //instead of at random to keep both bookcases linked
    if(bIsBookcase){
        
        currentImg = 0;
        
    } else {
        
        currentImg = round(ofRandom( images.size() - 1 ));
    }
    

    
    bWave = false;
    waveSpeed = 2.0;   //in pixels per ms
    waveTileIndex = 0;
    
    
    
}

void TiledObject::setupCommonGui(){
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    gui.add(settingsLabel.setup("  GENERAL SETTINGS", ""));
    gui.add(waveSpeedSlider.setup("Wave Speed", 1.0f, 0.1f, 10.0f));
    gui.add(effectDurationSlider.setup("Effect Duration", 1.8f, 0.1f, 5.0f));
    gui.add(backEasingSlider.setup("Easing Bounce Amt", 1.0, 0.0, 4.0));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    settingsLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    settingsLabel.setDefaultTextColor(ofColor(0));

    
}

void TiledObject::updateCommonGui(){
    
    //Distribute GUI values to where they belong
    waveSpeed = waveSpeedSlider;
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].effectDuration = effectDurationSlider;
        tiles[i].backEasing = backEasingSlider;
    }
    
    
}

void TiledObject::triggerWave(ofVec2f epicenter){
    
    waveEpicenter = epicenter;
    
    //pick next image depending on type
    //bookcase increments by one, everything else picks a random number
    if(bIsBookcase){
        nextImg = currentImg + 1;
        if(nextImg == images.size()) nextImg = 0;
        
    } else {
        
        nextImg++;
        
        if(nextImg > images.size() - 1){
            nextImg = 0;
        }
//        nextImg = round(ofRandom( images.size() - 1 ));
//        
//        //make sure the next image selected isn't the same as the current one
//        if(nextImg == currentImg){
//            
//            nextImg += (int)ofRandom(images.size() - 1);
//            
//            //wrap around
//            if(nextImg > images.size() - 1){
//                nextImg -= images.size();
//            }
//            
//        }
    }
    
    //then store it for the next time
    currentImg = nextImg;
    
    
    //go through all the tiles and calculate their distance from the epicenter
    for(int i = 0; i < tiles.size(); i++){
        
        //calculate distance to center to sort the vector
        tiles[i].distToEpicenter = ofDist(waveEpicenter.x, waveEpicenter.y, tiles[i].positionOnWall.x, tiles[i].positionOnWall.y);
        
        //calculate time from wave epicenter to triggering the specific tile
        tiles[i].timeUntilWave = tiles[i].distToEpicenter/waveSpeed;
        
        //set next texture
        tiles[i].setNextTexture( nextImg );
        
    }
    
    //now that we have their distances stored within them we can sort based on that distance
    std::sort(tiles.begin(), tiles.end());
    
    bWave = true;
    waveStartTime = ofGetElapsedTimeMillis();
    waveTileIndex = 0;
    
}

void TiledObject::applyEffectToAll(Tile::Effect e){
    
    //simple hack:
    //if the enumerated effect is EVEN it is
    //a fade IN, if it's ODD its fade OUT
    
    //FADE IN effects need new texture applied
    bool bNeedsNewTex = false;
    
    if(e % 2 == 0){
        
        bNeedsNewTex = true;
        
        //set the image we'll transition to
//        nextImg = (currentImg == 1 ? 0 : 1);   //alternated between 0 and 1
        
        //pick next image depending on type
        //bookcase increments by one, everything else picks a random number
        nextImg = bIsBookcase ? currentImg + 1 : round(ofRandom( images.size() - 1 ));
        
        //then store it for the next time
        currentImg = nextImg;
        
        
    }
    
    //shuffle the vector so the effect
    //doesn't happen such perfect order
    std::random_shuffle(tiles.begin(), tiles.end());
    
    //go through all the tiles and apply the effects staggered
    for(int i = 0; i < tiles.size(); i++){
        
        if(bNeedsNewTex){
            tiles[i].setNextTexture( nextImg );
        }
        
        float stagger = ofRandom(1.5);
        tiles[i].triggerEffect(e, stagger);
        
    }
    
}

void TiledObject::update(){
    
    
    //Handle wave effect
    if(bWave){
        
        //wave is based on actual speed: fps vs how far away from epicenter
        float timeBetweenFrames = 1000.0/ofGetFrameRate();
        
        
        
        //go through all the tiles from the point we left off and
        //trigger the ones that need to be triggered in between frames
        for(int i = waveTileIndex; i < tiles.size(); i++){
            
            
            if(ofGetElapsedTimeMillis() > waveStartTime + tiles[waveTileIndex].timeUntilWave + timeBetweenFrames){
                
                //                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_AXIS, 0.0, waveEpicenter );
                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_RAND, 0.0, waveEpicenter );
                
                waveTileIndex++;
                
                
                if(waveTileIndex > tiles.size() - 1){
                    
                    //turn off wave
                    bWave = false;
                    
                    //set the next image as the current one
                    currentImg = nextImg;
                    
                    break;
                }
                
                
            } else {
                
                //no tiles triggered, exit the loop
                break;
                
            }
            
            
        }
        
        
        
    }
    
    
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].update();
    }
    
    
}

void TiledObject::draw(){
    
    
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].draw();
    }
    
    
}


ofVec2f TiledObject::getIntersectionPoint(ofVec2f line1Start, ofVec2f line1End, ofVec2f line2Start, ofVec2f line2End){
    
    ofVec2f intersectionPt;
    
    //find equation of line 1
    float m1, b1, m2, b2;
    
    //vertical lines break the straightforward approach so check for those
    //NOTE: if both lines are vertical this function breaks
    
    //first line is vert
    if(line1Start.x - line1End.x == 0.0){
        
        //we know the x right away since the line segment is vertical (all has the same x)
        intersectionPt.x = line1Start.x;
        
        //find slope 2
        m2 = (line2Start.y - line2End.y)/(line2Start.x - line2End.x);
        //find yIntercept 2
        b2 = line2Start.y - m2 * line2Start.x;
        
        intersectionPt.y = m2 * intersectionPt.x + b2;
        
    } else if(line2Start.x - line2End.x == 0.0){   //second line is vert
        
        //we know the x right away since the line segment is vertical (all has the same x)
        intersectionPt.x = line2Start.x;
        
        //find slope 1
        m1 = (line1Start.y - line1End.y)/(line1Start.x - line1End.x);
        //find yIntercept 1
        b2 = line2Start.y - m2 * line2Start.x;
        
        intersectionPt.y = m2 * intersectionPt.x + b2;
        
        
    } else {        //normal scenario
        
        //slope (y diff)/(x diff)
        m1 = (line1Start.y - line1End.y)/(line1Start.x - line1End.x);
        m2 = (line2Start.y - line2End.y)/(line2Start.x - line2End.x);
        
        //Y = mX + b   ==> b = Y - mX
        b1 = line1Start.y - m1 * line1Start.x;
        b2 = line2Start.y - m2 * line2Start.x;
        
        //X intersection point
        intersectionPt.x = (b2 - b1)/(m1 - m2);
        
        //sub X back into one of y = mx + b to get Y intersect
        intersectionPt.y = m1 * intersectionPt.x + b1;
        
    }
    
    return intersectionPt;
    
}



