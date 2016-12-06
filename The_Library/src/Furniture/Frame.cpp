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

void Frame::loadMedia(){
    
    //load all the images from file
    ofDirectory dir;
    dir.listDir("images/frames/");
    dir.sort();
    
    //load images with manual file names
    //loading with ofDirectory conflicts with ofxAssimp
    for(int i = 0; i < (int)dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        
        images.push_back(img);
        
    }
    
    currentImg = round(ofRandom( images.size() - 1 ));
    
    
    //prepare the portrait
    ofDirectory portraitDirectory;
    portraitDirectory.listDir("images/portraits/");
    portraitDirectory.sort();
    
    for(int i = 0; i < (int)portraitDirectory.size(); i++){
        
        ofImage img;
        img.load(portraitDirectory.getPath(i));
        
        portraits.push_back(img);
    }
    
    currentPortrait = round(ofRandom( portraits.size() - 1 ));
    
}

void Frame::setup(string name){
        
    
    //allocate FBO
    //This will be mapped to a mesh to fit inside the portrait
    //so size is less important than aspect ratio
    float aspect = 1.43;
    float w = 300;
    float h = w * aspect;
    portraitFbo.allocate(w, h, GL_RGBA);
    
    //clear buffer garbage
    portraitFbo.begin();
    ofClear(0, 0, 0, 255);
    portraitFbo.end();

    lastPortraitChange = 0;
    
    
    //ALL THE BUSINESS HAPPENS IN HERE
    //This will need to be called whenever the mesh is altered by GUI/mouse
    prepareMesh();
    
    guiName = name;
    
    setupGui();
    
    //Set GUI values to these (Only needed for first population of GUI XML)
//    frameCorner0 = frameCorners[0];
//    frameCorner1 = frameCorners[1];
//    frameCorner2 = frameCorners[2];
//    frameCorner3 = frameCorners[3];
//    controlPtPct0 = controlPointPcts[0];
//    controlPtPct1 = controlPointPcts[1];
//    controlPtPct2 = controlPointPcts[2];
//    controlPtPct3 = controlPointPcts[3];
//    controlPtPct4 = controlPointPcts[4];
//    controlPtPct5 = controlPointPcts[5];
//    controlPtPct6 = controlPointPcts[6];
//    controlPtPct7 = controlPointPcts[7];
    
    loadSettings();
    
    
    
    mapMesh();
    
    
    
    //Setup the parent class TiledObject with all th info set above
    TiledObject::setupTiledObject(false);   //NOT a bookcase

    lastMapTime = 0;
    
    
    
}


void Frame::update(){
    
    TiledObject::updateCommonGui();
    
    //this only updates variables from the GUI values if the GUI is on screen
    //Simple hack to avoid using listeners/callbacks for EACH of the GUI elements
    if(bIsGuiActive){
        
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
        
        
        if(reMapMeshButton && ofGetElapsedTimeMillis() - lastMapTime > 1000){
            tiles.clear();
            mapMesh();
            
            lastMapTime = ofGetElapsedTimeMillis();
        }
    
    }
    
    //this will be set to true later if we're drawing the gui.
    bIsGuiActive = false;

    //update all the stuff from the parent object
    //like tile manipulation, wave effects, etc.
    TiledObject::update();
    
    
    if(ofGetElapsedTimeMillis() - lastPortraitChange > 5000){
        currentPortrait = round(ofRandom( portraits.size() - 1 ));
        lastPortraitChange = ofGetElapsedTimeMillis();
    }
    
    
    
}

//shadow is the black background behind the bookcase that
//prevents the wallpaper from being visible on the physicall
//raised wooden frames
void Frame::drawShadow(){
    
    shadow.setColor(ofColor(0));
    shadow.draw();
    
}

void Frame::draw(){
    
    
    portraitFbo.begin();
    ofSetColor(255);
    

    portraits[currentPortrait].draw(0, 0, portraitFbo.getWidth(), portraitFbo.getHeight());
    
//    ofSetColor(0, 255, 0);
//    ofDrawRectangle(0, 0, portraitFbo.getWidth(), portraitFbo.getHeight());
    portraitFbo.end();
    
    ofTexture tex;
    tex = portraitFbo.getTexture();
    
    tex.bind();
    ofSetColor(255);
    portraitMesh.draw();
    tex.unbind();
    
    
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
    
    TiledObject::setupCommonGui();
    
    
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
    
    float start = 0.0f;
    float firstStep = 0.25f;
    float lastStep = 0.75f;
    float end = 1.0f;
    
    //will be overwritten immediately by calculations in "prepareMesh()" or XML loading
    float defaultVal = 0.1f;
    
    
    gui.add(controlPtPct0.setup("Control Pt 0", defaultVal, start, firstStep));
    gui.add(controlPtPct1.setup("Control Pt 1", defaultVal, lastStep, end));
    gui.add(controlPtPct2.setup("Control Pt 2", defaultVal, start, firstStep));
    gui.add(controlPtPct3.setup("Control Pt 3", defaultVal, lastStep, end));
    gui.add(controlPtPct4.setup("Control Pt 4", defaultVal, lastStep, end));
    gui.add(controlPtPct5.setup("Control Pt 5", defaultVal, start, firstStep));
    gui.add(controlPtPct6.setup("Control Pt 6", defaultVal, lastStep, end));
    gui.add(controlPtPct7.setup("Control Pt 7", defaultVal, start, firstStep));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    mappingLabel.setBackgroundColor(ofColor(255));
    
    
    gui.setPosition(ofGetWidth()/2 - 200 , 10);
    
    //if we're drawing the gui, then it's active so
    //we'll update all the values in update()
    bIsGuiActive = true;
    
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

void Frame::drawGui(){
    gui.draw();
    
    bIsGuiActive = true;
}

void Frame::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
    
    bIsGuiActive = true;
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
    
    //Get texCoords from the image template: 367 x 487
    frameWidth = 367;
    frameHeight = 487;
    
    //Thickness of frame border from template as a fraction of the width/height
    borderWidthPct = 0.1296;
    borderHeightPct = 0.1017;
    
    vertBorderTileHeightPct = (1.0f - (borderHeightPct * 2.0f))/8.0f;
    horizBorderTileWidthPct = (1.0f - (borderWidthPct * 2.0f))/5.0f;
    
    float heightFromTop = 130;
    
    //Frame corners hold the actual points that will be moved around
    //to map the mesh to the physical frame. Start them off as square
    //but they will be overwritten by loaded GUI settings
    frameCorners.resize(4);
    frameCorners[0] = ofVec2f( ofGetWidth()/2 - frameWidth/2, heightFromTop );
    frameCorners[1] = frameCorners[0] + ofVec2f( frameWidth, 0           );
    frameCorners[2] = frameCorners[0] + ofVec2f( frameWidth, frameHeight );
    frameCorners[3] = frameCorners[0] + ofVec2f( 0         , frameHeight );
    
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
    
    //these are the vertices we'll be using to interpolate the tiles between
    ofVec2f topLeft;
    ofVec2f topRight;
    ofVec2f bottomRight;
    ofVec2f bottomLeft;
    
    //these will hold the texture coordinates for the interpolated tiles
    ofVec2f texTopLeft;
    ofVec2f texTopRight;
    ofVec2f texBottomRight;
    ofVec2f texBottomLeft;
    
    //start/end points for the interpolations and how far to move tiles (width/height)
    float startPct;
    float endPct;
    float tileHeightPct;
    float tileWidthPct;
    
    //----------LEFT AND RIGHT COLUMNS----------
    //Interpolates vertices betweem
    //frameCorners/controlPoints from top to bottom

    
    //Do LEFT and RIGHT with nested loop
    for(int j = 0; j < 2; j++){
        
        //do LEFT
        if(j == 0){
            
            topLeft = controlPoints[7];
            topRight = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[7], controlPoints[2]);
            bottomRight = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[6], controlPoints[3]);
            bottomLeft = controlPoints[6];
            
            texTopLeft = texCoordControlPts[7];
            texTopRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[7], texCoordControlPts[2]);
            texBottomRight = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[6], texCoordControlPts[3]);
            texBottomLeft = texCoordControlPts[6];
            
            
            
            startPct = 0.0f;
            endPct = 0.0f;
            tileHeightPct = 1.0f/8.0f;
            
            
        } else {
            
            //do RIGHT column
            
            //vertices
            topLeft = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[7], controlPoints[2]);
            topRight = controlPoints[2];
            bottomRight = controlPoints[3];
            bottomLeft = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[6], controlPoints[3]);

            //texCoords
            texTopLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[7], texCoordControlPts[2]);
            texTopRight = texCoordControlPts[2];
            texBottomRight = texCoordControlPts[3];
            texBottomLeft = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[6], texCoordControlPts[3]);
            
            startPct = 0.0f;
            endPct = 0.0f;
            tileHeightPct = 1.0f/8.0f;

        }
        
        
        //Make the 8 tiles per column, interpolating between the vertices declared above
        for(int i = 0; i < 8; i++){
            
            //startPct starts at zero and gets the new height (1/8 th the total) added every loop
            endPct = startPct + tileHeightPct;
            
            verts[0] = topLeft.getInterpolated(bottomLeft, startPct);
            verts[1] = topRight.getInterpolated(bottomRight, startPct);
            verts[2] = topRight.getInterpolated(bottomRight, endPct);
            verts[3] = topLeft.getInterpolated(bottomLeft, endPct);
            
            //texCoords have similar construction but they use the default "borderWidthPct" and
            //"borderHeightPct" values instead of the control points that will be moved with the GUI
            texCoords[0] = texTopLeft.getInterpolated(texBottomLeft, startPct);
            texCoords[1] = texTopRight.getInterpolated(texBottomRight, startPct);
            texCoords[2] = texTopRight.getInterpolated(texBottomRight, endPct);
            texCoords[3] = texTopLeft.getInterpolated(texBottomLeft, endPct);
            
            Tile t;
            t.setup(verts, texCoords);
            t.setTextures( &images );
            t.setActiveTexture( currentImg );
            
            tiles.push_back(t);
            
            //add endPct to startPct so the next tile knows where to start
            startPct = endPct;
        }
    }
    
    
    //----------TOP AND BOTTOM ROWS----------
    //Interpolates vertices betweem
    //frameCorners/controlPoints from left to right
    
    //Do TOP and BOTTOM with nested loop
    for(int j = 0; j < 2; j++){
        
        //do TOP
        if(j == 0){
            
            topLeft = controlPoints[0];
            topRight = controlPoints[1];
            bottomRight = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[7], controlPoints[2]);
            bottomLeft = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[7], controlPoints[2]);
            
            texTopLeft = texCoordControlPts[0];
            texTopRight = texCoordControlPts[1];
            texBottomRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[7], texCoordControlPts[2]);
            texBottomLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[7], texCoordControlPts[2]);
            
            
            
            startPct = 0.0f;
            endPct = 0.0f;
            tileWidthPct = 1.0f/5.0f;  //5 tiles in top row
            
            
        } else {
            
            //do BOTTOM
            
            topLeft = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[6], controlPoints[3]);
            topRight = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[6], controlPoints[3]);
            bottomRight = controlPoints[4];
            bottomLeft = controlPoints[5];

            texTopLeft = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[6], texCoordControlPts[3]);
            texTopRight = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[6], texCoordControlPts[3]);
            texBottomRight = texCoordControlPts[4];
            texBottomLeft = texCoordControlPts[5];
            
            startPct = 0.0f;
            endPct = 0.0f;
            tileWidthPct = 1.0f/5.0f;  //5 tiles in top row
            
        }
        
        
        //Make the 8 tiles per column, interpolating between the vertices declared above
        for(int i = 0; i < 5; i++){
            
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
    }

    
    //Make 4 corner tiles
    for(int i = 0; i < 4; i++){
        
        if(i == 0){
            //----------TOP LEFT CORNER----------
            verts[0] = frameCorners[0];
            verts[1] = controlPoints[0];
            verts[2] = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[7], controlPoints[2]);
            verts[3] = controlPoints[7];
            
            texCoords[0] = texCoordCorners[0];
            texCoords[1] = texCoordControlPts[0];
            texCoords[2] = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[7], texCoordControlPts[2]);
            texCoords[3] = texCoordControlPts[7];
            
        } else if(i == 1){
            //----------TOP RIGHT CORNER----------
            verts[0] = controlPoints[1];
            verts[1] = frameCorners[1];
            verts[2] = controlPoints[2];
            verts[3] = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[7], controlPoints[2]);
            
            texCoords[0] = texCoordControlPts[1];
            texCoords[1] = texCoordCorners[1];
            texCoords[2] = texCoordControlPts[2];
            texCoords[3] = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[7], texCoordControlPts[2]);
            
        } else if(i == 2){
            //----------BOTTOM RIGHT CORNER----------
            verts[0] = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[6], controlPoints[3]);
            verts[1] = controlPoints[3];
            verts[2] = frameCorners[2];
            verts[3] = controlPoints[4];

            texCoords[0] = getIntersectionPoint(texCoordControlPts[1], texCoordControlPts[4], texCoordControlPts[6], texCoordControlPts[3]);
            texCoords[1] = texCoordControlPts[3];
            texCoords[2] = texCoordCorners[2];
            texCoords[3] = texCoordControlPts[4];
            
        } else {
            //----------BOTTOM LEFT CORNER----------
            verts[0] = controlPoints[6];
            verts[1] = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[6], controlPoints[3]);
            verts[2] = controlPoints[5];
            verts[3] = frameCorners[3];
            
            texCoords[0] = texCoordControlPts[6];
            texCoords[1] = getIntersectionPoint(texCoordControlPts[0], texCoordControlPts[5], texCoordControlPts[6], texCoordControlPts[3]);
            texCoords[2] = texCoordControlPts[5];
            texCoords[3] = texCoordCorners[3];
            
        }
        
        Tile t;
        t.setup(verts, texCoords);
        t.setTextures( &images );
        t.setActiveTexture( currentImg );
        
        tiles.push_back(t);
        
    }
    

    //reset the shadow with the new corners
    shadow.clear();
    shadow.moveTo(frameCorners[0]);
    shadow.lineTo(frameCorners[1]);
    shadow.lineTo(frameCorners[2]);
    shadow.lineTo(frameCorners[3]);
    shadow.lineTo(frameCorners[0]);
    shadow.close();
    
    
    
    //map Portrait mesh
    portraitVerts.clear();
    portraitVerts.resize(4);
    
    portraitVerts[0] = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[7], controlPoints[2]);
    portraitVerts[1] = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[7], controlPoints[2]);
    portraitVerts[2] = getIntersectionPoint(controlPoints[1], controlPoints[4], controlPoints[6], controlPoints[3]);
    portraitVerts[3] = getIntersectionPoint(controlPoints[0], controlPoints[5], controlPoints[6], controlPoints[3]);
    
    
    portraitMesh.clear();
    portraitMesh.addVertices(portraitVerts);
    
    //set them according to the FBO
    setPortraitTexCoords(portraitFbo.getWidth(), portraitFbo.getHeight());
    
    portraitMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    
    
}


void Frame::setPortraitTexCoords(int x, int y){
    
    portraitMesh.addTexCoord(ofVec2f(0, 0));
    portraitMesh.addTexCoord(ofVec2f(x, 0));
    portraitMesh.addTexCoord(ofVec2f(x, y));
    portraitMesh.addTexCoord(ofVec2f(0, y));
    
    
}





