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

void Bookcase::loadMedia(){
    
    //load all the images from file
    ofDirectory dir;
    dir.listDir("images/bookcases/");
    dir.sort();
    
    //load images with manual file names
    //loading with ofDirectory conflicts with ofxAssimp
    for(int i = 0; i < (int)dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        
        images.push_back(img);
        
    }
    
//    currentImg = round(ofRandom( images.size() - 1 ));
    currentImg = 0;
    
}

void Bookcase::setup(string name, bool _leftCase){
    
    //are we left or right
    bLeftCase = _leftCase;
    
    
    
    
    //ALL THE BUSINESS HAPPENS IN HERE
    //This will need to be called whenever the mesh is altered by GUI/mouse
    prepareMesh();
    

    guiName = name;
    
    setupGui();
    
    loadSettings();
    
    mapMesh();
    
    //Set GUI values to these (Only needed for first population of GUI XML)
//    frameCorner0 = bookcaseCorners[0];
//    frameCorner1 = bookcaseCorners[1];
//    frameCorner2 = bookcaseCorners[2];
//    frameCorner3 = bookcaseCorners[3];
//    controlPtPct0 = controlPointPcts[0];
//    controlPtPct1 = controlPointPcts[1];
//    controlPtPct2 = controlPointPcts[2];
//    controlPtPct3 = controlPointPcts[3];
//    controlPtPct4 = controlPointPcts[4];
//    controlPtPct5 = controlPointPcts[5];
//    controlPtPct6 = controlPointPcts[6];
//    controlPtPct7 = controlPointPcts[7];
//    controlPtPct8 = controlPointPcts[8];
//    controlPtPct9 = controlPointPcts[9];
//    controlPtPct10 = controlPointPcts[10];
//    controlPtPct11 = controlPointPcts[11];
//    controlPtPct12 = controlPointPcts[12];
//    controlPtPct13 = controlPointPcts[13];
//    controlPtPct14 = controlPointPcts[14];
//    controlPtPct15 = controlPointPcts[15];
//    saveSettings();
    
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject(true);   //TRUE, this is a bookcase
    
    
}


void Bookcase::update(){
    
    TiledObject::updateCommonGui();
    
    
    //this only updates variables from the GUI values if the GUI is on screen
    //Simple hack to avoid using listeners/callbacks for EACH of the GUI elements
    if(bIsGuiActive){
    
        //set variables from GUI settings
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
        controlPointPcts[8] = controlPtPct8;
        controlPointPcts[9] = controlPtPct9;
        controlPointPcts[10] = controlPtPct10;
        controlPointPcts[11] = controlPtPct11;
        controlPointPcts[12] = controlPtPct12;
        controlPointPcts[13] = controlPtPct13;
        controlPointPcts[14] = controlPtPct14;
        controlPointPcts[15] = controlPtPct15;
        
        //ofVec2f controlPoints are for convenience and calculated
        //based on controlPointsPcts vector. Recalculate them in case settings have changed
        controlPoints[0] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[0]);
        controlPoints[1] = bookcaseCorners[0].getInterpolated(bookcaseCorners[1], controlPointPcts[1]);
        controlPoints[2] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[2]);
        controlPoints[3] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[3]);
        controlPoints[4] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[4]);
        controlPoints[5] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[5]);
        controlPoints[6] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[6]);
        controlPoints[7] = bookcaseCorners[1].getInterpolated(bookcaseCorners[2], controlPointPcts[7]);
        controlPoints[8] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[8]);
        controlPoints[9] = bookcaseCorners[3].getInterpolated(bookcaseCorners[2], controlPointPcts[9]);
        controlPoints[10] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[10]);
        controlPoints[11] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[11]);
        controlPoints[12] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[12]);
        controlPoints[13] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[13]);
        controlPoints[14] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[14]);
        controlPoints[15] = bookcaseCorners[0].getInterpolated(bookcaseCorners[3], controlPointPcts[15]);
        
        if(reMapMeshButton && ofGetElapsedTimeMillis() - lastMapTime > 1000){
            tiles.clear();
            mapMesh();
            
            lastMapTime = ofGetElapsedTimeMillis();
        }
        
    }
    
    TiledObject::update();
    
    //this will be set to true later if we're drawing the gui.
    bIsGuiActive = false;
    
    

    
}

//shadow is the black background behind the bookcase that
//prevents the wallpaper from being visible on the physicall
//raised wooden frames. Called from ofApp between
//enable/disable depth test calls
void Bookcase::drawShadow(){
    
    ofSetColor(ofColor(0));
    shadow.draw();
    
}


//NOT WORKING: Mesh draws but alpha blending does not
//work so objects behind are not visible
void Bookcase::drawShelfOverlay(int shelfNum, int trans){
    
//    shelfOverlays[shelfNum].setColor(ofColor(0, 255, 0, trans));
    ofSetColor(0, 255, 0, trans);
    shelfOverlays[shelfNum].draw();
    
    
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
    
    //if we're drawing the gui, then it's active so
    //we'll update all the values in update()
    bIsGuiActive = true;
    
}




//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------GUI SETUP AND DRAWING---------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


void Bookcase::drawGui(){

    gui.draw();
    
}

void Bookcase::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}

void Bookcase::setupGui(){
    
    TiledObject::setupCommonGui();
    
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
    
    float start = 0.0f;
    float firstStep = 0.15f;
    float midStep = 0.6f;
    float lastStep = 0.85f;
    float end = 1.0f;
    
    
    gui.add(controlPtPct0.setup("Control Pt 0", controlPointPcts[0], start, firstStep));
    gui.add(controlPtPct1.setup("Control Pt 1", controlPointPcts[1], lastStep, end));
    gui.add(controlPtPct2.setup("Control Pt 2", controlPointPcts[2], start, midStep));
    gui.add(controlPtPct3.setup("Control Pt 3", controlPointPcts[3], start, midStep));
    gui.add(controlPtPct4.setup("Control Pt 4", controlPointPcts[4], start, midStep));
    gui.add(controlPtPct5.setup("Control Pt 5", controlPointPcts[5], start, midStep));
    gui.add(controlPtPct6.setup("Control Pt 6", controlPointPcts[6], start, midStep));
    gui.add(controlPtPct7.setup("Control Pt 7", controlPointPcts[7], start, midStep));
    gui.add(controlPtPct8.setup("Control Pt 8", controlPointPcts[8], lastStep, end));
    gui.add(controlPtPct9.setup("Control Pt 9", controlPointPcts[9], start, firstStep));
    gui.add(controlPtPct10.setup("Control Pt 10", controlPointPcts[10], start, midStep));
    gui.add(controlPtPct11.setup("Control Pt 11", controlPointPcts[11], start, midStep));
    gui.add(controlPtPct12.setup("Control Pt 12", controlPointPcts[12], start, midStep));
    gui.add(controlPtPct13.setup("Control Pt 13", controlPointPcts[13], start, midStep));
    gui.add(controlPtPct14.setup("Control Pt 14", controlPointPcts[14], start, midStep));
    gui.add(controlPtPct15.setup("Control Pt 15", controlPointPcts[15], start, midStep));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    mappingLabel.setBackgroundColor(ofColor(255));
    
    float x = bLeftCase ? 10 : ofGetWidth() - 220;
    gui.setPosition(x, 150);
    
    
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





//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//------------------------------MESH PREPARATION AND TILING------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------



void Bookcase::prepareMesh(){

    
    //Get texCoords from the image template: 495 x 1076
    bookcaseWidth = 495;
    bookcaseHeight = 1076;
    
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
    
    ofVec2f topLeft;
    ofVec2f topRight;
    ofVec2f bottomRight;
    ofVec2f bottomLeft;
    
    ofVec2f texTopLeft;
    ofVec2f texTopRight;
    ofVec2f texBottomRight;
    ofVec2f texBottomLeft;
    
    float startPct;
    float endPct;
    float tileHeightPct;
    float tileWidthPct;
    
    
    //----------Bookcase LEFT Edge----------
    //      ASSEMBLED TOP TO BOTTOM
    //two columns of 23 tiles all with width = edgeTileWidth
    //and varying heights
    startPct = 0.0f;
    endPct = 0.0f;
    tileHeightPct = 0.0f;
    
    for(int i = 0; i < 23; i++){
        
        
        //Determine height of this tile (it will
        //be added to startPct later)
        if(i <= 1){
            
            topLeft = bookcaseCorners[0];
            topRight = controlPoints[0];
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[15], controlPoints[2]);
            bottomLeft = controlPoints[15];

            texTopLeft = texCoordCorners[0];
            texTopRight = texCoordControlPts[0];
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[15], texCoordControlPts[2]);
            texBottomLeft = texCoordControlPts[15];
            
            
            tileHeightPct = 0.5f; //two tiles in this corner
            startPct = 0.0f + tileHeightPct * (i);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 2 || i <= 4){  //first book slot
            
            topLeft = controlPoints[15];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[15], controlPoints[2]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[14], controlPoints[3]);
            bottomLeft = controlPoints[14];
            
            texTopLeft = texCoordControlPts[15];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[15], texCoordControlPts[2]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[14], texCoordControlPts[3]);
            texBottomLeft = texCoordControlPts[14];
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 2);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 5){  //first shelf
            
            topLeft = controlPoints[14];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[14], controlPoints[3]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[13], controlPoints[4]);
            bottomLeft = controlPoints[13];
            
            texTopLeft = texCoordControlPts[14];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[14], texCoordControlPts[3]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[13], texCoordControlPts[4]);
            texBottomLeft = texCoordControlPts[13];
            
            startPct = 0.0f;
            endPct = 1.0f; //one tile in shelf
            
            
        } else if(i == 6 || i <= 8){  //second book slot
            
            topLeft = controlPoints[13];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[13], controlPoints[4]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[12], controlPoints[5]);
            bottomLeft = controlPoints[12];
            
            texTopLeft = texCoordControlPts[13];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[13], texCoordControlPts[4]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[12], texCoordControlPts[5]);
            texBottomLeft = texCoordControlPts[12];
            

            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 6);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 9){  //second shelf
            
            topLeft = controlPoints[12];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[12], controlPoints[5]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[11], controlPoints[6]);
            bottomLeft = controlPoints[11];
            
            texTopLeft = texCoordControlPts[12];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[12], texCoordControlPts[5]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[11], texCoordControlPts[6]);
            texBottomLeft = texCoordControlPts[11];
            
            
            startPct = 0.0f;
            endPct = 1.0f; //one tile in shelf
            
        } else if(i == 10 || i <= 12){  //third book slot
            
            topLeft = controlPoints[11];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[11], controlPoints[6]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[10], controlPoints[7]);
            bottomLeft = controlPoints[10];
            
            texTopLeft = texCoordControlPts[11];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[11], texCoordControlPts[6]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[10], texCoordControlPts[7]);
            texBottomLeft = texCoordControlPts[10];
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 10);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 13 || i <= 22){  //bottom tiles
            
            topLeft = controlPoints[10];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[10], controlPoints[7]);
            bottomRight = controlPoints[9];
            bottomLeft = bookcaseCorners[3];
            
            texTopLeft = texCoordControlPts[10];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[10], texCoordControlPts[7]);
            texBottomRight = texCoordControlPts[9];
            texBottomLeft = texCoordCorners[3];
            
            tileHeightPct = 1.0f/10.0f; //ten tiles in bottom part
            startPct = 0.0f + tileHeightPct * (i - 13);  //from the thirteenth tile down
            endPct = startPct + tileHeightPct;
            
        }
        
        
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
        

        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
        
    }

    

    //----------Bookcase RIGHT Edge----------
    //      ASSEMBLED TOP TO BOTTOM
    //two columns of 23 tiles all with width = edgeTileWidth
    //and varying heights
    startPct = 0.0f;
    endPct = 0.0f;
    tileHeightPct = 0.0f;
    
    for(int i = 0; i < 23; i++){
        
        //Determine height of this tile (it will
        //be added to startPct later)
        if(i <= 1){
            
            topLeft = controlPoints[1];
            topRight = bookcaseCorners[1];
            bottomRight = controlPoints[2];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[15], controlPoints[2]);
            
            texTopLeft = texCoordControlPts[1];
            texTopRight = texCoordCorners[1];
            texBottomRight = texCoordControlPts[2];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[15], texCoordControlPts[2]);
            
            
            tileHeightPct = 0.5f; //two tiles in this corner
            startPct = 0.0f + tileHeightPct * (i);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 2 || i <= 4){  //first book slot
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[15], controlPoints[2]);
            topRight = controlPoints[2];
            bottomRight = controlPoints[3];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[14], controlPoints[3]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[15], texCoordControlPts[2]);
            texTopRight = texCoordControlPts[2];
            texBottomRight = texCoordControlPts[3];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[14], texCoordControlPts[3]);
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 2);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 5){  //first shelf
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[14], controlPoints[3]);
            topRight = controlPoints[3];
            bottomRight = controlPoints[4];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[13], controlPoints[4]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[14], texCoordControlPts[3]);
            texTopRight = texCoordControlPts[3];
            texBottomRight = texCoordControlPts[4];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[13], texCoordControlPts[4]);
            
            
            startPct = 0.0f;
            endPct = 1.0f; //one tile in shelf
            
            
        } else if(i == 6 || i <= 8){  //second book slot
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[13], controlPoints[4]);
            topRight = controlPoints[4];
            bottomRight = controlPoints[5];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[12], controlPoints[5]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[13], texCoordControlPts[4]);
            texTopRight = texCoordControlPts[4];
            texBottomRight = texCoordControlPts[5];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[12], texCoordControlPts[5]);
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 6);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 9){  //second shelf
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[12], controlPoints[5]);
            topRight = controlPoints[5];
            bottomRight = controlPoints[6];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[11], controlPoints[6]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[12], texCoordControlPts[5]);
            texTopRight = texCoordControlPts[5];
            texBottomRight = texCoordControlPts[6];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[11], texCoordControlPts[6]);
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 6);
            endPct = startPct + tileHeightPct;
            
            
            startPct = 0.0f;
            endPct = 1.0f; //one tile in shelf
            
        } else if(i == 10 || i <= 12){  //third book slot
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[11], controlPoints[6]);
            topRight = controlPoints[6];
            bottomRight = controlPoints[7];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[10], controlPoints[7]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[11], texCoordControlPts[6]);
            texTopRight = texCoordControlPts[6];
            texBottomRight = texCoordControlPts[7];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[10], texCoordControlPts[7]);
            
            
            tileHeightPct = 1.0f/3.0f; //three tiles in book slot
            startPct = 0.0f + tileHeightPct * (i - 10);
            endPct = startPct + tileHeightPct;
            
        } else if(i == 13 || i <= 22){  //bottom tiles
            
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[10], controlPoints[7]);
            topRight = controlPoints[7];
            bottomRight = bookcaseCorners[2];
            bottomLeft = controlPoints[8];
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[10], texCoordControlPts[7]);
            texTopRight = texCoordControlPts[7];
            texBottomRight = texCoordCorners[2];
            texBottomLeft = texCoordControlPts[8];
            
            
            tileHeightPct = 1.0f/10.0f; //ten tiles in bottom part
            startPct = 0.0f + tileHeightPct * (i - 13);  //from the thirteenth tile down
            endPct = startPct + tileHeightPct;
            
        }
        
        
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
        
        
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
        
    }
    
    

    //----------Bookcase TOP MIDDLE Tiles----------
    //          ASSEMBLED LEFT TO RIGHT
    //two rows of 8 tiles between controlPoints 0 and 1 (left to right)
    
    //use width and height now since we're going across AND down
    startPct = 0.0f;
    endPct = 0.0f;
    tileWidthPct = 0.0f;
    
    
    //This region involves interpolation in two dimensions
    //so we'll store the bounds of the entire region first
    ofVec2f regionTopLeft = controlPoints[0];
    ofVec2f regionTopRight = controlPoints[1];
    ofVec2f regionBottomRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[15], controlPoints[2]);
    ofVec2f regionBottomLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[15], controlPoints[2]);
    
    ofVec2f regionTexTopLeft = texCoordControlPts[0];
    ofVec2f regionTexTopRight = texCoordControlPts[1];
    ofVec2f regionTexBottomRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[15], texCoordControlPts[2]);
    ofVec2f regionTexBottomLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[15], texCoordControlPts[2]);
    
    //these will hold the bounds of the individual row we're doing
    ofVec2f rowTopLeft;
    ofVec2f rowTopRight;
    ofVec2f rowBottomRight;
    ofVec2f rowBottomLeft;
    
    ofVec2f rowTexTopLeft;
    ofVec2f rowTexTopRight;
    ofVec2f rowTexBottomRight;
    ofVec2f rowTexBottomLeft;
    
    //Two outer for loops, one for each row
    for(int j = 0; j < 2; j++){
        
        tileHeightPct = 0.5f;
        tileWidthPct = 1.0f/8.0f;

            
        //the bounds of the individual row we're in will be
        //interpolated between the bounds of the entire region
        
        rowTopLeft = regionTopLeft.getInterpolated(regionBottomLeft, tileHeightPct * j);
        rowTopRight = regionTopRight.getInterpolated(regionBottomRight, tileHeightPct * j);
        rowBottomRight = regionTopRight.getInterpolated(regionBottomRight, tileHeightPct * (j + 1));
        rowBottomLeft = regionTopLeft.getInterpolated(regionBottomLeft, tileHeightPct * (j + 1));

        rowTexTopLeft = regionTexTopLeft.getInterpolated(regionTexBottomLeft, tileHeightPct * j);
        rowTexTopRight = regionTexTopRight.getInterpolated(regionTexBottomRight, tileHeightPct * j);
        rowTexBottomRight = regionTexTopRight.getInterpolated(regionTexBottomRight, tileHeightPct * (j + 1));
        rowTexBottomLeft = regionTexTopLeft.getInterpolated(regionTexBottomLeft, tileHeightPct * (j + 1));
        
        
        //make tiles across then move down to next row with j == 1
        for(int i = 0; i < 8; i++){
            
            endPct = startPct + tileWidthPct;
            
            verts[0] = rowTopLeft.getInterpolated(rowTopRight, startPct);
            verts[1] = rowTopLeft.getInterpolated(rowTopRight, endPct);
            verts[2] = rowBottomLeft.getInterpolated(rowBottomRight, endPct);
            verts[3] = rowBottomLeft.getInterpolated(rowBottomRight, startPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values from the image template instead of the control
            //points that will be moved with the GUI
            texCoords[0] = rowTexTopLeft.getInterpolated(rowTexTopRight, startPct);
            texCoords[1] = rowTexTopLeft.getInterpolated(rowTexTopRight, endPct);
            texCoords[2] = rowTexBottomLeft.getInterpolated(rowTexBottomRight, endPct);
            texCoords[3] = rowTexBottomLeft.getInterpolated(rowTexBottomRight, startPct);
            
            
            //start the next tile where this one ends
            startPct = endPct;
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
        }

        //start next row from the left again
        startPct = 0.0;
    }
    
    //----------Bookcase BOTTOM MIDDLE Tiles----------
    //          ASSEMBLED LEFT TO RIGHT
    //two rows of 8 tiles between controlPoints 0 and 1 (left to right)
    
    //use width and height now since we're going across AND down
    startPct = 0.0f;
    endPct = 0.0f;
    tileWidthPct = 0.0f;
    
    
    //This region involves interpolation in two dimensions
    //so we'll store the bounds of the entire region first
    regionTopLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[10], controlPoints[7]);
    regionTopRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[10], controlPoints[7]);
    regionBottomRight = controlPoints[8];
    regionBottomLeft = controlPoints[9];

    regionTexTopLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[10], texCoordControlPts[7]);
    regionTexTopRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[10], texCoordControlPts[7]);
    regionTexBottomRight = texCoordControlPts[8];
    regionTexBottomLeft = texCoordControlPts[9];
    
    //Two outer for loops, one for each row
    for(int j = 0; j < 10; j++){
        
        tileHeightPct = 1.0f/10.0f;
        tileWidthPct = 1.0f/8.0f;
        
        
        //the bounds of the individual row we're in will be
        //interpolated between the bounds of the entire region
        rowTopLeft = regionTopLeft.getInterpolated(regionBottomLeft, tileHeightPct * j);
        rowTopRight = regionTopRight.getInterpolated(regionBottomRight, tileHeightPct * j);
        rowBottomRight = regionTopRight.getInterpolated(regionBottomRight, tileHeightPct * (j + 1));
        rowBottomLeft = regionTopLeft.getInterpolated(regionBottomLeft, tileHeightPct * (j + 1));
        
        rowTexTopLeft = regionTexTopLeft.getInterpolated(regionTexBottomLeft, tileHeightPct * j);
        rowTexTopRight = regionTexTopRight.getInterpolated(regionTexBottomRight, tileHeightPct * j);
        rowTexBottomRight = regionTexTopRight.getInterpolated(regionTexBottomRight, tileHeightPct * (j + 1));
        rowTexBottomLeft = regionTexTopLeft.getInterpolated(regionTexBottomLeft, tileHeightPct * (j + 1));
        
        
        //make tiles across then move down to next row with j == 1
        for(int i = 0; i < 8; i++){
            
            endPct = startPct + tileWidthPct;
            
            verts[0] = rowTopLeft.getInterpolated(rowTopRight, startPct);
            verts[1] = rowTopLeft.getInterpolated(rowTopRight, endPct);
            verts[2] = rowBottomLeft.getInterpolated(rowBottomRight, endPct);
            verts[3] = rowBottomLeft.getInterpolated(rowBottomRight, startPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values from the image template instead of the control
            //points that will be moved with the GUI
            texCoords[0] = rowTexTopLeft.getInterpolated(rowTexTopRight, startPct);
            texCoords[1] = rowTexTopLeft.getInterpolated(rowTexTopRight, endPct);
            texCoords[2] = rowTexBottomLeft.getInterpolated(rowTexBottomRight, endPct);
            texCoords[3] = rowTexBottomLeft.getInterpolated(rowTexBottomRight, startPct);
            
            
            //start the next tile where this one ends
            startPct = endPct;
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
        }
        
        //start next row from the left again
        startPct = 0.0;
    }
    
    
    
    //----------FIRST AND SECOND SHELVES----------
    //Interpolates vertices betweem
    //frameCorners/controlPoints from left to right
    
    //Do TOP and BOTTOM with nested loop
    for(int j = 0; j < 2; j++){
        
        //do TOP shelf
        if(j == 0){
            
            topLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[14], controlPoints[3]);
            topRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[14], controlPoints[3]);
            bottomRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[13], controlPoints[4]);
            bottomLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[13], controlPoints[4]);

            texTopLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[14], texCoordControlPts[3]);
            texTopRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[14], texCoordControlPts[3]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[13], texCoordControlPts[4]);
            texBottomLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[13], texCoordControlPts[4]);

            
            
        } else {
            
            //do BOTTOM shelf
            
            topLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[12], controlPoints[5]);
            topRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[12], controlPoints[5]);
            bottomRight = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[11], controlPoints[6]);
            bottomLeft = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[11], controlPoints[6]);
            
            texTopLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[12], texCoordControlPts[5]);
            texTopRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[12], texCoordControlPts[5]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[8], texCoordControlPts[11], texCoordControlPts[6]);
            texBottomLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[9], texCoordControlPts[11], texCoordControlPts[6]);
            
        }
        
        tileWidthPct = 1.0f/8.0f;  //8 tiles in row
        
        //Make the 8 tiles per column, interpolating between the vertices declared above
        for(int i = 0; i < 8; i++){
            
            //startPct starts at zero and gets the new height (1/8 th the total) added every loop
            endPct = startPct + tileWidthPct;
            
            verts[0] = topLeft.getInterpolated(topRight, startPct);
            verts[1] = topLeft.getInterpolated(topRight, endPct);
            verts[2] = bottomLeft.getInterpolated(bottomRight, endPct);
            verts[3] = bottomLeft.getInterpolated(bottomRight, startPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values instead of the control points that will be moved with the GUI
            texCoords[0] = texTopLeft.getInterpolated(texTopRight, startPct);
            texCoords[1] = texTopLeft.getInterpolated(texTopRight, endPct);
            texCoords[2] = texBottomLeft.getInterpolated(texBottomRight, endPct);
            texCoords[3] = texBottomLeft.getInterpolated(texBottomRight, startPct);
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
            //add endPct to startPct so the next tile knows where to start
            startPct = endPct;
        }
        
        startPct = 0.0f;
    }
    
    
    //reset the shadow with the new corners
    shadow.clear();
    shadow.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    shadow.addVertex(bookcaseCorners[0]);
    shadow.addVertex(bookcaseCorners[1]);
    shadow.addVertex(bookcaseCorners[2]);
    shadow.addVertex(bookcaseCorners[3]);
    shadow.addColor(ofFloatColor(0));
    shadow.addColor(ofFloatColor(0));
    shadow.addColor(ofFloatColor(0));
    shadow.addColor(ofFloatColor(0));

    
    
    //get location of the shelf corners
    shelfCorners.clear();
    shelfCorners.resize(3);
    
    shelfCorners[0].resize(4);
    shelfCorners[0][0] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[14], controlPoints[3]);
    shelfCorners[0][1] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[14], controlPoints[3]);
    shelfCorners[0][2] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[15], controlPoints[2]);
    shelfCorners[0][3] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[15], controlPoints[2]);

    shelfCorners[1].resize(4);
    shelfCorners[1][0] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[12], controlPoints[5]);
    shelfCorners[1][1] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[12], controlPoints[5]);
    shelfCorners[1][2] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[13], controlPoints[4]);
    shelfCorners[1][3] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[13], controlPoints[4]);
    
    shelfCorners[2].resize(4);
    shelfCorners[2][0] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[10], controlPoints[7]);
    shelfCorners[2][1] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[10], controlPoints[7]);
    shelfCorners[2][2] = getIntersectionPoint(controlPoints[1], controlPoints[8], controlPoints[11], controlPoints[6]);
    shelfCorners[2][3] = getIntersectionPoint(controlPoints[0], controlPoints[9], controlPoints[11], controlPoints[6]);
    

    //setup the transparent overlay underneath the book UI
    
    shelfOverlays.clear();
    shelfOverlays.resize(3);
    //3 shelves
    for(int i = 0; i < shelfOverlays.size(); i++){

        shelfOverlays[i].clear();
        shelfOverlays[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        shelfOverlays[i].addVertex(shelfCorners[i][0]);
        shelfOverlays[i].addVertex(shelfCorners[i][1]);
        shelfOverlays[i].addVertex(shelfCorners[i][2]);
        shelfOverlays[i].addVertex(shelfCorners[i][3]);
                
    }
    
    
    
}



