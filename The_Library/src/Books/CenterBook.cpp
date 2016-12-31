//
//  CenterBook.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 12/11/16.
//
//

#include "CenterBook.hpp"


CenterBook::CenterBook(){
    
    
}

void CenterBook::loadModel(){
    
//    cout << "Loading center book model" << endl;
    
    model.loadModel("assets/centralBook/bookMedium.fbx", false);
    
    
    
}


void CenterBook::setup(){
    
    //Since model Scale and dimensions are mostly hardcoded to give good, pixel related values,
    //"modelScale" scales all of them equally to make minor adjustments

    //set later by GUI
    bulkScale = 1.0;
    
    modelScale = baseScale * bulkScale;
    
    model.setScaleNormalization(false);
    model.setScale(1.0, 1.0, 1.0);
    model.setLoopStateForAllAnimations(OF_LOOP_NONE);
    model.setPausedForAllAnimations(true);
    
    //Real pixel dimensions determined visually at the scale above (819.219)
    //Short book
    //x = 24.2906, y= 111.599
    //Medium book
    //x = 24.2344, y= 124.109 (aspect: 5.121191364)
    //Tall book
    //x = 18.401, y = 136.316
    height = 124.10;
    thickness = 24.2906;

    
    height *= bulkScale;
    thickness *= bulkScale;
    
    
    //depth is almost arbitrary since camera ortho is disabled
    //just pick a usable number so book pulls out of shelf properly
    depth = 100;
    
    
    //model positioned at bottom left corner of spine
    //values acquired visually since AssImp model does not give reliable size/scaling
    //-----short book-----
//    model.setPosition(-0.0477083, -0.0147917, -0.0682016);
    
    //-----medium book-----
    model.setPosition(-0.0477083, -0.0147917, -0.0757602);
    
    //-----tall book-----
//    model.setPosition(-0.0477083, -0.0147917, -0.0833189);

    
    
    //load Book and desk textures
    ofImage b;
    b.load("assets/centralBook/baseTex.png");
    bookTex = b.getTexture();
    
    bookTexFBO.allocate(bookTex.getWidth(), bookTex.getHeight());
    
    bookTexFBO.begin();
    ofClear(255, 255, 255, 255);
    bookTex.draw(0, 0);
    bookTexFBO.end();
    
    
    ofImage d;
    d.load("assets/centralBook/deskTex.jpg");
    deskTex = d.getTexture();

    //size of the FBO. Just needs to be rough since
    //it will be stretched into position over a mesh.
    deskFBO.allocate(deskWidth, deskHeight);
    
    deskFBO.begin();
    ofClear(255, 255, 255, 255);
    deskTex.draw(0, 0, deskWidth, deskHeight);
    deskFBO.end();
    
    //desk Mesh setup
    float deskHeightFromTop = 700;  //just a starting point, adjusted by gui later
    
    meshPoints.resize(4);
    meshPoints[0] = ofVec2f( ofGetWidth()/2 - deskWidth/2 , deskHeightFromTop );
    meshPoints[1] = ofVec2f( ofGetWidth()/2 + deskWidth/2 , deskHeightFromTop );
    meshPoints[2] = ofVec2f( ofGetWidth()/2 + deskWidth/2 , deskHeightFromTop + deskHeight );
    meshPoints[3] = ofVec2f( ofGetWidth()/2 - deskWidth/2 , deskHeightFromTop + deskHeight );
    

    filigreeBorder.load("assets/interface/filigree/frame.png");

    
    borderMargin = 10;

    //scale it down so it fits in the space just right
    borderHeight = (deskHeight - borderMargin * 2);
    borderWidth = filigreeBorder.getWidth() * borderHeight/(float)filigreeBorder.getHeight();

    //image X is inverted inside FBO so image anchors from the apparent top right corner
    borderPos.set(deskWidth - borderMargin - borderWidth, borderMargin);
    
    //set help text
    helpText.resize(2);
    
    helpText[0].resize(5);
    helpText[0][0] = "Choose";
    helpText[0][1] = "a scholar to";
    helpText[0][2] = "learn more";
    helpText[0][3] = "about their";
    helpText[0][4] = "research";
    
    helpText[1].resize(5);
    helpText[1][0] = "View scholar";
    helpText[1][1] = "information";
    helpText[1][2] = "or highlight";
    helpText[1][3] = "relevant";
    helpText[1][4] = "works";

    currentHelpText = 0;

    
    
    
    bookPos.set(deskWidth/2, deskHeight - 10);
    
    //need to use a camera so we can enable
    //orthographic projection (disables perspective)
    camera.enableOrtho();
//    camera.disableMouseInput();
    camera.setTarget(ofVec3f(deskWidth/2, deskHeight/2, 0));
    camera.setOrientation(ofVec3f(0, 0, 180));
    camera.setPosition(deskWidth/2, deskHeight/2, 1000);
    
    
    //Book content and UI stuff
    
    //positions within the book texture (512x512) for all the pages
    pageTexCoords.resize(6);
    
    pageTexCoords[0].set( 261.299,  21.838);
    pageTexCoords[1].set( 385.824,  21.838);
    pageTexCoords[2].set(  14.281, 216.057);
    pageTexCoords[3].set( 140.605, 215.922);
    pageTexCoords[4].set( 263.491, 215.922);
    pageTexCoords[5].set( 386.88 , 215.419);
    
    //width and height of each page from the texture
    pageWidth = 114.16;
    pageHeight = 186.273;

    //Dimensions and texcoords above are based on a 512x512 texture
    //scale up by 2 for a 1024x1024
    pageTexCoords[0] *= 2.0;
    pageTexCoords[1] *= 2.0;
    pageTexCoords[2] *= 2.0;
    pageTexCoords[3] *= 2.0;
    pageTexCoords[4] *= 2.0;
    pageTexCoords[5] *= 2.0;
    
    pageWidth *= 2.0;
    pageHeight *= 2.0;
    

    scholarList.resize(10);
    scholarList[0].nameTop = "John";
    scholarList[0].nameBottom = "Dewey";
    scholarList[0].id = 0;
    scholarList[0].selected = false;
    
    scholarList[1].nameTop = "Jean";
    scholarList[1].nameBottom = "Piaget";
    scholarList[1].id = 1;
    scholarList[1].selected = false;

    scholarList[2].nameTop = "Maria";
    scholarList[2].nameBottom = "Montessori";
    scholarList[2].id = 2;
    scholarList[2].selected = false;
    
    scholarList[3].nameTop = "Lev";
    scholarList[3].nameBottom = "Vygotsky";
    scholarList[3].id = 3;
    scholarList[3].selected = false;
    
    scholarList[4].nameTop = "Gloria";
    scholarList[4].nameBottom = "Ladson";
    scholarList[4].id = 4;
    scholarList[4].selected = false;
    
    scholarList[5].nameTop = "Benjamin";
    scholarList[5].nameBottom = "Bloom";
    scholarList[5].id = 5;
    scholarList[5].selected = false;
    
    scholarList[6].nameTop = "Howard";
    scholarList[6].nameBottom = "Gardner";
    scholarList[6].id = 6;
    scholarList[6].selected = false;
    
    scholarList[7].nameTop = "Jerome";
    scholarList[7].nameBottom = "Bruner";
    scholarList[7].id = 7;
    scholarList[7].selected = false;
    
    scholarList[8].nameTop = "Albert";
    scholarList[8].nameBottom = "Bandura";
    scholarList[8].id = 8;
    scholarList[8].selected = false;
    
    scholarList[9].nameTop = "Lisa";
    scholarList[9].nameBottom = "Delpit";
    scholarList[9].id = 9;
    scholarList[9].selected = false;
    

    
    
//    pageText[0] = p1;
//    pageText[1] = p2;
//    pageText[2] = p1;
//    pageText[3] = p2;
    
    
    font.load("bookFonts/Arcon-Rounded-Regular.otf", 11);
    boldFont.load("assets/interface/Century-gothic-bold.ttf", 14);
    boldFont.setLetterSpacing(0.95);


    
    //GUI SETUP
    
    filePath = "settings/";
    guiName = "centerBook";
    setupGui();

    
    //not needed after first XML population
    //saveSettings();
    
    loadSettings();
    setVariablesFromGui();
    
    mapMesh();
    
    drawContentToTexture();
    
    //The position the raw desk will draw when debugging
    rawDeskPos.set( ofGetWidth()/2.0 - deskWidth/2, ofGetHeight()/2 - deskHeight + 50 );
    
    
    
    //Layout of the central book content
    //first page spread: list of scholars
    topMargin = page1LeftMargin;
    betweenScholars = 15;

    
    nameBoxHeight = (pageHeight - topMargin * 2 - betweenScholars * 4)/5.0f;
    cout << "box height: " << nameBoxHeight << endl;
    
    float spaceForName = nameBoxHeight;
    betweenNames = 4;

    int nameFontSize = (spaceForName - betweenNames)/2.0f;
    
    cout << "Font size: " << nameFontSize << endl;
    
    scholarFont.load("assets/interface/EBGaramond08-Regular.ttf", nameFontSize);
    scholarFont.setLetterSpacing(1.05);
    
    lineHeight = scholarFont.stringHeight("A");
    
    divider.load("assets/interface/filigree/thinDivider.png");
    divider.setAnchorPercent(0.5f, 0.5f);

}

void CenterBook::mapMesh(){

    //create a finely spaced mesh from the FBO.
    //Then set texture coordinates and do a bilinear interpolation
    //of all the grid points between the four corners
    
    int gridSpacing = 25;
    int numCols = (deskWidth / gridSpacing);
    int numRows = (deskHeight / gridSpacing);
    
    
    
    deskMesh.clear();
    
    //add a vertex at every grid point
    // "<=" means 4 columns is 5 points horizontally
    
//    int i = 0;
    for(int y = 0; y <= numRows; y ++){
        
        for(int x = 0; x <= numCols; x ++){
            
            //EASY: add the point in as the tex coord
            deskMesh.addTexCoord( ofVec2f( x * gridSpacing, y * gridSpacing ) );
            
            //HARDER: Make the vertex a bilinear interpolation of the 4 corners
            //start by finding the start and end of each row which is an interpolation
            //from the top corners down to the bottom on left and right sides
            float thisRowPct = (float)y/(float)numRows;
            ofVec2f rowStart( meshPoints[0].getInterpolated(meshPoints[3], thisRowPct ) );
            ofVec2f rowEnd( meshPoints[1].getInterpolated(meshPoints[2], thisRowPct ) );
            
            //this point is now an interpolation from row start to row end

            float thisColPct = (float)x/(float)numCols;
            ofVec3f thisVert = rowStart.getInterpolated(rowEnd, thisColPct);
            thisVert.z += -1;
            deskMesh.addVertex( thisVert );
            
//            cout << "Vert[" << i << "]: " << thisVert << endl;
//            cout << "TexCoord[" << i << "]: " << deskMesh.getTexCoord(i) << endl;
//            i++;
        }
        
    }
    
    //now go through all the rows and columns and add the indices to build the plane
    
    /*
         ____
        |   /|
        |  / |
        | /  |
        |/___|

     */
    
    deskMesh.clearIndices();
    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numCols; x++){
            
            //First triangle
            deskMesh.addIndex( x +       y        * (numCols+1) );
            deskMesh.addIndex((x + 1) +  y        * (numCols+1) );
            deskMesh.addIndex( x +      ( y + 1 ) * (numCols+1) );
            
            //Second triangle
            deskMesh.addIndex((x + 1) +   y       * (numCols+1) );
            deskMesh.addIndex( x      + ( y + 1 ) * (numCols+1) );
            deskMesh.addIndex((x + 1) + ( y + 1 ) * (numCols+1) );
            
        }
    }

    
    deskMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
}


void CenterBook::resetCamera(){
    camera.setTarget(ofVec3f(deskWidth/2, deskHeight/2, 0));
    camera.setOrientation(ofVec3f(0, 0, 180));
    camera.setPosition(deskWidth/2, deskHeight/2, 1000);
}

void CenterBook::update(){
    
    
    //Model manipulation
    
    //uncomment to scroll through model animation with mouse
    //    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1.0);
    //    model.setPositionForAllAnimations(x);
    
    
    
    //draws text to book
    drawContentToTexture();
    
    
    
    animationPos = animationSpread1;
    model.setPositionForAllAnimations(animationPos);
    
    model.update();
    
    
    //update variables with values from GUI
    if(reMapMeshButton){
        mapMesh();
    }
    
    setVariablesFromGui();
    
    
    
    
    //if we're showing the raw desk, see if the mouse is inside of it and
    //pass normalized mouse touches into the FBO
    
    //this is practice for when touches actually come in through OSC
    //Touches will be normalized from 0-1 over the width/height of the entire desk
    
    mouseTouches.clear();
    
    if(showRawDeskToggle){
        
        ofVec2f m(ofGetMouseX(), ofGetMouseY());
        
        if(m.x > rawDeskPos.x && m.x < rawDeskPos.x + deskWidth && m.y > rawDeskPos.y && m.y < rawDeskPos.y + deskHeight){
            
            ofVec2f normalizedMouse((m.x - rawDeskPos.x)/deskWidth, (m.y - rawDeskPos.y)/deskHeight);
            
            //flip the X, since the X direction in the FBO is inverted due to the camera
            normalizedMouse.x = 1.0f - normalizedMouse.x;
            
            MouseTouch mt;
            mt.pos = normalizedMouse;
            mt.bIsTouching = ofGetMousePressed();
            
            mouseTouches.push_back(mt);
            
        }
        
        
    }
    
    
    //go through all the scholars and turn them off so
    //we can turn them on with mouse hover
    for(int i = 0; i < scholarList.size(); i++){
        scholarList[i].selected = false;
    }
    
    
    //go through mouse touches and check for detection zones
    for(int i = 0; i < mouseTouches.size(); i++){
        
        ofVec2f m = mouseTouches[i].pos;
        cout << m << endl;
        
        //touch data is normalized from 0-1 in x and y
        //for now, x is inverted (0 at right) and y is normal
        
        //see if we're over book
        if(m.x < 0.6f){
            
            int scholarIndex;
            
            //see which page we're on
            if(m.x > 0.31){
                //left page
                
                //the height divided into 5 regions means each region is 0.2
                scholarIndex = floor(m.y / 0.2);
                
            } else {
                //right page
                scholarIndex = floor(m.y / 0.2) + 5;
                
            }
            
            scholarList[scholarIndex].selected = true;
            
            
        }
        
        
        
    }
    
    
    
    
    
}






void CenterBook::drawContentToTexture(){
    
    //-----DRAW TEXT ONTO TEXTURE-----
    
    bookTexFBO.begin();
    
    //clear the FBO from the last frame
    ofClear(255, 255, 255, 255);
    
    //draw the base texture first
    ofSetColor(255);
    bookTex.draw(0, 0);
    
    float padding = 5;
    

    
    
    //go through all the pages and draw out the text
    for(int i = 0; i < scholarList.size(); i++){

        //select things with mouse just for testing
//        int sliceWidth = ofGetWidth()/scholarList.size();
//        float mx = ofGetMouseX();
//        
//        if(mx > i * sliceWidth && mx < (i + 1) * sliceWidth){
//            scholarList[i].selected = true;
//        } else {
//            scholarList[i].selected = false;
//        }
        
        ofPushMatrix();
        
        float boxX, x1, x2, y, w;
        
        //draw left page names
        if(i < 5){

            ofTranslate(pageTexCoords[0]);

            boxX = page1LeftMargin;
            y = topMargin + (nameBoxHeight + betweenScholars) * i;
            w = pageWidth - page1LeftMargin * 2;
            
            //stagger 2nd and 4th names to right align
            if(i % 2 == 0){
                x1 = page1LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 3 - scholarFont.stringWidth(scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 3 - scholarFont.stringWidth(scholarList[i].nameBottom);
            }
            
        } else {
            
            //draw right page names
            ofTranslate(pageTexCoords[1]);
            
            boxX = page2LeftMargin;
            y = topMargin + (nameBoxHeight + betweenScholars) * (i - 5);
            w = pageWidth - page2LeftMargin - page1LeftMargin;
            
            //staggered names are odd on this page
            if(i % 2 == 1){
                x1 = page2LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(scholarList[i].nameBottom);
            }
            
        }

        //draw the filigree divider above all names except the 0th and 5th names
        if(i != 0 && i != 5){
            
            ofSetColor(255);
            divider.draw(pageWidth/2, y - betweenScholars/2, divider.getWidth(), divider.getHeight() * 2 );
            
        }
        
        
        //draw highlight box
        if(scholarList[i].selected){
            ofSetColor(0, 128, 255);
            ofDrawRectangle(boxX, y, w, nameBoxHeight);
            
            ofSetColor(255);
        } else {
            ofSetColor(92, 54, 9);
        }
        
        scholarFont.drawString(scholarList[i].nameTop, x1 + padding, y + padding + lineHeight);
        scholarFont.drawString(scholarList[i].nameBottom, x2 + padding, y + padding + lineHeight * 2 + betweenNames);

        
        ofPopMatrix();
    }

    
    
    
    
    
    bookTexFBO.end();
    
}



void CenterBook::draw(){
    

    //draw to desk texture
    deskFBO.begin();{
        
        camera.begin();
       
        ofClear(0, 0, 0, 255);
        
        
        deskTex.draw(0, 0, deskWidth, deskHeight);
        
        ofPushMatrix();
        
        //translate the book to the position on the shelf
        //with the bottom left corner of the spine at pos
        ofTranslate(bookPos.x, bookPos.y, 1);
        
        //now rotate and translate book so that it has the lower left corner of the spine
        //as its origin and is oriented properly: book stored in shelf
        ofRotateX(displayRotX);
        ofRotateZ(displayRotZ + 180);
        
        //scale the model up (with a little extra in the X to add more viewable reading area.
        //Also, squash the book down to minimize the curvature
        //making text close to the spine easier to read
        ofScale(modelScale * widthScale, modelScale * depthScale, modelScale * heightScale);
        
        //disable the model textures so we can use our own
        model.disableTextures();
        ofEnableDepthTest();
        if(bookTexFBO.isAllocated()) bookTexFBO.getTexture().bind();
        
        ofSetColor(255);
        model.drawFaces();
        
        if(bookTexFBO.isAllocated()) bookTexFBO.getTexture().unbind();
        ofDisableDepthTest();

        ofPopMatrix();
        
        
        //draw the filigree border
        ofSetColor(255);

        
        filigreeBorder.draw(borderPos, borderWidth, borderHeight);

        //draw help text
        float betLines = 3;
        float lineHeight = boldFont.stringWidth("A");

        float startHeight = (deskHeight - lineHeight * 4 - betLines * 4)/2.0f + lineHeight/2.0f; //add an extra line to the end since text draws from the baseline
        
        ofPushMatrix();
        

        
        //invert the text to cancel out the text texture flipping
        ofScale(-1, -1);
        for(int i = 0; i < helpText[currentHelpText].size(); i++){
            
            ofSetColor(255);
            string s = helpText[currentHelpText][i];
            ofVec2f thisLine(borderPos.x + borderWidth/2 + boldFont.stringWidth(s)/2, startHeight + (lineHeight + betLines) * i);

            //invert the position to cancel out the scaling effect on position
            thisLine *= -1;
            boldFont.drawString(s, thisLine.x, thisLine.y);
            
        }
        ofPopMatrix();
        
        
        //go through touches (for now mouseTouches, but eventually OSC data from camera)
        //draw cursors, do button region detection, etc.
        for(int i = 0; i < mouseTouches.size(); i++){
            
            if(mouseTouches[i].bIsTouching){
                ofSetColor(255, 0, 0);
            } else {
                ofSetColor(0);
            }
        
            
            ofDrawCircle(mouseTouches[i].pos.x * deskWidth, mouseTouches[i].pos.y * deskHeight, -50, 5);
            
        }
        
        
        
        camera.end();
        
    }deskFBO.end();
    

    //draw the actual texture to screen
    deskFBO.getTexture().bind();
    ofSetColor(255);
    deskMesh.draw();
    deskFBO.getTexture().unbind();

    
    

    
}

void CenterBook::drawDebug(){
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    
    
    
    if(drawWireframeToggle){
        ofSetColor(255, 100);
        deskMesh.drawWireframe();
        
    }
    
    
    if(drawBookTexToggle){
        ofSetColor(255);
        float scale = 0.5;
        
        bookTexFBO.draw(10, 10 + bookTexFBO.getHeight() * scale, bookTexFBO.getWidth() * scale, -bookTexFBO.getHeight() * scale);
    }
    
    //draw corners
    ofPolyline corners;
    ofSetColor(0, 255, 0);
    
    
    
    for(int i = 0; i < meshPoints.size(); i++){
        
        ofDrawCircle(meshPoints[i], 10);
        ofDrawBitmapStringHighlight(ofToString(i), meshPoints[i].x + 10, meshPoints[i].y - 10);
        
        corners.addVertex(meshPoints[i]);
    }
    
    corners.close();
    corners.draw();
    
    if(showRawDeskToggle){

        //invert the Y since the camera flips the way textures are drawn
        ofSetColor(255);
        deskFBO.draw(rawDeskPos.x, rawDeskPos.y + deskFBO.getHeight(), deskWidth, -deskHeight );
        
    }
    
    
    
    ofPopStyle();
    
    
    
    
}













void CenterBook::setupGui(){
    

    
    
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    gui.add(positioningLabel.setup("  POSITIONING", ""));
    
    float maxRangeX = 100;
    float maxRangeY = 100;
    ofVec2f plus(maxRangeX, maxRangeY);
    ofVec2f minus(-maxRangeX, -maxRangeY);
    
    gui.add(bookPosSlider.setup("Book Pos", bookPos, bookPos + minus, bookPos + plus));
    
//    cout << "Vec2 Val: " << bookPos << endl;
//    cout << "GUI Val: "  << bookPosSlider << endl;
    gui.add(bulkScaleSlider.setup("Bulk Scale", 1.0f, 0.8f, 2.5f));
    gui.add(depthScaleSlider.setup("Depth Scale", 1.0f, 0.2f, 1.5f));
    gui.add(heightScaleSlider.setup("Height Scale", 1.0f, 0.2f, 1.5f));
    gui.add(widthScaleSlider.setup("Width Scale", 1.0f, 0.2f, 1.5f));
    
    gui.add(mappingLabel.setup("  MAPPING", ""));

    gui.add(showRawDeskToggle.setup("Show Raw Desk", false));
    gui.add(reMapMeshButton.setup("Re-Map Mesh"));
    gui.add(drawWireframeToggle.setup("Draw Wireframe", true));

    gui.add(contentTextureLabel.setup("  CONTENT and TEXTURING", ""));
    gui.add(page1LeftMarginSlider.setup("Page1 Left Margin", 10, -5, 40));
    gui.add(page2LeftMarginSlider.setup("Page2 Left Margin", 20, -5, 40));
    gui.add(pageTopMarginSlider.setup("Page Top Margin", 38, 0, 60));
    gui.add(drawBookTexToggle.setup("Draw Raw Book Tex", false));
    
    
    
    maxRangeX = 100;
    maxRangeY = 250;
    plus.set(maxRangeX, maxRangeY);
    minus.set(-maxRangeX, -maxRangeY);
    
    gui.add(meshPt0.setup("Mesh Pt 0", meshPoints[0], meshPoints[0] + minus, meshPoints[0] + plus));
    gui.add(meshPt1.setup("Mesh Pt 1", meshPoints[1], meshPoints[1] + minus, meshPoints[1] + plus));
    gui.add(meshPt2.setup("Mesh Pt 2", meshPoints[2], meshPoints[2] + minus, meshPoints[2] + plus));
    gui.add(meshPt3.setup("Mesh Pt 3", meshPoints[3], meshPoints[3] + minus, meshPoints[3] + plus));


    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    positioningLabel.setBackgroundColor(ofColor(255));
    mappingLabel.setBackgroundColor(ofColor(255));
    contentTextureLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    positioningLabel.setDefaultTextColor(ofColor(0));
    
    gui.setPosition(meshPoints[0].x - 300, meshPoints[0].y - 120);
    
}

void CenterBook::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    
}

void CenterBook::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}

void CenterBook::setVariablesFromGui(){
 
    bookPos.x = bookPosSlider -> x;
    bookPos.y = bookPosSlider -> y;
    
    widthScale = widthScaleSlider;
    heightScale = heightScaleSlider;
    depthScale = depthScaleSlider;
    bulkScale = bulkScaleSlider;
    
    modelScale = baseScale * bulkScale;
    
    meshPoints.clear();
    meshPoints.resize(4);
    meshPoints[0] = ofVec2f(meshPt0 -> x, meshPt0 -> y);
    meshPoints[1] = ofVec2f(meshPt1 -> x, meshPt1 -> y);
    meshPoints[2] = ofVec2f(meshPt2 -> x, meshPt2 -> y);
    meshPoints[3] = ofVec2f(meshPt3 -> x, meshPt3 -> y);
    
    page1LeftMargin = page1LeftMarginSlider;
    page2LeftMargin = page2LeftMarginSlider;
    pageTopMargin = pageTopMarginSlider;
}


void CenterBook::drawGui(){
    gui.draw();
}

void CenterBook::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}

