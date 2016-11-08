//
//  Bookcase.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/7/16.
//
//

#include "Bookcase.hpp"




Bookcase::Bookcase(){
    
    
    
}

void Bookcase::setup(string name, bool _leftCase){
    
    //are we left or right
    bLeftCase = _leftCase;
    
    
    //load all the images from file
    ofDirectory dir;
    
    dir.listDir("images/bookcases/");
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
    //        frameCorner0 = bookcaseCorners[0];
    //        frameCorner1 = bookcaseCorners[1];
    //        frameCorner2 = bookcaseCorners[2];
    //        frameCorner3 = bookcaseCorners[3];
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
    
//    loadSettings();
    
    
    
    mapMesh();
    
    
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject();
    
    
}


void Bookcase::update(){
    
    //Distribute GUI values to where they belong
    waveSpeed = waveSpeedSlider;
    
    for(int i = 0; i < tiles.size(); i++){
        tiles[i].effectDuration = effectDurationSlider;
    }
    
    //set variables from GUI settings
//    bookcaseCorners[0] = frameCorner0;
//    bookcaseCorners[1] = frameCorner1;
//    bookcaseCorners[2] = frameCorner2;
//    bookcaseCorners[3] = frameCorner3;
//    
//    controlPointPcts[0] = controlPtPct0;
//    controlPointPcts[1] = controlPtPct1;
//    controlPointPcts[2] = controlPtPct2;
//    controlPointPcts[3] = controlPtPct3;
//    controlPointPcts[4] = controlPtPct4;
//    controlPointPcts[5] = controlPtPct5;
//    controlPointPcts[6] = controlPtPct6;
//    controlPointPcts[7] = controlPtPct7;
//    
//    //ofVec2f controlPoints are for convenience and calculated
//    //based on controlPointsPcts vector. Recalculate them in case settings have changed
//    controlPoints.resize(8);
//    controlPoints[0] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[0]);
//    controlPoints[1] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[1]);
//    controlPoints[2] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[2]);
//    controlPoints[3] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[3]);
//    controlPoints[4] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[4]);
//    controlPoints[5] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[5]);
//    controlPoints[6] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[6]);
//    controlPoints[7] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[7]);
    
    
    if(reMapMeshButton){
        tiles.clear();
        mapMesh();
    }
    
    
    TiledObject::update();
    
}

void Bookcase::draw(){
    
    TiledObject::draw();
    
    
    
}

void Bookcase::drawDebug(){
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    
    
    //draw corners
    for(int i = 0; i < bookcaseCorners.size(); i++){
        
        ofSetColor(255, 0, 0);
        ofDrawCircle(bookcaseCorners[i], 10);
        ofDrawBitmapStringHighlight(ofToString(i), bookcaseCorners[i].x + 10, bookcaseCorners[i].y - 10);
        
    }
    
    ofPolyline corners;
    corners.addVertex(bookcaseCorners[0]);
    corners.addVertex(bookcaseCorners[1]);
    corners.addVertex(bookcaseCorners[2]);
    corners.addVertex(bookcaseCorners[3]);
    corners.close();
    corners.draw();
    
    //draw control points
    for(int i = 0; i < controlPoints.size(); i++){
        
        ofSetColor(0, 255, 0);
        ofDrawCircle(controlPoints[i], 10);
        ofDrawBitmapStringHighlight(ofToString(i), controlPoints[i].x + 10, controlPoints[i].y - 10);
        
    }
    
    //Draw lines between control points
    ofDrawLine(controlPoints[0], controlPoints[9]);
    ofDrawLine(controlPoints[1], controlPoints[8]);
    ofDrawLine(controlPoints[15], controlPoints[2]);
    ofDrawLine(controlPoints[14], controlPoints[3]);
    ofDrawLine(controlPoints[13], controlPoints[4]);
    ofDrawLine(controlPoints[12], controlPoints[5]);
    ofDrawLine(controlPoints[11], controlPoints[6]);
    ofDrawLine(controlPoints[10], controlPoints[7]);
    
    
    ofPopStyle();
    
}

void Bookcase::setupGui(){
    
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
    
    gui.add(frameCorner0.setup("Corner 0", bookcaseCorners[0], bookcaseCorners[0] + minus, bookcaseCorners[0] + plus));
    gui.add(frameCorner1.setup("Corner 1", bookcaseCorners[1], bookcaseCorners[1] + minus, bookcaseCorners[1] + plus));
    gui.add(frameCorner2.setup("Corner 2", bookcaseCorners[2], bookcaseCorners[2] + minus, bookcaseCorners[2] + plus));
    gui.add(frameCorner3.setup("Corner 3", bookcaseCorners[3], bookcaseCorners[3] + minus, bookcaseCorners[3] + plus));
    
    gui.add(controlPtPct0.setup("Control Pt 0", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct1.setup("Control Pt 1", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct2.setup("Control Pt 2", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct3.setup("Control Pt 3", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct4.setup("Control Pt 4", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct5.setup("Control Pt 5", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct6.setup("Control Pt 6", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct7.setup("Control Pt 7", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct8.setup("Control Pt 8", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct9.setup("Control Pt 9", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct10.setup("Control Pt 10", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct11.setup("Control Pt 11", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct12.setup("Control Pt 12", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct13.setup("Control Pt 13", 0.1f, 0.0f, 0.2f));
    gui.add(controlPtPct14.setup("Control Pt 14", 0.1f, 0.8f, 1.0f));
    gui.add(controlPtPct15.setup("Control Pt 15", 0.1f, 0.0f, 0.2f));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    settingsLabel.setBackgroundColor(ofColor(255));
    mappingLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    settingsLabel.setDefaultTextColor(ofColor(0));
    
    
    float x = bLeftCase ? ofGetWidth()/2 - 100 : ofGetWidth() - 220;
    gui.setPosition(x, 10);
    
    
}

void Bookcase::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    //set runtime variables to loaded GUI settings
    bookcaseCorners[0] = frameCorner0;
    bookcaseCorners[1] = frameCorner1;
    bookcaseCorners[2] = frameCorner2;
    bookcaseCorners[3] = frameCorner3;
    
    controlPointPcts[0] = controlPtPct0;
    controlPointPcts[1] = controlPtPct1;
    controlPointPcts[2] = controlPtPct2;
    controlPointPcts[3] = controlPtPct3;
    controlPointPcts[4] = controlPtPct4;
    controlPointPcts[5] = controlPtPct5;
    controlPointPcts[6] = controlPtPct6;
    controlPointPcts[7] = controlPtPct7;
    
}

void Bookcase::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}

void Bookcase::drawGui(){
    gui.draw();
}

void Bookcase::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}


void Bookcase::prepareMesh(){

    
    //Get texCoords from the image template
    bookcaseWidth = images[0].getWidth();
    bookcaseHeight = images[0].getHeight();
    
    //Tile sizes as percentage of overall width/height
    //see bookcase_mapping.png for details
    edgeTileWidth       = 0.0991f;
    midTileWidth        = 0.1002f;
    topTileHeight       = 0.0456f;
    bookTileHeight      = 0.0449f;
    shelfTileHeight     = 0.0143f;
    bottomTileHeight    = 0.0477f;
    
    float fudge = 50;
    float heightFromTop = ofGetWindowHeight() - bookcaseHeight - fudge;
    float leftMargin;
    
    if(bLeftCase){
        //from design layout files
        leftMargin = 183;
    } else {
        
        leftMargin = ofGetWindowWidth() - bookcaseWidth - 183;
        
    }
    
    //Frame corners hold the actual points that will be moved around
    //to map the mesh to the physical frame. Start them off as square
    //but they will be overwritten by loaded GUI settings
    bookcaseCorners.resize(4);
    bookcaseCorners[0] = ofVec2f( leftMargin, heightFromTop );
    bookcaseCorners[1] = bookcaseCorners[0] + ofVec2f( bookcaseWidth, 0              );
    bookcaseCorners[2] = bookcaseCorners[0] + ofVec2f( bookcaseWidth, bookcaseHeight );
    bookcaseCorners[3] = bookcaseCorners[0] + ofVec2f( 0            , bookcaseHeight );
    
    //these floats are how far along the edge of the portrait they are
    //(left to right or top to bottom) as a percentage of the distance from one corner to the next
    //Declared here for first mesh creation, later to be replaced with loaded GUI settings
    //SEE REFERENCE IMAGES (TRUST ME)
    controlPointPcts.resize(16);
    controlPointPcts[0] = edgeTileWidth;
    controlPointPcts[1] = 1.0f - edgeTileWidth;
    controlPointPcts[2] = topTileHeight * 2;
    controlPointPcts[3] = controlPointPcts[2] + bookTileHeight * 3;
    controlPointPcts[4] = controlPointPcts[3] + shelfTileHeight;
    controlPointPcts[5] = controlPointPcts[4] + bookTileHeight * 3;
    controlPointPcts[6] = controlPointPcts[5] + shelfTileHeight;
    controlPointPcts[7] = controlPointPcts[6] + bookTileHeight * 3;
    controlPointPcts[8] = 1.0f - edgeTileWidth;
    controlPointPcts[9] = edgeTileWidth;
    controlPointPcts[10] = controlPointPcts[7];
    controlPointPcts[11] = controlPointPcts[6];
    controlPointPcts[12] = controlPointPcts[5];
    controlPointPcts[13] = controlPointPcts[4];
    controlPointPcts[14] = controlPointPcts[3];
    controlPointPcts[15] = controlPointPcts[2];
    
    //texCoordControlPts are like control points but aligned with the image template
    //and not altered by points remapped to the physical space by the GUI
    
    texCoordCorners.resize(4);
    texCoordCorners[0] = ofVec2f( 0            , 0              );
    texCoordCorners[1] = ofVec2f( bookcaseWidth, 0              );
    texCoordCorners[2] = ofVec2f( bookcaseWidth, bookcaseHeight );
    texCoordCorners[3] = ofVec2f( 0            , bookcaseHeight );
    
    texCoordControlPts.resize(16);
    //top from left to right
    texCoordControlPts[0] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[0]);
    texCoordControlPts[1] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[1]);
    //right side from top to bottom
    texCoordControlPts[2] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[2]);
    texCoordControlPts[3] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[3]);
    texCoordControlPts[4] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[4]);
    texCoordControlPts[5] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[5]);
    texCoordControlPts[6] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[6]);
    texCoordControlPts[7] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[7]);
    //bottom from left to right
    texCoordControlPts[8] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[8]);
    texCoordControlPts[9] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[9]);
    //left side from top to bottom
    texCoordControlPts[10] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[10]);
    texCoordControlPts[11] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[11]);
    texCoordControlPts[12] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[12]);
    texCoordControlPts[13] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[13]);
    texCoordControlPts[14] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[14]);
    texCoordControlPts[15] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[15]);
    
    //subtract the top left corner of the frame from the
    //tex coords since the image starts at the origin
    for(int i = 0; i < texCoordControlPts.size(); i++){
        texCoordControlPts[i] -= bookcaseCorners[0];
    }
    
    
}

void Bookcase::mapMesh(){
    
    //ofVec2f controlPoints are for convenience and calculated
    //based on controlPointsPcts vector. Recalculate them in case settings have changed
    controlPoints.resize(16);
    //top from left to right
    controlPoints[0] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[0]);
    controlPoints[1] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[1]);
    //right side from top to bottom
    controlPoints[2] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[2]);
    controlPoints[3] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[3]);
    controlPoints[4] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[4]);
    controlPoints[5] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[5]);
    controlPoints[6] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[6]);
    controlPoints[7] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[7]);
    //bottom from left to right
    controlPoints[8] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[8]);
    controlPoints[9] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[9]);
    //left side from top to bottom
    controlPoints[10] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[10]);
    controlPoints[11] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[11]);
    controlPoints[12] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[12]);
    controlPoints[13] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[13]);
    controlPoints[14] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[14]);
    controlPoints[15] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[15]);
    
    
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
    
    //----------Bookcase LEFT Edge----------
    //      ASSEMBLED TOP TO BOTTOM
    //two columns of 23 tiles all with width = edgeTileWidth
    //and varying heights
    
    float startPct = 0.0f;
    float endPct = 0.0f;
    float tileHeightPct = 0.0f;
    
    //two outer for loops, one for each column
    for(int j = 0; j < 2; j++){
        
        ofVec2f topLeft;
        ofVec2f topRight;
        ofVec2f bottomRight;
        ofVec2f bottomLeft;
        
        ofVec2f texTopLeft;
        ofVec2f texTopRight;
        ofVec2f texBottomRight;
        ofVec2f texBottomLeft;
        
        //select points for left column
        if(j == 0){
            
            topLeft = bookcaseCorners[0];
            topRight = controlPoints[0];
            bottomRight = controlPoints[9];
            bottomLeft = bookcaseCorners[3];
            
            texTopLeft = texCoordCorners[0];
            texTopRight = texCoordControlPts[0];
            texBottomRight = texCoordControlPts[9];
            texBottomLeft = texCoordCorners[3];
            
        } else {
            //right column is between...
            topLeft = controlPoints[1];
            topRight = bookcaseCorners[1];
            bottomRight = bookcaseCorners[2];
            bottomLeft = controlPoints[8];
            
            texTopLeft = texCoordControlPts[1];
            texTopRight = texCoordCorners[1];
            texBottomRight = texCoordCorners[2];
            texBottomLeft = texCoordControlPts[8];
            
        }
        
        for(int i = 0; i < 23; i++){
            
            
            //Determine height of this tile (it will
            //be added to startPct later)
            if(i <= 1){
                
                //halfway from top to control point
                tileHeightPct = controlPointPcts[15]/2.0f;
                
                
            } else if(i == 2 || i <= 4){  //first book slot
                
                //one third of the height of inside the book shelf
                tileHeightPct = (controlPointPcts[14] - controlPointPcts[15])/3.0f;
                
            } else if(i == 5){  //first shelf
                
                tileHeightPct = controlPointPcts[13] - controlPointPcts[14];
                
            } else if(i == 6 || i <= 8){  //second book slot
                
                tileHeightPct = (controlPointPcts[12] - controlPointPcts[13])/3.0f;
                
            } else if(i == 9){  //second shelf
                
                tileHeightPct = controlPointPcts[11] - controlPointPcts[12];
                
            } else if(i == 10 || i <= 12){  //third book slot
                
                tileHeightPct = (controlPointPcts[10] - controlPointPcts[11])/3.0f;
                
            } else {  //bottom tiles
                
                tileHeightPct = (1.0f - controlPointPcts[10])/10.0f;
                
            }
            
            endPct = startPct + tileHeightPct;
            
            verts[0] = topLeft.getInterpolated(bottomLeft, startPct);
            verts[1] = topRight.getInterpolated(bottomRight, startPct);
            verts[2] = topRight.getInterpolated(bottomRight, endPct);
            verts[3] = topLeft.getInterpolated(bottomLeft, endPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values from the image template instead of the control
            //points that will be moved with the GUI
            texCoords[0] = texTopLeft.getInterpolated(texBottomLeft, startPct);
            texCoords[1] = texTopRight.getInterpolated(texBottomRight, startPct);
            texCoords[2] = texTopRight.getInterpolated(texBottomRight, endPct);
            texCoords[3] = texTopLeft.getInterpolated(texBottomLeft, endPct);

            
            //add tileHeight to startPct so we know where to start the next tile
            startPct += tileHeightPct;
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
            
        }
        
        //start from the top for the next column
        startPct = 0;
    }
    

    //----------Bookcase TOP MIDDLE Tiles----------
    //          ASSEMBLED LEFT TO RIGHT
    //two rows of 8 tiles between controlPoints 0 and 1 (left to right)
    
    startPct = 0.0f;
    endPct = 0.0f;
    
    //use width now since we're going across
    float tileWidthPct = 0.0f;
    
    //Two outer for loops, one for each row
    for(int j = 0; j < 2; j++){
        
        ofVec2f topLeft;
        ofVec2f topRight;
        ofVec2f bottomRight;
        ofVec2f bottomLeft;
        
        ofVec2f texTopLeft;
        ofVec2f texTopRight;
        ofVec2f texBottomRight;
        ofVec2f texBottomLeft;
        
        //select points for left column
        if(j == 0){
            
            topLeft = bookcaseCorners[0];
            topRight = controlPoints[0];
            bottomRight = controlPoints[9];
            bottomLeft = bookcaseCorners[3];
            
            texTopLeft = texCoordCorners[0];
            texTopRight = texCoordControlPts[0];
            texBottomRight = texCoordControlPts[9];
            texBottomLeft = texCoordCorners[3];
            
        } else {
            //right column is between...
            topLeft = controlPoints[1];
            topRight = bookcaseCorners[1];
            bottomRight = bookcaseCorners[2];
            bottomLeft = controlPoints[8];
            
            texTopLeft = texCoordControlPts[1];
            texTopRight = texCoordCorners[1];
            texBottomRight = texCoordCorners[2];
            texBottomLeft = texCoordControlPts[8];
            
        }
        
        for(int i = 0; i < 23; i++){
            
            tileWidthPct = (controlPointPcts[1] - controlPointPcts[0])/8.0f;
            
            endPct = startPct + tileWidthPct;
            
            verts[0] = topLeft.getInterpolated(bottomLeft, startPct);
            verts[1] = topRight.getInterpolated(bottomRight, startPct);
            verts[2] = topRight.getInterpolated(bottomRight, endPct);
            verts[3] = topLeft.getInterpolated(bottomLeft, endPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values from the image template instead of the control
            //points that will be moved with the GUI
            texCoords[0] = texTopLeft.getInterpolated(texBottomLeft, startPct);
            texCoords[1] = texTopRight.getInterpolated(texBottomRight, startPct);
            texCoords[2] = texTopRight.getInterpolated(texBottomRight, endPct);
            texCoords[3] = texTopLeft.getInterpolated(texBottomLeft, endPct);
            
            
            //add tileHeight to startPct so we know where to start the next tile
            startPct += tileHeightPct;
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
            
        }
        
        //start from the top for the next column
        startPct = 0;
    }
    
    
    
}






