//
//  Wallpaper.cpp
//  Visuals_Test
//
//  Created by Adiel Fernandez on 10/23/16.
//
//

#include "Wallpaper.hpp"



Wallpaper::Wallpaper(){
    
    
    
}

void Wallpaper::setup(){
    
    
    //load all the images from file
    ofDirectory dir;
    
    dir.listDir("images/Wallpapers/");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector with correct # of images
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    
    for(int i = 0; i < (int)dir.size(); i++){
        images[i].load(dir.getPath(i));
    }

    currentImg = round(ofRandom( images.size() - 1 ));
    
    //scene variables
    sceneDim.set(1920, 1200);
    
    //-----Construct the mesh-----
    
    //Common factors of 1920x1200 for square grid sizing:
    //20, 24, 30, 40, 48, 60, 80, 120, 240
    gridSpacing = 80;
    
    for(int x = 0; x < sceneDim.x + 1 ; x += gridSpacing){
        for(int y = 0; y < sceneDim.y + 1; y += gridSpacing){
            
            /*
             Tiles vertex arrangement
             
             0 _____ 1
              |     |
              |     |
              |____ |
             3       2
             
             */
            
            vector<ofVec3f> verts;
            verts.resize(4);
            
            verts[0].set( x               , y               );
            verts[1].set( x + gridSpacing , y               );
            verts[2].set( x + gridSpacing , y + gridSpacing );
            verts[3].set( x               , y + gridSpacing );
            
            Tile t;
            t.setup(verts, gridSpacing);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            if(tiles.size() == 0) t.debugCout = true;
            
            tiles.push_back(t);
        
        }
    }
    

    bWave = false;
    waveSpeed = 1.0;   //in pixels per ms
    waveTileIndex = 0;

    
    
}

void Wallpaper::triggerWave(ofVec2f epicenter){
    
    waveEpicenter = epicenter;
    
    //go through all the tiles and calculate their distance from the epicenter
    for(int i = 0; i < tiles.size(); i++){
        
        //calculate distance to center to sort the vector
        tiles[i].distToEpicenter = ofDist(waveEpicenter.x, waveEpicenter.y, tiles[i].positionOnWall.x, tiles[i].positionOnWall.y);
        
        //calculate time from wave epicenter to triggering the specific tile
        tiles[i].timeUntilWave = tiles[i].distToEpicenter/waveSpeed;
        
    }
    
    //now that we have their distances stored within them we can sort based on that distance
    std::sort(tiles.begin(), tiles.end());
    
    bWave = true;
    waveStartTime = ofGetElapsedTimeMillis();
    waveTileIndex = 0;
    
}

void Wallpaper::applyEffectToAll(Tile::Effect e){
 
    //simple hack:
    //if the enumerated effect is EVEN it is
    //a fade IN, if it's ODD its fade OUT
    
    //FADE IN effects need new texture applied
    bool bNeedsNewTex = false;
    
    if(e % 2 == 0){
    
        bNeedsNewTex = true;
        
        //set the image we'll transition to
        nextImg = (currentImg == 1 ? 0 : 1);
//        newTexNum = round(ofRandom( images.size() - 1 ));
        
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

void Wallpaper::update(){
    
    
    //Handle wave effect
    if(bWave){
        
        //wave is based on actual speed: fps vs how far away from epicenter
        float timeBetweenFrames = 1000.0/ofGetFrameRate();



        //go through all the tiles from the point we left off and
        //trigger the ones that need to be triggered in between frames
        for(int i = waveTileIndex; i < tiles.size(); i++){
            
            
            if(ofGetElapsedTimeMillis() > waveStartTime + tiles[waveTileIndex].timeUntilWave + timeBetweenFrames){
                
//                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_AXIS, 0.0, waveEpicenter );
                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_HORIZ, 0.0, waveEpicenter );
                
                //CHANGE THIS TO REFLECT RANDOM TEXTURE PICKING
                nextImg = (currentImg == 1 ? 0 : 1);
                
                tiles[ waveTileIndex ].setNextTexture( nextImg );
                
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

void Wallpaper::draw(){
    

    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].draw();
    }
    
    
}





