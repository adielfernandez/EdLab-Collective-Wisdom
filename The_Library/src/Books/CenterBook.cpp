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


void CenterBook::setup(ScholarData *sData, Frame *f){
    
    scholarData = sData;
    frame = f;
    
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
    

    
    //GUI SETUP
    
    filePath = "settings/";
    guiName = "centerBook";
    setupGui();

    
    //not needed after first XML population
    //saveSettings();
    
    loadSettings();
    setVariablesFromGui();
    
    mapMesh();
    

    
    //The position the raw desk will draw when debugging
    rawDeskPos.set( ofGetWidth()/2.0 - deskWidth/2, ofGetHeight()/2 - deskHeight + 50 );
    
    
    
    //Layout of the central book content
    //first page spread: list of scholars
    topMargin = page1LeftMargin;
    betweenScholars = 15;

    
    highlightBoxHeight = (pageHeight - topMargin * 2 - betweenScholars * 4)/5.0f;

    
    float spaceForName = highlightBoxHeight;
    betweenNames = 4;

    int nameFontSize = (spaceForName - betweenNames)/2.0f;
        
    //scholar font
    scholarFont.load("assets/interface/EBGaramond08-Regular.ttf", nameFontSize);
    scholarFont.setLetterSpacing(.95);
    
    lineHeight = scholarFont.stringHeight("A");
    
    //smaller serif font in book
    smallBookFont.load("assets/interface/EBGaramond08-Regular.ttf", 12);
    
    
    //bold interface text
    boldUIFont.load("assets/interface/Century-gothic-bold.ttf", 14);
    boldUIFont.setLetterSpacing(0.95);
    
    tagButtonFont.load("assets/interface/Century-gothic-bold.ttf", 26);
    tagButtonFont.setLetterSpacing(0.9);
    
    //brownish color for text
    textColor.set(92, 54, 9);
    highlightColor.set(0, 128, 255);
    
    //Hover states, initialize all to false
    scholarHoverStates.assign(10, false);
    page4OptionHoverStates.assign(4, false);
    
    thinDivider.load("assets/interface/filigree/thinDivider.png");
    thinDivider.setAnchorPercent(0.5f, 0.5f);
    
    divider.load("assets/interface/filigree/divider.png");
    divider.setAnchorPercent(0.5f, 0.5f);
    
    
    //Animation and page flipping
    currentOpenPage = 0;
    targetAnimationPos = animationSpread1;
    
    //don't check for touches
    lastTouchTime = 0;
    
    
    
    //make a tag button by hijacking the tag
    //button object we use for the regular books
    tagPos.set(-35, 250, 0);
    
    tagButton.setup(3, tagPos);
    tagButton.setFont(&tagButtonFont);
    tagButton.centerBookButton = true;
    tagButton.setTag("", 0, ofColor(0));
    tagButton.helpTextCol = textColor;
    tagButton.show();
    
    tagHelpTextPos.set(page1LeftMargin, tagPos.y - scholarFont.stringHeight("A") * 4 - 15);
    tagHelpText = "Tag associated\nwith this\nscholar:";
    
    //text for page 4 options
    //multi lines are broken out into different elements of vector
    page4OptionText.resize(8);
    page4OptionText[0] = "Show Scholar";
    page4OptionText[1] = "Portrait";
    page4OptionText[2] = "Show Scholar";
    page4OptionText[3] = "Fact Sheet";
    page4OptionText[4] = "Set Room to";
    page4OptionText[5] = "This Scholar";
    page4OptionText[6] = "Back to";
    page4OptionText[7] = "Scholar List";
    
    //draw everything to the texture
    drawContentToTexture();
    
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
    
    
    //update variables with values from GUI if it's active
    if(bIsGuiActive){
        
        if(reMapMeshButton){
            mapMesh();
        }
        
        setVariablesFromGui();
        
    }
    

    
    
    
    //if we're showing the raw desk, see if the mouse is inside of it and
    //pass normalized mouse touches into the FBO
    
    //this is practice for when touches actually come in through OSC
    //Touches will be normalized from 0-1 over the width/height of the entire desk
    
    mouseTouches.clear();
    
    if(showRawDeskToggle && bIsGuiActive){
        
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
    
    
    //go through and manage scholars
    for(int i = 0; i < scholarHoverStates.size(); i++){
        //set all the hover states off, we'll turn them
        //back on if we're actually hovering over them
        scholarHoverStates[i] = false;
    }
    
    //same for page 4 options
    for(int i = 0; i < page4OptionHoverStates.size(); i++){
        page4OptionHoverStates[i] = false;
    }
    
    

    
    
    //go through mouse touches and check for detection zones
    for(int i = 0; i < mouseTouches.size(); i++){
        
        
        ofVec2f m = mouseTouches[i].pos;
//        cout << m << endl;
        
        //touch data is normalized from 0-1 in x and y
        //for now, x is inverted (0 at right) and y is normal
        
        //see if we're over book
        if(m.x < bookLeftBoundSlider && m.y > bookPageTopSlider && m.y < bookPageBottomSlider){
            
            //now we have different detection zones based on what page we're on
            if(currentOpenPage == 0){
            

                
                
                //FIRST OPEN PAGE: SCHOLAR SELECT
                //The height of each name region = space between the top and bottom bounds divided into 5 regions
                float regionHeight = (bookPageBottomSlider - bookPageTopSlider)/5.0f;
                
                int scholarIndex;
                
                //see which page we're on
                if(m.x > bookCenterSlider){
                    //left page
                    
                    scholarIndex = floor( (m.y - bookPageTopSlider)/regionHeight );
                    
                } else {
                    //right page = next 5 scholars
                    scholarIndex = floor( (m.y - bookPageTopSlider)/regionHeight ) + 5;
                    
                }
                
                //set hover state to true
                scholarHoverStates[scholarIndex] = true;
                
                
                //check if the "touch" is actually selecting the scholar
                if(mouseTouches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                    
                    currentOpenPage = 1;
                    currentScholar = scholarIndex;
                    
                    //set frame to show this scholar
                    frame -> changeScholar(currentScholar);
                    
                    cout << "Changing scholar to: " << currentScholar << endl;
                    
                    //set the tag button to the current scholar's data
                    string t = scholarData -> scholarList[currentScholar].tag;
                    ofColor c = scholarData -> tagColorList[currentScholar];
                    tagButton.setTag(t, currentScholar, c);
                    
                    lastTouchTime = ofGetElapsedTimeMillis();
                    
                    //don't check any other touches this frame
                    break;
                    
                }
                
            } else if(currentOpenPage == 1){
                
                //SECOND OPEN PAGE: SCHOLAR INFO AND TAG SELECT
                
                //which option on the right page is beign selected
                int optionSelect;
                
                //We already know we're hovering over the book
                //but let's see which page we're on
                if(m.x > bookCenterSlider){
                    //left page
                    
//                    scholarIndex = floor(m.y / 0.2);
                    if(m.y > 0.62 && m.y < 0.87){
                        
                        //if we're inside the tag button, fake a button press
                        //to trigger the event to the book controller
                        tagButton.checkForClicks(tagButton.currentPos.x + 10, tagButton.currentPos.y + 10, mouseTouches[i].bIsTouching);
                    }
                    
                    
                } else {
                    
                    //right page
                    //The height of each option = space between the top and bottom bounds divided into 4 regions
                    float regionHeight = (bookPageBottomSlider - bookPageTopSlider)/4.0f;
                    optionSelect = floor(m.y / regionHeight);
                    
                    page4OptionHoverStates[optionSelect] = true;

                    if(mouseTouches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){

                        
                        lastTouchTime = ofGetElapsedTimeMillis();
                        
                        //just check the 4th button to go back to the first page for now
                        if(optionSelect == 0){
                            
                            frame -> changeScholar(currentScholar);
                            frame -> hideFactSheet();
                            
                            
                        } else if(optionSelect == 1){

                            frame -> changeScholar(currentScholar);
                            frame -> showFactSheet();
                            
                            
                        } else if(optionSelect == 2){
                            
                            //set room to scholar(?)
                            
                        } else if(optionSelect == 3){
                            currentOpenPage = 0;
                        }
                        
                    }
                    
                }
                
                
                
            }
            
        }
        
        
        
    }
    
    
    //hack to save a few CPU cycles. Only redraw the texture if there are actual touches.
    //otherwise the texture will be static.
    if(mouseTouches.size()){
        
        //draws text to book
        drawContentToTexture();
        
    }
    
    
    
    //Actual book manipulation
    
    if(currentOpenPage == 0){
        targetAnimationPos = animationSpread1;
        currentHelpText = 0;
    } else if(currentOpenPage == 1){
        targetAnimationPos = animationSpread2;
        currentHelpText = 1;
    }
    
    animationPos = ofLerp(animationPos, targetAnimationPos, 0.05);
    
    model.setPositionForAllAnimations(animationPos);
    model.update();
    
    
    //tag button stuff
    tagButton.currentPos = tagPos;
    tagButton.displayedPos = tagPos;
    tagButton.update();

    
    
    
    
    //will be set back to true if we're actually drawing the gui.
    bIsGuiActive = false;
    
    
}






void CenterBook::drawContentToTexture(){
    
    //-----DRAW TEXT ONTO TEXTURE-----
    
    bookTexFBO.begin();
    
    //clear the FBO from the last frame
    ofClear(255, 255, 255, 0);
    
    //draw the base texture first
    ofSetColor(255);
    bookTex.draw(0, 0);
    
    float padding = 5;
    

    //DRAW FIRST OPEN SPREAD: SCHOLAR LIST
    highlightBoxHeight = (pageHeight - topMargin * 2 - betweenScholars * 4)/5.0f;
    
    //go through all the pages and draw out the text
    for(int i = 0; i < scholarData -> scholarList.size(); i++){
        
        ofPushMatrix();
        
        float boxX, x1, x2, y, w;
        
        //draw left page names
        if(i < 5){

            ofTranslate(pageTexCoords[0]);

            boxX = page1LeftMargin;
            y = topMargin + (highlightBoxHeight + betweenScholars) * i;
            w = pageWidth - page1LeftMargin * 2;
            
            //stagger 2nd and 4th names to right align
            if(i % 2 == 0){
                x1 = page1LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 3 - scholarFont.stringWidth(scholarData -> scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 3 - scholarFont.stringWidth(scholarData -> scholarList[i].nameBottom);
            }
            
        } else {
            
            //draw right page names
            ofTranslate(pageTexCoords[1]);
            
            boxX = page2LeftMargin;
            y = topMargin + (highlightBoxHeight + betweenScholars) * (i - 5);
            w = pageWidth - page2LeftMargin - page1LeftMargin;
            
            //staggered names are odd on this page
            if(i % 2 == 1){
                x1 = page2LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(scholarData -> scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(scholarData -> scholarList[i].nameBottom);
            }
            
        }

        //draw the filigree thinDivider above all names except the 0th and 5th names
        if(i != 0 && i != 5){
            
            ofSetColor(255);
            thinDivider.draw(pageWidth/2, y - betweenScholars/2, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
            
        }
        
        
        //draw highlight box
        if(scholarHoverStates[i]){
            ofSetColor(highlightColor);
            ofDrawRectangle(boxX, y, w, highlightBoxHeight);
            
            ofSetColor(255);
        } else {
            ofSetColor(textColor);
        }
        
        scholarFont.drawString(scholarData -> scholarList[i].nameTop, x1 + padding, y + padding + lineHeight);
        scholarFont.drawString(scholarData -> scholarList[i].nameBottom, x2 + padding, y + padding + lineHeight * 2 + betweenNames);

        
        ofPopMatrix();
    }

    //DRAW SECOND OPEN SPREAD: SCHOLAR INFO AND TAG SELECT
    highlightBoxHeight = (pageHeight - topMargin * 2 - betweenScholars * 3)/4.0f;
    
    //left page
    ofPushMatrix();
    ofTranslate(pageTexCoords[2]);
    
    ofSetColor(textColor);
    scholarFont.drawString(scholarData -> scholarList[currentScholar].nameTop, page1LeftMargin, topMargin + lineHeight);
    scholarFont.drawString(scholarData -> scholarList[currentScholar].nameBottom, page1LeftMargin, topMargin + lineHeight * 2 + betweenNames * 2);
    
    //draw divider below bottom name
    ofSetColor(textColor);
    divider.draw(pageWidth/2 - 10, topMargin + lineHeight * 2 + betweenNames + 20, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
    
    
    //draw tag help text

    //squish the font together a bit
    scholarFont.setLetterSpacing(0.95);
    scholarFont.drawString(tagHelpText, tagHelpTextPos.x, tagHelpTextPos.y);
    
    
    //draw tag button
    tagButton.draw();
    
    ofPopMatrix();
    
    //right page has 4 options
    //first 3 are left align, last one is right align
    for(int i = 0; i < 4; i++){
        
        ofPushMatrix();
        
        float boxX, x1, x2, y, w;
        
        //Move to right page of spread 2
        ofTranslate(pageTexCoords[3]);
        
        //positioning of elements and the highlight box
        boxX = page2LeftMargin;
        y = topMargin + (highlightBoxHeight + betweenScholars) * i;
        w = pageWidth - page2LeftMargin - page1LeftMargin;
        
        //4th option is right aligned
        if(i < 3){
            x1 = page2LeftMargin;
            x2 = x1;
        } else {
            x1 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(page4OptionText[ i * 2 ]);
            x2 = pageWidth - page1LeftMargin * 2 - scholarFont.stringWidth(page4OptionText[ i * 2 + 1 ]);
        }
        
        //draw the filigree thinDivider above all names except the first one
        if(i != 0){
            
            ofSetColor(255);
            thinDivider.draw(pageWidth/2, y - betweenScholars/2, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
            
        }
        
        
        //draw highlight box
        if(page4OptionHoverStates[i]){
            ofSetColor(highlightColor);
            ofDrawRectangle(boxX, y, w, highlightBoxHeight);
            
            ofSetColor(255);
        } else {
            ofSetColor(textColor);
        }
        
        scholarFont.drawString(page4OptionText[ i * 2 ]    , x1 + padding, y + padding + lineHeight);
        scholarFont.drawString(page4OptionText[ i * 2 + 1 ], x2 + padding, y + padding + lineHeight * 2 + betweenNames);
            
        
        ofPopMatrix();
    }
    
    
    //right page
    ofPushMatrix();
    ofTranslate(pageTexCoords[3]);
    

    
    
    ofPopMatrix();
    
    
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
        
        //divide by two since we're scaling the font down to half
        float lineHeight = boldUIFont.stringWidth("A");

        float startHeight = (deskHeight - lineHeight * 4 - betLines * 4)/2.0f + lineHeight/2.0f; //add an extra line to the end since text draws from the baseline
        
        ofPushMatrix();
        

        
        //invert the text to cancel out the text texture flipping
        ofScale(-1, -1);
        
        for(int i = 0; i < helpText[currentHelpText].size(); i++){
            
            ofSetColor(255);
            string s = helpText[currentHelpText][i];
            ofVec2f thisLine(borderPos.x + borderWidth/2 + boldUIFont.stringWidth(s)/ 2.0f, startHeight + (lineHeight + betLines) * i);

            //invert the position to cancel out the scaling effect on position
            thisLine *= -1;
            ofPushMatrix();
//            ofScale(0.5f, 0.5f);
            boldUIFont.drawString(s, thisLine.x, thisLine.y);
            ofPopMatrix();
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
    
    deskFBO.begin();
    
    ofSetLineWidth(2);
    ofSetColor(255, 0, 255);
    
    //Draw lines that outline touch regions
    //We're not using the camera to draw in debug so we need to flip everything
    //to match the coordinates that ARE being drawn with the camera in draw()
    float topY = (1.0f - bookPageTopSlider) * deskHeight;
    float botY = (1.0f - bookPageBottomSlider) * deskHeight;
    float leftX = (1.0f - bookLeftBoundSlider) * deskWidth;
    float centerX = (1.0f - bookCenterSlider) * deskWidth;
    
    //Book left edge
    ofDrawLine(leftX, 0, leftX, deskHeight);
    //Book center
    ofDrawLine(centerX, 0, centerX, deskHeight);
    
    //Top and bottom bounds
    ofDrawLine(leftX, topY, deskWidth, topY);
    ofDrawLine(leftX, botY, deskWidth, botY);
    
    deskFBO.end();

    
    
    
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
    
    
    bIsGuiActive = true;
    
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
    
    gui.add(detectionLabel.setup("  TOUCH REGIONS", ""));
    gui.add(bookLeftBoundSlider.setup("Book Left Bound", 0.6f, 0.0, 1.0));
    gui.add(bookCenterSlider.setup("Book Center", 0.31f, 0.0, 1.0));
    gui.add(bookPageTopSlider.setup("Book Top Bound", 0.1f, 0.0, 1.0));
    gui.add(bookPageBottomSlider.setup("Book Bottom Bound", 0.95f, 0.0, 1.0));
    gui.add(touchWaitSlider.setup("Wait after touch", 500, 10, 2000));
    
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
    detectionLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    positioningLabel.setDefaultTextColor(ofColor(0));
    
    gui.setPosition(meshPoints[0].x - 300, meshPoints[0].y - 160);
    
    bIsGuiActive = true;
    
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

