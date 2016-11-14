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
    gridSpacing = 120;
    
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
            
            //The wallpaper doesn't require mapping
            //so texCoords will be the same as vertices
            vector<ofVec2f> texCoords;
            texCoords.resize(4);
            texCoords[0] = verts[0];
            texCoords[1] = verts[1];
            texCoords[2] = verts[2];
            texCoords[3] = verts[3];
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
        
        }
    }
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject(false);   //NOT a bookcase
    

    bWave = false;
    waveSpeed = 1.0;   //in pixels per ms
    waveTileIndex = 0;

    guiName = "wallpaper";
    setupGui();
    
    waveSpeedSlider = 1.0;
    effectDurationSlider = 1.8;
    
    //not needed after first XML population
//    saveSettings();
    
    loadSettings();
    
}

void Wallpaper::update(){
    
    
    TiledObject::updateCommonGui();

    
    
    TiledObject::update();
    

    
    
}

void Wallpaper::draw(){
    

    
    TiledObject::draw();
    
    
}



void Wallpaper::setupGui(){
    
    TiledObject::setupCommonGui();
    
    gui.setPosition(10, 10);
    
}

void Wallpaper::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    
}

void Wallpaper::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}

void Wallpaper::drawGui(){
    gui.draw();
}

void Wallpaper::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}

