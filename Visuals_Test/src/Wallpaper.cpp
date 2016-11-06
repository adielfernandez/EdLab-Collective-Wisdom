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
    //15, 16, 20, 24, 30, 40, 48, 60, 80
    gridSpacing = 40;
    
    for(int x = 0; x < sceneDim.x + 1 ; x += gridSpacing){
        for(int y = 0; y < sceneDim.y + 1; y += gridSpacing){
            
            /*
             Tiles are squares from grid, we'll use
             
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
    waveTileIndex = 0;
    waitToNextWaveTile = 0;
    lastWaveTileTime = 0;
    
}

void Wallpaper::triggerWave(ofVec2f epicenter){
    
    waveEpicenter = epicenter;
    
    //go through all the tiles and calculate their distance from the epicenter
    for(int i = 0; i < tiles.size(); i++){
        
        tiles[i].distSqToEpicenter = ofDistSquared(waveEpicenter.x, waveEpicenter.y, tiles[i].positionOnWall.x, tiles[i].positionOnWall.y);
        
    }
    
    //now that we have their distances stored within them we can sort based on that distance
    std::sort(tiles.begin(), tiles.end());
    
    bWave = true;
    lastWaveTileTime = ofGetElapsedTimeMillis();
    waitToNextWaveTile = 0;
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
        float waveSpeed = 15.0;  //in frames
        float timeBetweenFrames = 1000.0/ofGetFrameRate();
        
        //time until we trigger the next wave is calculated below using wave speed and tile distance
        if(ofGetElapsedTimeMillis() - lastWaveTileTime > waitToNextWaveTile){
            
            //go through and trigger all the triangles that need it
            //Use a do while loop to compensate from triangles that
            //need to be triggered between frames
            do {
                
                
                //Trigger the tile transition and set the next texture
//                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_AXIS, 0.0, waveEpicenter );
                tiles[ waveTileIndex ].triggerEffect( Tile::FLIP_TRANSITION_HORIZ, 0.0, waveEpicenter );
                
                //CHANGE THIS TO REFLECT RANDOM TEXTURE PICKING
                nextImg = (currentImg == 1 ? 0 : 1);
                
                tiles[ waveTileIndex ].setNextTexture( nextImg );
                

                
                //only calculate the time to next tri if we're not the last one
                if(waveTileIndex < tiles.size() - 1){
                    //time till next triggering the next triangle is
                    //distance between tris divided by wave speed
                    //Square root since the dist stored is distSq
                    float dist = sqrt( tiles[waveTileIndex + 1].distSqToEpicenter - tiles[waveTileIndex].distSqToEpicenter );
                    
                    waitToNextWaveTile = dist/waveSpeed;
                    lastWaveTileTime = ofGetElapsedTimeMillis();
                    
                    //get ready for the next one
                    waveTileIndex++;
                    
                } else {
                    
                    //if we ARE the last one, turn off the wave
                    bWave = false;
                    
                    //random non-zero value to exit while loop
                    waitToNextWaveTile = -1;
                    
                    currentImg = nextImg;
                }
                
            } while (waitToNextWaveTile == 0);
            
            
            
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





