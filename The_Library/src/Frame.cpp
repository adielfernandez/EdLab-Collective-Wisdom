//
//  Frame.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/6/16.
//
//

#include "Frame.hpp"



Frame::Frame(){
    
    
    
}

void Frame::setup(string name){
    
    
    
    //load all the images from file
    ofDirectory dir;
    
    dir.listDir("images/frames/");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector with correct # of images
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    
    for(int i = 0; i < (int)dir.size(); i++){
        images[i].load(dir.getPath(i));
    }
    
    currentImg = round(ofRandom( images.size() - 1 ));

    //ALL THE BUSINESS HAPPENS IN HERE
    //This will need to be called whenever the mesh is altered by GUI/mouse
    prepareMesh();
    
    //Set GUI values to these (Only needed for first population of GUI XML)
//        frameCorner0 = frameCorners[0];
//        frameCorner1 = frameCorners[1];
//        frameCorner2 = frameCorners[2];
//        frameCorner3 = frameCorners[3];
//        controlPtPct0 = controlPointPcts[0];
//        controlPtPct1 = controlPointPcts[1];
//        controlPtPct2 = controlPointPcts[2];
//        controlPtPct3 = controlPointPcts[3];
//        controlPtPct4 = controlPointPcts[4];
//        controlPtPct5 = controlPointPcts[5];
//        controlPtPct6 = controlPointPcts[6];
//        controlPtPct7 = controlPointPcts[7];
    
    guiName = name;
    
    setupGui();
    
    loadSettings();
    
    
    
    mapMesh();
    
    
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject();

    
}


void Frame::update(){
    
    //Distribute GUI values to where they belong
    waveSpeed = waveSpeedSlider;
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].effectDuration = effectDurationSlider;
    }
    
    //set variables from GUI settings
    frameCorners[0] = frameCorner0;
    frameCorners[1] = frameCorner1;
    frameCorners[2] = frameCorner2;
    frameCorners[3] = frameCorner3;
    
    controlPointPcts[0] = controlPtPct0;
    controlPointPcts[1] = controlPtPct1;
    controlPointPcts[2] = controlPtPct2;
    controlPointPcts[3] = controlPtPct3;
    controlPointPcts[4] = controlPtPct4;
    controlPointPcts[5] = controlPtPct5;
    controlPointPcts[6] = controlPtPct6;
    controlPointPcts[7] = controlPtPct7;
    
    //ofVec2f controlPoints are for convenience and calculated
    //based on controlPointsPcts vector. Recalculate them in case settings have changed
    controlPoints.resize(8);
    controlPoints[0] = frameCorners[0].getInterpolated(frameCorners[1], controlPointPcts[0]);
    controlPoints[1] = frameCorners[0].getInterpolated(frameCorners[1], controlPointPcts[1]);
    controlPoints[2] = frameCorners[1].getInterpolated(frameCorners[2], controlPointPcts[2]);
    controlPoints[3] = frameCorners[1].getInterpolated(frameCorners[2], controlPointPcts[3]);
    controlPoints[4] = frameCorners[3].getInterpolated(frameCorners[2], controlPointPcts[4]);
    controlPoints[5] = frameCorners[3].getInterpolated(frameCorners[2], controlPointPcts[5]);
    controlPoints[6] = frameCorners[0].getInterpolated(frameCorners[3], controlPointPcts[6]);
    controlPoints[7] = frameCorners[0].getInterpolated(frameCorners[3], controlPointPcts[7]);
    
    
    if(reMapMeshButton){
        tiles.clear();
        mapMesh();
    }
    
    
    TiledObject::update();
    
}

void Frame::draw(){
    
    TiledObject::draw();
    

    
}

void Frame::drawDebug(){
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    
    
    //draw corners
    for(int i = 0; i < frameCorners.size(); i++){

        ofSetColor(255, 0, 0);
        ofDrawCircle(frameCorners[i], 10);
        ofDrawBitmapStringHighlight(ofToString(i), frameCorners[i].x + 10, frameCorners[i].y - 10);

    }

    ofPolyline corners;
    corners.addVertex(frameCorners[0]);
    corners.addVertex(frameCorners[1]);
    corners.addVertex(frameCorners[2]);
    corners.addVertex(frameCorners[3]);
    corners.close();
    corners.draw();
    
    //draw control points
    for(int i = 0; i < controlPoints.size(); i++){

        ofSetColor(0, 255, 0);
        ofDrawCircle(controlPoints[i], 10);
        ofDrawBitmapStringHighlight(ofToString(i), controlPoints[i].x + 10, controlPoints[i].y - 10);

    }
    
    //Draw lines between control points
    ofDrawLine(controlPoints[0], controlPoints[5]);
    ofDrawLine(controlPoints[1], controlPoints[4]);
    ofDrawLine(controlPoints[7], controlPoints[2]);
    ofDrawLine(controlPoints[6], controlPoints[3]);
    
    ofPopStyle();

}

void Frame::setupGui(){
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    gui.add(settingsLabel.setup("  GENERAL SETTINGS", ""));
    gui.add(waveSpeedSlider.setup("Wave Speed", 1.0f, 0.1f, 10.0f));
    gui.add(effectDurationSlider.setup("Effect Duration", 1.8f, 0.1f, 5.0f));
    
    gui.add(mappingLabel.setup("  Mapping Points", ""));
    gui.add(reMapMeshButton.setup("Re-Map Mesh"));
    
    //slider bounds for frame corner
    float maxRange = 50;
    ofVec2f plus(maxRange, maxRange);
    ofVec2f minus(-maxRange, -maxRange);
    
    gui.add(frameCorner0.setup("Corner 0", frameCorners[0], frameCorners[0] + minus, frameCorners[0] + plus));
    gui.add(frameCorner1.setup("Corner 1", frameCorners[1], frameCorners[1] + minus, frameCorners[1] + plus));
    gui.add(frameCorner2.setup("Corner 2", frameCorners[2], frameCorners[2] + minus, frameCorners[2] + plus));
    gui.add(frameCorner3.setup("Corner 3", frameCorners[3], frameCorners[3] + minus, frameCorners[3] + plus));
    
    gui.add(controlPtPct0.setup("Control Pt 0", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct1.setup("Control Pt 1", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct2.setup("Control Pt 2", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct3.setup("Control Pt 3", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct4.setup("Control Pt 4", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct5.setup("Control Pt 5", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct6.setup("Control Pt 6", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct7.setup("Control Pt 7", 0.1f, 0.0f, 0.2f));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    settingsLabel.setBackgroundColor(ofColor(255));
    mappingLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    settingsLabel.setDefaultTextColor(ofColor(0));
    
}

void Frame::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    //set runtime variables to loaded GUI settings
    frameCorners[0] = frameCorner0;
    frameCorners[1] = frameCorner1;
    frameCorners[2] = frameCorner2;
    frameCorners[3] = frameCorner3;
    
    controlPointPcts[0] = controlPtPct0;
    controlPointPcts[1] = controlPtPct1;
    controlPointPcts[2] = controlPtPct2;
    controlPointPcts[3] = controlPtPct3;
    controlPointPcts[4] = controlPtPct4;
    controlPointPcts[5] = controlPtPct5;
    controlPointPcts[6] = controlPtPct6;
    controlPointPcts[7] = controlPtPct7;

}

void Frame::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}

void Frame::drawGui(int x, int y){
 
//    gui.setPosition(x, y);
    gui.draw();
    
}

void Frame::prepareMesh(){
    
    /*
     -----set up the mesh/tiles-----
     Control points for frame:
     
        ___0___________________1___     ________
       |   |   |   |   |   |   |   |            This thickness in texture = borderHeight
     7_|___|___|___|___|___|___|___|_2  ________Percentage of total height = 0.1017
       |   |                   |   |
       |___|                   |___|
       |   |                   |   |
       |___|                   |___|
       |   |                   |   |
       |___|                   |___|    ________
       |   |                   |   |            This percentage is the rest of the height
       |___|                   |___|    ________minus borderHeight*2 divided by 8 sections
       |   |                   |   |            "vertBorderTileHeightPct"
       |___|                   |___|
       |   |                   |   |            Same for "horizBorderTileWidthPct"
       |___|                   |___|            but with 5 sections
       |   |                   |   |
       |___|                   |___|
       |   |                   |   |
     6_|___|___________________|___|_3
       |   |   |   |   |   |   |   |
       |___|___|___|___|___|___|___|
           5                   4
       |   |
       |   |
       |   |
       |   | This thickness in texture =  borderWidth
             Percentage of total width = 0.1296
     
     
     Horizontal frame sections divided into 5 tiles
     Vertical frame sections divded into 8 tiles
     (not including corner tiles)
     
     */
    
    //Get texCoords from the image template
    frameWidth = images[0].getWidth();
    frameHeight = images[0].getHeight();
    
    //Thickness of frame border from template as a fraction of the width/height
    borderWidthPct = 0.1296;
    borderHeightPct = 0.1017;
    
    vertBorderTileHeightPct = (1.0f - (borderHeightPct * 2.0f))/8.0f;
    horizBorderTileWidthPct = (1.0f - (borderWidthPct * 2.0f))/5.0f;
    
    float heightFromTop = 170;
    
    //Frame corners hold the actual points that will be moved around
    //to map the mesh to the physical frame. Start them off as square
    //but they will be overwritten by loaded GUI settings
    frameCorners.resize(4);
    frameCorners[0] = ofVec2f( ofGetWidth()/2 - frameWidth/2, heightFromTop );
    frameCorners[1] = frameCorners[0] + ofVec2f( frameWidth, 0           );
    frameCorners[2] = frameCorners[0] + ofVec2f( frameWidth, frameHeight );
    frameCorners[3] = frameCorners[0] + ofVec2f( 0         , frameHeight );
    
    cout << "Frame width: " << frameWidth  << endl;
    
    
    //these floats are how far along the edge of the portrait they are
    //(left to right or top to bottom) as a percentage of the distance from one corner to the next
    //Declared here for first mesh creation, later to be replaced with loaded GUI settings
    controlPointPcts.resize(8);
    controlPointPcts[0] = borderWidthPct;
    controlPointPcts[1] = 1.0f - borderWidthPct;
    controlPointPcts[2] = borderHeightPct;
    controlPointPcts[3] = 1.0f - borderHeightPct;
    controlPointPcts[4] = 1.0f - borderWidthPct;
    controlPointPcts[5] = borderWidthPct;
    controlPointPcts[6] = 1.0f - borderHeightPct;
    controlPointPcts[7] = borderHeightPct;
    
    //texCoordControlPts are like control points but aligned with the image template
    //and not altered by points remapped to the physical space by the GUI
    
    texCoordCorners.resize(4);
    texCoordCorners[0] = ofVec2f( 0         , 0           );
    texCoordCorners[1] = ofVec2f( frameWidth, 0           );
    texCoordCorners[2] = ofVec2f( frameWidth, frameHeight );
    texCoordCorners[3] = ofVec2f( 0         , frameHeight );
    
    texCoordControlPts.resize(8);
    texCoordControlPts[0] = frameCorners[0].getInterpolated(frameCorners[1], borderWidthPct);
    texCoordControlPts[1] = frameCorners[0].getInterpolated(frameCorners[1], 1.0f - borderWidthPct);
    texCoordControlPts[2] = frameCorners[1].getInterpolated(frameCorners[2], borderHeightPct);
    texCoordControlPts[3] = frameCorners[1].getInterpolated(frameCorners[2], 1.0f - borderHeightPct);
    texCoordControlPts[4] = frameCorners[3].getInterpolated(frameCorners[2], 1.0f - borderWidthPct);
    texCoordControlPts[5] = frameCorners[3].getInterpolated(frameCorners[2], borderWidthPct);
    texCoordControlPts[6] = frameCorners[0].getInterpolated(frameCorners[3], 1.0f - borderHeightPct);
    texCoordControlPts[7] = frameCorners[0].getInterpolated(frameCorners[3], borderHeightPct);
    
    //subtract the top left corner of the frame from the
    //tex coords since the image starts at the origin
    for(int i = 0; i < texCoordControlPts.size(); i++){
        texCoordControlPts[i] -= frameCorners[0];
    }
    
    
}

void Frame::mapMesh(){
    

    

    
    
    
    
    
    //ofVec2f controlPoints are for convenience and calculated
    //based on controlPointsPcts vector. Recalculate them in case settings have changed
    controlPoints.resize(8);
    controlPoints[0] = frameCorners[0].getInterpolated(frameCorners[1], controlPointPcts[0]);
    controlPoints[1] = frameCorners[0].getInterpolated(frameCorners[1], controlPointPcts[1]);
    controlPoints[2] = frameCorners[1].getInterpolated(frameCorners[2], controlPointPcts[2]);
    controlPoints[3] = frameCorners[1].getInterpolated(frameCorners[2], controlPointPcts[3]);
    controlPoints[4] = frameCorners[3].getInterpolated(frameCorners[2], controlPointPcts[4]);
    controlPoints[5] = frameCorners[3].getInterpolated(frameCorners[2], controlPointPcts[5]);
    controlPoints[6] = frameCorners[0].getInterpolated(frameCorners[3], controlPointPcts[6]);
    controlPoints[7] = frameCorners[0].getInterpolated(frameCorners[3], controlPointPcts[7]);
    
    
    
    
    /*
     * Construct the tiles from vertices and
     * load them into the tiles vector
     *
     * Tiles vertex arrangement
     *
     *   0____1
     *   |    |
     *   |____|
     *   3    2
     *
     */
    
    
    vector<ofVec3f> verts;
    verts.resize(4);
    
    vector<ofVec2f> texCoords;
    texCoords.resize(4);
    
    //----------TOP ROW----------
    //Interpolates vertices between
    //frameCorners/controlPoints from left to right
    
    //Construct the first row (2 corners + 5 middle = 7 tiles)
    //BOUNDED BY...
    //top left:     frameCorners[0]
    //bottom left:  controlPoint 7
    //top Right:    frameCorners[1]
    //bottom Right: controlPoint 2
    for(int i = 0; i < 7; i++){
        
        float startPct, endPct;
        
        if(i == 0){
            
            //left Tile
            startPct = 0.0f;
            endPct = controlPointPcts[0];
            
        } else if(i == 6){
            
            //right tile
            startPct = controlPointPcts[1];
            endPct = 1.0f;
            
        } else {
            
            //middle tiles
            startPct = controlPointPcts[0] + horizBorderTileWidthPct * (i - 1);
            endPct = startPct + horizBorderTileWidthPct;
        }
        
        verts[0] = frameCorners[0].getInterpolated(frameCorners[1], startPct);
        verts[1] = frameCorners[0].getInterpolated(frameCorners[1], endPct);
        verts[2] = controlPoints[7].getInterpolated(controlPoints[2], endPct);
        verts[3] = controlPoints[7].getInterpolated(controlPoints[2], startPct);
        
        //texCoords have similar construction but they use the default "borderWidthPct" and
        //"borderHeightPct" values from the image template instead of the control
        //points that will be moved with the GUI
        texCoords[0] = texCoordCorners[0].getInterpolated(texCoordCorners[1], startPct);
        texCoords[1] = texCoordCorners[0].getInterpolated(texCoordCorners[1], endPct);
        texCoords[2] = texCoordControlPts[7].getInterpolated(texCoordControlPts[2], endPct);
        texCoords[3] = texCoordControlPts[7].getInterpolated(texCoordControlPts[2], startPct);
        
        
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
    }
    
    //----------BOTTOM ROW----------
    //Interpolates vertices between
    //frameCorners/controlPoints from left to right
    for(int i = 0; i < 7; i++){
        
        float startPct, endPct;
        
        if(i == 0){
            
            //left Tile
            startPct = 0.0f;
            endPct = controlPointPcts[5];
            
        } else if(i == 6){
            
            //right tile
            startPct = controlPointPcts[4];
            endPct = 1.0f;
            
        } else {
            
            //middle tiles
            startPct = controlPointPcts[5] + horizBorderTileWidthPct * (i - 1);
            endPct = startPct + horizBorderTileWidthPct;
        }
        
        verts[0] = controlPoints[6].getInterpolated(controlPoints[3], startPct);
        verts[1] = controlPoints[6].getInterpolated(controlPoints[3], endPct);
        verts[2] = frameCorners[3].getInterpolated(frameCorners[2], endPct);
        verts[3] = frameCorners[3].getInterpolated(frameCorners[2], startPct);
        
        //texCoords have similar construction but they use the default "borderWidthPct" and
        //"borderHeightPct" values instead of the control points that will be moved with the GUI
        texCoords[0] = texCoordControlPts[6].getInterpolated(texCoordControlPts[3], startPct);
        texCoords[1] = texCoordControlPts[6].getInterpolated(texCoordControlPts[3], endPct);
        texCoords[2] = texCoordCorners[3].getInterpolated(texCoordCorners[2], endPct);
        texCoords[3] = texCoordCorners[3].getInterpolated(texCoordCorners[2], startPct);
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
    }
    
    //----------LEFT COLUMN----------
    //Interpolates vertices betweem
    //frameCorners/controlPoints from top to bottom
    
    //8 tiles between top row and bottom row
    for(int i = 0; i < 8; i++){
        
        //middle tiles
        float startPct = borderHeightPct + (vertBorderTileHeightPct * i);
        float endPct = startPct + vertBorderTileHeightPct;
        
        verts[0] = frameCorners[0].getInterpolated(frameCorners[3], startPct);
        verts[1] = controlPoints[0].getInterpolated(controlPoints[5], startPct);
        verts[2] = controlPoints[0].getInterpolated(controlPoints[5], endPct);
        verts[3] = frameCorners[0].getInterpolated(frameCorners[3], endPct);
        
        //texCoords have similar construction but they use the default "borderWidthPct" and
        //"borderHeightPct" values instead of the control points that will be moved with the GUI
        texCoords[0] = texCoordCorners[0].getInterpolated(texCoordCorners[3], startPct);
        texCoords[1] = texCoordControlPts[0].getInterpolated(texCoordControlPts[5], startPct);
        texCoords[2] = texCoordControlPts[0].getInterpolated(texCoordControlPts[5], endPct);
        texCoords[3] = texCoordCorners[0].getInterpolated(texCoordCorners[3], endPct);
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
    }
    
    //----------RIGHT COLUMN----------
    //Interpolates vertices betweem
    //frameCorners/controlPoints from top to bottom
    
    //8 tiles between top row and bottom row
    for(int i = 0; i < 8; i++){
        
        //middle tiles
        float startPct = borderHeightPct + (vertBorderTileHeightPct * i);
        float endPct = startPct + vertBorderTileHeightPct;
        
        verts[0] = controlPoints[1].getInterpolated(controlPoints[4], startPct);
        verts[1] = frameCorners[1].getInterpolated(frameCorners[2], startPct);
        verts[2] = frameCorners[1].getInterpolated(frameCorners[2], endPct);
        verts[3] = controlPoints[1].getInterpolated(controlPoints[4], endPct);
        
        //texCoords have similar construction but they use the default "borderWidthPct" and
        //"borderHeightPct" values instead of the control points that will be moved with the GUI
        texCoords[0] = texCoordControlPts[1].getInterpolated(texCoordControlPts[4], startPct);
        texCoords[1] = texCoordCorners[1].getInterpolated(texCoordCorners[2], startPct);
        texCoords[2] = texCoordCorners[1].getInterpolated(texCoordCorners[2], endPct);
        texCoords[3] = texCoordControlPts[1].getInterpolated(texCoordControlPts[4], endPct);
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
    }

    
}






