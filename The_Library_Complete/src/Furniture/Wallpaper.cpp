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
    

    
    //scene variables
    sceneDim.set(1920, 1200);


    

    bWave = false;
    waveSpeed = 1.0;   //in pixels per ms
    waveTileIndex = 0;

    guiName = "wallpaper";
    
    tileResSliderName = "Tile Res _ ";
    
    bIsGuiActive = true;
    
    setupGui();
    
    
    waveSpeedSlider = 1.0;
    effectDurationSlider = 1.8;
    
    //not needed after first XML population
//    saveSettings();

    
    tileRes = getResFromSlider(tileResSlider);
    tileResSlider.setName(tileResSliderName + ofToString(tileRes));
    
    currentImg = 3; //round(ofRandom( images.size() - 1 ));
    
    mapMesh(tileRes);
    
    needsReMap = false;
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject(false);   //NOT a bookcase
    
    
    
}

//runs AFTER setup()
void Wallpaper::loadMedia(){
    
    //load all the images from file
    ofDirectory dir;
    dir.listDir("assets/wallpapers/");
    dir.sort();
    
    //load images with manual file names
    //loading with ofDirectory conflicts with ofxAssimp
    for(int i = 0; i < (int)dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        
        images.push_back(img);
        
    }
    
    
    
}

void Wallpaper::mapMesh(int res){
    
    tiles.clear();
    
    //-----Construct the mesh-----
    
    //Common factors of 1920x1200 for square grid sizing:
    //20, 24, 30, 40, 48, 60, 80, 120, 240
    gridSpacing = res;
    
    for(int x = 0; x < sceneDim.x; x += gridSpacing){
        for(int y = 0; y < sceneDim.y; y += gridSpacing){
            
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

    
    
}

int Wallpaper::getResFromSlider(int sliderVal){
    
    //get the value of the resolution slider and cast it to certain limited sizes
    //Sizes for square tiles (common factors of 1920x1200): 20, 24, 30, 40, 48, 60, 80, 120, 240
    int res = 0;
    
    switch (sliderVal) {
        case 1:
            res = 20;
            break;
        case 2:
            res = 24;
            break;
        case 3:
            res = 30;
            break;
        case 4:
            res = 40;
            break;
        case 5:
            res = 48;
            break;
        case 6:
            res = 60;
            break;
        case 7:
            res = 80;
            break;
        case 8:
            res = 120;
            break;
        case 9:
            res = 240;
            break;
            
        default:
            break;
    }
    
    return res;
    
}


void Wallpaper::update(){
    
    

    if(bIsGuiActive){
        tileRes = getResFromSlider(tileResSlider);
        tileResSlider.setName(tileResSliderName + ofToString(tileRes));
        
        //flag that a re-map is needed
        if(reMapMeshButton){
            needsReMap = true;
        }
        
        //Remap if needed, but wait for there to
        //be no active wave happening
        if(needsReMap && !bWave){
            mapMesh(tileRes);
            needsReMap = false;
        }
        
        TiledObject::updateCommonGui();
        
    }
    
    
    //will be set to true if we're drawing the gui
    bIsGuiActive = false;
    

    
    if( bIsAnimating ){
        TiledObject::update();
    }

    
    
}

void Wallpaper::draw(){
    
    //only draw all the tiles if we're animating
    //otherwise, draw the static image
    
    if(bIsAnimating){
        
        TiledObject::draw();
        
    } else {
        //flip the image in Y because of the camera
        ofSetColor(255);
        images[currentImg].draw(0, 1200, 1920, -1200);
    }
    
}



void Wallpaper::setupGui(){
    
    TiledObject::setupCommonGui();
    
    gui.add(mappingLabel.setup("  Mapping Points", ""));
    gui.add(tileResSlider.setup("Tile Resolution - ", 7, 1, 9));
    gui.add(reMapMeshButton.setup("Re-Map Mesh"));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    mappingLabel.setBackgroundColor(ofColor(255));
    
    gui.setPosition(10, 10);
    
    loadSettings();
    
    //now that we've loaded, move the gui to where we last had it
    gui.setPosition(guiPosSlider -> x, guiPosSlider -> y);
    gui.minimizeAll();

}

void Wallpaper::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    
}

void Wallpaper::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}

void Wallpaper::drawGui(){
    gui.draw();
    bIsGuiActive = true;
}

void Wallpaper::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
    bIsGuiActive = true;
}

