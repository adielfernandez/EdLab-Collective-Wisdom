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
    
    cout << "Loading center book model" << endl;
    
    model.loadModel("books/bookMedium.fbx", false);
    
    
    
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
    b.load("images/centralBook/darkPages.png");
    bookTex = b.getTexture();
    
    bookTexFBO.allocate(bookTex.getWidth(), bookTex.getHeight());
    
    bookTexFBO.begin();
    ofClear(255, 255, 255, 255);
    bookTex.draw(0, 0);
    bookTexFBO.end();
    
    
    ofImage d;
    d.load("images/centralBook/deskTex.jpg");
    deskTex = d.getTexture();

    //size of the FBO. Just needs to be rough since
    //it will be stretched into position over a mesh.
    //make a multiple of 10 for mapMesh() to work correctly!
    deskWidth = 270;
    deskHeight = 180;

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
    

    
    mapMesh();
    
    
    bookPos.set(deskWidth/2, deskHeight - 10);
    
    //need to use a camera so we can enable
    //orthographic projection (disables perspective)
    camera.enableOrtho();
//    camera.disableMouseInput();
    camera.setTarget(ofVec3f(deskWidth/2, deskHeight/2, 0));
    camera.setOrientation(ofVec3f(0, 0, 180));
    camera.setPosition(deskWidth/2, deskHeight/2, -1000);
    
    
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
    
    pageText.clear();
    pageText.resize(4);
    
    
    //Pages can roughly fit 15-16 characters per line and 8 lines per page
    string p1 = "";
    
    p1 += "John Dewey\n";
    p1 += "Jean Piaget\n";
    p1 += "Maria Montessori\n";
    p1 += "Lev Vygotsky\n";
    p1 += "Benjamin Bloom\n";
    p1 += "Howard Gardner\n";
    p1 += "Jerome Bruner\n";
    p1 += "Albert Bandura\n";
    
    pageText[0] = p1;
    pageText[1] = p1;
    pageText[2] = p1;
    pageText[3] = p1;
    
    
    font.load("fonts/Arcon-Rounded-Regular.otf", 12);
    
    animationPos = animFirstPages;
    
    
    
    //GUI SETUP
    
    filePath = "settings/";
    guiName = "centerBook";
    setupGui();

    
    //not needed after first XML population
    //saveSettings();
    
    loadSettings();
}

void CenterBook::mapMesh(){

    //create a finely spaced mesh from the FBO.
    //Then set texture coordinates and do a bilinear interpolation
    //of all the grid points between the four corners
    
    int gridSpacing = 10;
    int numCols = (deskFBO.getWidth() / gridSpacing);
    int numRows = (deskFBO.getHeight() / gridSpacing);
    
    
    
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



void CenterBook::drawContentToTexture(){
    
    //-----DRAW TEXT ONTO TEXTURE-----
    
    bookTexFBO.begin();
    
    //clear the FBO from the last frame
    ofClear(255, 255, 255, 255);
    
    //draw the base texture first
    ofSetColor(255);
    bookTex.draw(0, 0);
    
    ofDisableDepthTest();
    //go through all the pages and draw out the text
    for(int i = 0; i < pageText.size(); i++){
        
        ofPushMatrix();
        
        //left pages draw closer to page edge
        //right pages draw further to get past page curvature
        float leftMargin = (i % 2 == 1 ? 10 : 5);
        float topMargin = 18.5;
        
        ofTranslate(pageTexCoords[i].x + leftMargin, pageTexCoords[i].y + topMargin);
        ofScale( 1.0/widthScale , 1.0);
        ofSetColor(93, 50, 0);
        //                    float lineHeight = font -> stringHeight("A");
        font.drawString(pageText[i], 0, 0); //-lineHeight);
        
        ofPopMatrix();
        
    }
    ofEnableDepthTest();
    
    bookTexFBO.end();
    
}


void CenterBook::update(){
    
    
    //Model manipulation
    
    //uncomment to scroll through model animation with mouse
//    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1.0);
//    model.setPositionForAllAnimations(x);

    
    //loop through pages automatically
    if(ofGetElapsedTimeMillis() - lastPageFlip > 5000){
        
        animationPos += 0.001;
        
        if(animationPos > animSecondPages){
            animationPos = animFirstPages;
            lastPageFlip = ofGetElapsedTimeMillis();
        }
    }
    
    
    
    
    //draws text to book
    drawContentToTexture();
    
    
    
    model.setPositionForAllAnimations(animationPos);
    model.update();
    
    
    
    //update values with GUI
    if(reMapMeshButton){
        mapMesh();
    }

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

    

    
    

    
    
    
    
}







void CenterBook::draw(){
    

    //draw to desk texture
    deskFBO.begin();{
        
        camera.begin();
       
        ofClear(0, 0, 0, 255);
        
        
        deskTex.draw(0, 0, deskFBO.getWidth(), deskFBO.getHeight());
        
        ofPushMatrix();
        
        //translate the book to the position on the shelf
        //with the bottom left corner of the spine at pos
        ofTranslate(bookPos.x, bookPos.y, 1);
        
        //        float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), -90, 90);
        //        float z = ofMap(ofGetMouseY(), 0, ofGetHeight(), -90, 90);
        //        cout << "XZ: " << x << ", " << z << endl;
        
        
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
        if(bookTexFBO.isAllocated()) bookTexFBO.getTexture().bind();
        
        ofSetColor(255);
        model.drawFaces();
        
        if(bookTexFBO.isAllocated()) bookTexFBO.getTexture().unbind();
        

        
        
        ofPopMatrix();
        
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
    
    
    
    
    
    ofPopStyle();
    
    
    
    
}













void CenterBook::setupGui(){
    

    
    
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    gui.add(positioningLabel.setup("  POSITIONING", ""));
    
    float maxRangeX = 50;
    float maxRangeY = 50;
    ofVec2f plus(maxRangeX, maxRangeY);
    ofVec2f minus(-maxRangeX, -maxRangeY);
    
    gui.add(bookPosSlider.setup("Book Pos", bookPos, bookPos + minus, bookPos + plus));
    
//    cout << "Vec2 Val: " << bookPos << endl;
//    cout << "GUI Val: "  << bookPosSlider << endl;
    gui.add(bulkScaleSlider.setup("Bulk Scale", 1.0f, 0.8f, 1.5f));
    gui.add(depthScaleSlider.setup("Depth Scale", 1.0f, 0.8f, 1.5f));
    gui.add(heightScaleSlider.setup("Height Scale", 1.0f, 0.8f, 1.5f));
    gui.add(widthScaleSlider.setup("Width Scale", 1.0, 0.8, 1.5f));
    
    gui.add(mappingLabel.setup("  MAPPING", ""));

    gui.add(reMapMeshButton.setup("Re-Map Mesh"));
    gui.add(showUnwarpedToggle.setup("Show UnWarped", true));
    gui.add(drawWireframeToggle.setup("Draw Wireframe", true));
    
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

void CenterBook::drawGui(){
    gui.draw();
}

void CenterBook::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}

