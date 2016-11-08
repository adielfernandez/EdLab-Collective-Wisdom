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
    gridSpacing = 48;
    
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
    TiledObject::setupTiledObject();
    

    bWave = false;
    waveSpeed = 1.0;   //in pixels per ms
    waveTileIndex = 0;

    guiName = "wallpaper";
    setupGui();
    
    waveSpeedSlider = 1.0;
    effectDurationSlider = 1.8;
    lastEffectDuration = 1.0;
    
    //not needed after first XML population
//    saveSettings();
    
    loadSettings();
    
}

void Wallpaper::update(){
    
    
    //update tiles and TiledObject with GUI settings
    waveSpeed = waveSpeedSlider;
    
    //check if the gui value is different
    if(effectDurationSlider != lastEffectDuration){
        
        //then change all the tiles
        for(int i = 0; i < tiles.size(); i++){
            tiles[i].effectDuration = effectDurationSlider;
        }
        //store the last value
        lastEffectDuration = effectDurationSlider;
    }
    
    
    TiledObject::update();
    

    
    
}

void Wallpaper::draw(){
    

    
    TiledObject::draw();
    
    
}



void Wallpaper::setupGui(){
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    gui.add(settingsLabel.setup("  GENERAL SETTINGS", ""));
    gui.add(waveSpeedSlider.setup("Wave Speed", 1.0f, 0.1f, 10.0f));
    gui.add(effectDurationSlider.setup("Effect Duration", 1.8f, 0.1f, 5.0f));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    settingsLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    settingsLabel.setDefaultTextColor(ofColor(0));
    
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

