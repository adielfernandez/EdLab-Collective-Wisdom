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
    gridSpacing = 80;
    
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
            t.setNewTexture( &images[currentImg].getTexture() );
            
            tiles.push_back(t);
        
        }
    }
    

    
    
}

void Wallpaper::applyEffectToAll(Tile::Effect e){
 
    //simple hack:
    //if the enumerated effect is EVEN it is
    //a fade IN, if it's ODD its fade OUT
    
    //FADE IN effects need new texture applied
    bool bNeedsNewTex = false;
    int newTexNum = 0;
    
    if(e % 2 == 0){
    
        bNeedsNewTex = true;
        newTexNum = round(ofRandom( images.size() - 1 ));
    
    }
    
    //shuffle the vector so the effect
    //doesn't happen such perfect order
    std::random_shuffle(tiles.begin(), tiles.end());
    
    //go through all the tiles and apply the effects staggered
    for(int i = 0; i < tiles.size(); i++){

        if(bNeedsNewTex){
            tiles[i].setNewTexture( &images[newTexNum].getTexture() );
        }
        
        float stagger = ofRandom(2.0);
        tiles[i].triggerEffect(e, stagger);
        
    }
    
}

void Wallpaper::update(){
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].update();
    }
    
    
}

void Wallpaper::draw(){
    

    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].draw();
    }
    
    
}





