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

    
    borderMargin = 5;

    //scale it down so it fits in the space just right
    borderHeight = (deskHeight - borderMargin * 2);
    borderWidth = filigreeBorder.getWidth() * borderHeight/(float)filigreeBorder.getHeight();

    //image X is inverted inside FBO so image anchors from the apparent top right corner
    borderPos.set(deskWidth - borderMargin - borderWidth, borderMargin);
    
    
    
    //set help text
    /*
     0 = home page
     1 = scholar list
     2 = single scholar
     3 = tag list
     4 = single tag
     */
    helpText.resize(5);
    
    helpText[0].resize(4);
    helpText[0][0] = "What would";
    helpText[0][1] = "you like";
    helpText[0][2] = "to learn";
    helpText[0][3] = "about?";
    
    helpText[1].resize(4);
    helpText[1][0] = "Choose a";
    helpText[1][1] = "scholar to";
    helpText[1][2] = "learn about";
    helpText[1][3] = "their work";

    helpText[2].resize(5);
    helpText[2][0] = "Learn about";
    helpText[2][1] = "this scholar and";
    helpText[2][2] = "see their";
    helpText[2][3] = "contributions";
    helpText[2][4] = "in this library";
    
    helpText[3].resize(5);
    helpText[3][0] = "Choose";
    helpText[3][1] = "one of the";
    helpText[3][2] = "library's main";
    helpText[3][3] = "content";
    helpText[3][4] = "areas";
    
    helpText[4].resize(5);
    helpText[4][0] = "Learn about";
    helpText[4][1] = "tags and";
    helpText[4][2] = "find other";
    helpText[4][3] = "books in this";
    helpText[4][4] = "category";
    
    currentHelpText = 0;

    
    //Home button stuff
    homeButtonWidth = 52;
    homeButtonHeight = 18;
    homeButtonBottomMargin = 5;


    //apparent position of book on screen after FBO is mapped
    apparentBookCenter.set( 1094, 840, -80);
    
    
    
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
    

    
    mapMesh();
    

    
    //The position the raw desk will draw when debugging
    rawDeskPos.set( ofGetWidth()/2.0 - deskWidth/2, ofGetHeight()/2 - deskHeight + 50 );
    
    
    
    //Layout of the central book content
    //first page spread: list of scholars
    topMargin = page1LeftMargin;

    float betweenListItems = 15;
    float highlightBoxHeight = (pageHeight - topMargin * 2 - betweenListItems * 4)/5.0f;
    float spaceForName = highlightBoxHeight;
    betweenNames = 4;

    int nameFontSize = (spaceForName - betweenNames)/2.0f;
    nameFontSize *= 0.85;
        
    //scholar font
    listFont.load("assets/interface/EBGaramondSC12-Regular.ttf", nameFontSize);
    listFont.setLetterSpacing(.95);
    
    lineHeight = listFont.stringHeight("A");
    

    //help text on the left
    helpTextFont.load("assets/interface/Century-gothic-bold.ttf", 14);
    helpTextFont.setLetterSpacing(0.95);
    
    //used for home button
    homeButtonFont.load("assets/interface/Century-gothic.ttf", 11);
    homeButtonFont.setLetterSpacing(0.95);

    
    //font used for descriptions/non selectable text
    staticTextFont.load("assets/interface/EBGaramond12-Regular.ttf", 24);
    staticTextFont.setLetterSpacing(0.92);
    staticTextFont.setLineHeight(staticTextFont.stringHeight("Ap") -1);

    //font used for selectable text
    activeTextFont.load("assets/interface/EBGaramond08-Regular.ttf", 24);
    activeTextFont.setLetterSpacing(0.95);
    
    staticTextColor.set(255 * 0.45);
    activeTextColor.set(255 * 0.3);
    listTextColor.set(255 * 0.25);
    
    buttonHighlightColor.set(0, 128, 255);
    buttonBaseColor.set(255 * 0.78);
    
    bookFiligreeColor.set( 255 * 0.7);

    
    //Hover states for second spread list, initialize all to false
    listHoverStates.assign(10, false);
    scholarOptionHoverStates.assign(3, false);
    
    thinDivider.load("assets/interface/filigree/thinDivider.png");
    thinDivider.setAnchorPercent(0.5f, 0.5f);
    
    divider.load("assets/interface/filigree/divider1.png");
    divider.setAnchorPercent(0.5f, 0.5f);
    
    
    cornerFiligree.load("assets/interface/filigree/cornerFiligree.png");
    
    
    redecorateHeights.assign(11, 0);
    
    
    //Animation and page flipping
    currentOpenPage = 0;
    targetAnimationPos = animationSpread1;
    
    //don't check for touches
    lastTouchTime = 0;
    
    
    
    //make a tag button by hijacking the tag
    //button object we use for the regular books
//    tagPos.set(-35, 250, 0);
//    
//    //some aspects (like tag text) will be set
//    //later in run-time
//    tagButton.setup(3, tagPos);
//    tagButton.setFont(&tagButtonFont);
//    tagButton.bIsCenterBookButton = true;
//    tagButton.setTag("", "", 0, ofColor(0));
//    tagButton.helpTextCol = staticTextColor;
//    tagButton.show();
    

    
    
    
    //this chooses what content is drawn to book
    //texture: scholar or tag information
    bViewingScholars = true;
    
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


void CenterBook::receiveTouch(Touch t){
    
    //check to see if t is new or if there
    //exists one with the same ID already
    
    bool touchExists = false;
    int existingIndex = -1;
    
    for(int i = 0; i < touches.size(); i++){
        
        if(touches[i].ID == t.ID){
            
            touchExists = true;
            existingIndex = i;
            break;
            
        }
        
    }
    
    //if it's new, add it. If not, update it.
    if( touchExists ){
        
        touches[existingIndex].ID = t.ID;
        touches[existingIndex].pos = t.pos;
        touches[existingIndex].dist = t.dist;
        touches[existingIndex].bIsTouching = t.bIsTouching;
        
    } else {
        
        t.birthTime = ofGetElapsedTimef();
        touches.push_back(t);
        
        
    }
    
    



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
    
//    mouseTouches.clear();
    
    if(showRawDeskToggle && bIsGuiActive){
        
        ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
        
        if(mouse.x > rawDeskPos.x && mouse.x < rawDeskPos.x + deskWidth && mouse.y > rawDeskPos.y && mouse.y < rawDeskPos.y + deskHeight){
            
            ofVec2f normalizedMouse((mouse.x - rawDeskPos.x)/deskWidth, (mouse.y - rawDeskPos.y)/deskHeight);
            
//            MouseTouch mt;
//            mt.pos = normalizedMouse;
//            mt.bIsTouching = ofGetMousePressed();
//            mouseTouches.push_back(mt);
//            if(printNormCoordsToggle) cout << mt.pos << endl;

            
            //add mouse touch to touches vector
            Touch m;
            m.ID = 99999999;
            m.pos = normalizedMouse;
            m.dist = 1000;
            m.bIsTouching = ofGetMousePressed();
            
            receiveTouch(m);
            
        }
        
        
    }
    
    

    
    
    //go through and manage scholars
    for(int i = 0; i < listHoverStates.size(); i++){
        //set all the hover states off, we'll turn them
        //back on if we're actually hovering over them
        listHoverStates[i] = false;
    }
    
    //same for page 4 options
    for(int i = 0; i < scholarOptionHoverStates.size(); i++){
        scholarOptionHoverStates[i] = false;
    }
    
    
    //turn off hover states
    homeButtonHover = false;
    scholarButtonHover = false;
    tagButtonHover = false;
    redecorateButtonHover = false;
    tagHover = false;
    backToListHover = false;
    
    //go through mouse touches and check for detection zones
    for(int i = 0; i < touches.size(); i++){
        
    
        //touch data is normalized from 0-1 in x and y
        ofVec2f thisTouch = touches[i].pos;
        

        
        //see if we're over book
        if(thisTouch.x > bookLeftBoundSlider && thisTouch.y > bookPageTopSlider && thisTouch.y < bookPageBottomSlider){
            
            //now we have different detection zones based on what page we're on
            if(currentOpenPage == 0){
                
                //check if we're in the vertical slice with the two buttons
                if(thisTouch.y > 0.17 && thisTouch.y < 0.36){

                    //check what page we're on
                    if(thisTouch.x > 0.48 && thisTouch.x < bookCenterSlider){

                        //left page
                        scholarButtonHover = true;
                        
                        if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                            currentOpenPage = 1;
                            bViewingScholars = true;
                            lastTouchTime = ofGetElapsedTimeMillis();
                        }
                        
                    } else if(thisTouch.x > bookCenterSlider && thisTouch.x < 0.89){
                        //right page
                        tagButtonHover = true;
                        
                        if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                            currentOpenPage = 1;
                            bViewingScholars = false;
                            lastTouchTime = ofGetElapsedTimeMillis();
                        }
                        
                    }
                    
                } else if(thisTouch.x > bookCenterSlider && thisTouch.y > 0.73){
                    
                    redecorateButtonHover = true;
                    
                    if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){

                        //do redecorate animation here...
                        
//                        bool redecorate = true;
//                        ofNotifyEvent(redecorateEvent, redecorate, this);
                        
                        SceneEvent se;
                        
                        //set at just above desk level
                        se.pos.set( ofGetWidth()/2, 700 );
                        se.type = SceneEvent::CENTERBOOK;
                        
                        ofNotifyEvent(redecorateEvent, se, this);
                        
                        
                        lastTouchTime = ofGetElapsedTimeMillis();
                    }
                    
                }
                
                
                
                
            } else if(currentOpenPage == 1){
                
                //Second OPEN PAGE: LIST SELECT - CAN BE SCHOLARS OR TAGS
                //The height of each name region = space between the top and bottom bounds divided into 5 regions
                float regionHeight = (bookPageBottomSlider - bookPageTopSlider)/5.0f;
                
                int listIndex;
                
                //see which page we're on
                if(thisTouch.x < bookCenterSlider){
                    //left page
                    
                    listIndex = floor( (thisTouch.y - bookPageTopSlider)/regionHeight );
                    
                } else {
                    //right page = next 5 scholars
                    listIndex = floor( (thisTouch.y - bookPageTopSlider)/regionHeight ) + 5;
                    
                }
                
                //set hover state to true
                listHoverStates[listIndex] = true;
                
                
                //check if the "touch" is actually selecting the scholar
                if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                    
                    currentOpenPage = 2;
                    
                    if(bViewingScholars){

                        currentScholar = listIndex;
                        
                        //set frame to show this scholar
                        frame -> changeScholar(currentScholar);
                        frame -> hideFactSheet();
                        
                        //set the tag button to the current scholar's data
                        tagLine1 = "Reveal Scholar";
                        tagLine2 = "Contributions";
                        tagHoverColor = buttonHighlightColor;
                        
                        
                        //"Show Portrait" option is automatically highlighted
                        selectedScholarOption = 0;
                    
                    } else {
                        
                        currentTag = listIndex;
                        
                        //set the tag button to the current tag's data
                        tagLine1 = scholarData -> formattedTagList[currentTag][0];
                        tagLine2 = scholarData -> formattedTagList[currentTag][1];
                        
                        tagHoverColor = scholarData -> tagColorList[currentTag];
                        tagTextColor.setHsb(tagHoverColor.getHue(), tagHoverColor.getSaturation(), 100);
                        
                        //get the tag description from scholar data and format it
                        tagDescription = scholarData -> tagDescriptions[currentTag];
                        
                        tagDescription = ScholarData::formatText(tagDescription, &staticTextFont, pageWidth - page1LeftMargin * 3);
                        
                    }
                    
                    
                    lastTouchTime = ofGetElapsedTimeMillis();
                    
                    //don't check any other touches this frame
                    break;
                    
                }
                
            } else if(currentOpenPage == 2){
                
                //SECOND OPEN PAGE: SCHOLAR INFO or TAG SELECT
                
                //which option on the right page is beign selected
                int hoveredOption;
                
                //We already know we're hovering over the book
                //but let's see which page we're on
                if(thisTouch.x < bookCenterSlider){
                    //left page

                    //over button area
                    if(thisTouch.y > 0.21){
                        
                        //"show portrait" button
                        if(thisTouch.y < 0.4){
                        
                            hoveredOption = 0;
                            
                        //"show factsheet" button
                        } else if(thisTouch.y < 0.6){
                            
                            hoveredOption = 1;
                            
                        //"show well known works" button
                        } else if(thisTouch.y < 0.78){
                            
                            hoveredOption = 2;
                            
                        }
                        
                        
                    }
                    
                    if( hoveredOption >= 0 && hoveredOption <= 2){
                        
                        scholarOptionHoverStates[hoveredOption] = true;
                        
                        if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                            
                            
                            lastTouchTime = ofGetElapsedTimeMillis();
                            
                            //don't do anything if this option is already selected
                            if(selectedScholarOption != hoveredOption){
                            
                                if(hoveredOption == 0){
                                    
                                    frame -> changeScholar(currentScholar);
                                    frame -> hideFactSheet();
                                    frame -> hideWorksSheet();
                                    
                                } else if(hoveredOption == 1){
                                    
                                    frame -> changeScholar(currentScholar);
                                    frame -> showFactSheet();
                                    frame -> hideWorksSheet();
                                    
                                } else if(hoveredOption == 2){
                                    
                                    frame -> changeScholar(currentScholar);
                                    frame -> showWorksSheet();
                                    frame -> hideFactSheet();
                                }
                                
                                selectedScholarOption = hoveredOption;
                                
                            }
                        }
                    }
                    
                } else {
                    
                    //right page
                    //tag button
                    if(thisTouch.y > 0.26 && thisTouch.y < 0.45){
                        
                        tagHover = true;
                        
                        //if we're inside the tag button, fake a Button press event
                        //to trigger the event to the book controller
                        if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){

                            //create a button event and send it to the book controller
                            ButtonEvent e;
                            
                            //tag buttons are type 3
                            e.type = 3;
                            e.bIsCenterBookButton = true;
                            
                            //tagNum will be either tag or scholar number
                            //depending on the bScholarButton boolean
                            e.bScholarButton = bViewingScholars;
                            e.tagNum = bViewingScholars ? currentScholar : currentTag;
                            e.tagCol = scholarData -> tagColorList[currentTag];
                            
                            //set these as non-applicable
                            e.bookNum = -1;
                            e.shelfNum = -1;
                            e.src = apparentBookCenter;
                            
                            ofNotifyEvent(newButtonClickEvt, e, this);
                            
                                                        
                            lastTouchTime = ofGetElapsedTimeMillis();
                        }
                        
                        //check for "back to list" button
                    } else if(thisTouch.y > 0.61 && thisTouch.y < 0.78){
                        
                        backToListHover = true;
                        
                        //go back to the list
                        if(touches[i].bIsTouching && ofGetElapsedTimeMillis() - lastTouchTime > touchWaitSlider){
                            
                            currentOpenPage = 1;
                            
                            lastTouchTime = ofGetElapsedTimeMillis();
                        }
                        
                    }
                    
                }
                
                
                
            }
            
        } else {
            
            //if mouse is NOT over the book, 
            //look for home button on first page
            if(currentOpenPage != 0){
                
                if(thisTouch.x > homeLeftX && thisTouch.x < homeRightX && thisTouch.y > homeTopY && thisTouch.y < homeBotY){
                    
                    homeButtonHover = true;
                    
                    if(touches[i].bIsTouching){
                        currentOpenPage = 0;
                        lastTouchTime = ofGetElapsedTimeMillis();
                    }
                    
                }
                
            }
            
        }
        
        
        
        
    }
    
    
    
    
    //remove all the touches that haven't been updated recently
    //starting from the end of the vector and moving to the front
    for(int i = touches.size() - 1; i >= 0; i--){
        
        touches[i].update();
        
        if(touches[i].age > touchKillAgeSlider){
            touches.erase( touches.begin() + i );
        }
        
    }
    
    
    
    drawContentToTexture();
    
    
    //Actual book manipulation
    
    if(currentOpenPage == 0){
        targetAnimationPos = animationSpread1;
        currentHelpText = 0;
    } else if(currentOpenPage == 1){
        targetAnimationPos = animationSpread2;
        
        if(bViewingScholars){
            currentHelpText = 1;
        } else {
            currentHelpText = 3;
        }
    } else {
        targetAnimationPos = animationSpread3;
        
        if(bViewingScholars){
            currentHelpText = 2;
        } else {
            currentHelpText = 4;
        }
    }
    
    if(currentOpenPage == 0){
        bShowHomeButton = false;
    } else {
        bShowHomeButton = true;
    }
    
    animationPos = ofLerp(animationPos, targetAnimationPos, 0.05);
    
    //if we're very very close to our target, no need to update the model
    //updating the model is expensive and should be done sparingly
    if( abs( animationPos - targetAnimationPos ) > 0.01){
        model.setPositionForAllAnimations(animationPos);
        model.update();
    }
    
    
    
    

    
    
    
    
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
    
    
    
    

    //Book texture overview:
    //Page 1: choose scholars or tags
    //  If scholars...
    //  Page 2: List of Scholars
    //  Page 3: Scholar page
    //  If Tags
    //  Page 2: List of Tags
    
    
    //----------DRAW FIRST OPEN SPREAD: SCHOLAR vs TAG SELECT----------
    
    float filigreeMargin = 10;
    float filigreeWidth = 50;
    
    float buttonWidth = 160;
    float buttonHeight = 80;
    float buttonTopMargin = 60;

    //-----PAGE ONE-----
    ofPushMatrix();
    ofTranslate(pageTexCoords[0]);
    
    //corner filigree
    ofSetColor(bookFiligreeColor);
    cornerFiligree.draw(filigreeMargin, filigreeMargin);
    cornerFiligree.draw(filigreeMargin, pageHeight - filigreeMargin, filigreeWidth, -filigreeWidth);
    
    
    //draw Scholar button
    if(scholarButtonHover){
        scholarButtonColPct = ofLerp(scholarButtonColPct, 1.0, colorLerpInSlider);
    } else {
        scholarButtonColPct = ofLerp(scholarButtonColPct, 0.0, colorLerpOutSlider);
    }
    
    //box color
    ofSetColor( buttonBaseColor.getLerped(buttonHighlightColor, scholarButtonColPct) );
    ofDrawRectangle(pageWidth/2 - buttonWidth/2, buttonTopMargin, buttonWidth, buttonHeight);

    //text color
    ofSetColor( activeTextColor.getLerped(ofColor(255), scholarButtonColPct) );
    
    vector<string> scholarButtonText = {"Show", "Scholars" };
    
    float betweenLines = 10;
    
    float string0Width = activeTextFont.stringWidth(scholarButtonText[0]);
    float string1Width = activeTextFont.stringWidth(scholarButtonText[1]);
    float stringHeight = activeTextFont.stringHeight("A");
    float textTopPadding = (buttonHeight - stringHeight * 2 - betweenLines)/2.0f;
    
    activeTextFont.drawString(scholarButtonText[0], pageWidth/2 - string0Width/2, buttonTopMargin + textTopPadding + stringHeight);
    activeTextFont.drawString(scholarButtonText[1], pageWidth/2 - string1Width/2, buttonTopMargin + textTopPadding + stringHeight * 2 + betweenLines);
    
    //filigree divider
    float filigreeVerticalArea = 40;
    ofSetColor(bookFiligreeColor);
    divider.draw(pageWidth/2, buttonTopMargin + buttonHeight + filigreeVerticalArea/2, divider.getWidth() * 0.7, divider.getHeight() * 0.7);
    
    
    vector<string> scholarHelpText;
    scholarHelpText.resize(3);
    scholarHelpText[0] = "View prominent";
    scholarHelpText[1] = "scholars in this";
    scholarHelpText[2] = "libary";
    
    
    float staticLineHeight = activeTextFont.stringHeight("A");
    float textYPos = buttonTopMargin + buttonHeight + filigreeVerticalArea + staticLineHeight;

    
    ofSetColor(staticTextColor);
    for(int i = 0; i < scholarHelpText.size(); i++){
        
        float w = staticTextFont.stringWidth(scholarHelpText[i]);
        staticTextFont.drawString(scholarHelpText[i], pageWidth/2 - w/2, textYPos);
        
        textYPos += staticLineHeight + betweenLines;
    }
    
    
    
    ofPopMatrix();
    
    
    
    //-----PAGE TWO-----
    ofPushMatrix();
    ofTranslate(pageTexCoords[1]);
    
    ofSetColor(bookFiligreeColor);
    cornerFiligree.draw(pageWidth - filigreeMargin, filigreeMargin, -filigreeWidth, filigreeWidth);
    cornerFiligree.draw(pageWidth - filigreeMargin, pageHeight - filigreeMargin, -filigreeWidth, -filigreeWidth);
    
    
    //draw Tag button
    if(tagButtonHover){
        tagButtonColPct = ofLerp(tagButtonColPct, 1.0, colorLerpInSlider);
    } else {
        tagButtonColPct = ofLerp(tagButtonColPct, 0.0, colorLerpOutSlider);
    }
    
    //box color
    ofSetColor( buttonBaseColor.getLerped(buttonHighlightColor, tagButtonColPct) );
    ofDrawRectangle(pageWidth/2 - buttonWidth/2, buttonTopMargin, buttonWidth, buttonHeight);
    
    //text color
    ofSetColor( activeTextColor.getLerped(ofColor(255), tagButtonColPct) );
    
    
    vector<string> tagButtonText = {"Show", "Tags"};
    
    string0Width = activeTextFont.stringWidth(tagButtonText[0]);
    string1Width = activeTextFont.stringWidth(tagButtonText[1]);
    
    activeTextFont.drawString(tagButtonText[0], pageWidth/2 - string0Width/2, buttonTopMargin + textTopPadding + stringHeight);
    activeTextFont.drawString(tagButtonText[1], pageWidth/2 - string1Width/2, buttonTopMargin + textTopPadding + stringHeight * 2 + betweenLines);
    

    ofSetColor(bookFiligreeColor);
    divider.draw(pageWidth/2, buttonTopMargin + buttonHeight + filigreeVerticalArea/2, divider.getWidth() * 0.7, divider.getHeight() * 0.7);
    
    
    vector<string> tagHelpText;
    tagHelpText.resize(3);
    tagHelpText[0] = "View how";
    tagHelpText[1] = "libary content";
    tagHelpText[2] = "is categorized";
    
    textYPos = buttonTopMargin + buttonHeight + filigreeVerticalArea + staticLineHeight;
    
    ofSetColor(staticTextColor);
    for(int i = 0; i < tagHelpText.size(); i++){
        
        float w = staticTextFont.stringWidth(tagHelpText[i]);
        staticTextFont.drawString(tagHelpText[i], pageWidth/2 - w/2, textYPos);
        
        textYPos += staticLineHeight + betweenLines;
    }
    
    
    
    //redecorate button
    
    vector<string> redecorateText = {"R", "e", "d", "e", "c", "o", "r", "a", "t", "e", "!"};
    if(redecorateButtonHover){

        float xPos = 30;
        float yAmp = 8;
        float sineSpeed = 3;
        float letterPhase = TWO_PI/(float)redecorateText.size();
        
        for(int i = 0; i < redecorateText.size(); i++){
            
            float yOscillate = yAmp * sin(ofGetElapsedTimef() * sineSpeed - letterPhase * i);
            
            redecorateHeights[i] = ofLerp(redecorateHeights[i], yOscillate, 0.04);
            
            ofColor c;
            c.setHsb(255 * i/(float)redecorateText.size(), 200, 200);
            
            ofSetColor(c);
            staticTextFont.drawString(redecorateText[i], xPos, pageHeight - 10 + redecorateHeights[i]);
            
            
            xPos += staticTextFont.stringWidth(redecorateText[i]) + 1;
            
        }
        
    } else {
        ofSetColor(staticTextColor);
//        activeTextFont.drawString("Redecorate!", 30, pageHeight - 10);
        float xPos = 30;

        for(int i = 0; i < redecorateHeights.size(); i++){
            
            //lerp letters back to still
            redecorateHeights[i] = ofLerp(redecorateHeights[i], 0, 0.05);
            
            //and draw them
            staticTextFont.drawString(redecorateText[i], xPos, pageHeight - 10 + redecorateHeights[i]);
            xPos += staticTextFont.stringWidth(redecorateText[i]) + 1;
        }
        
    }
    
    
    
//    activeTextFont.drawString("Redecorate!", 30, pageHeight - 10);
    
    
    
    ofPopMatrix();
    
    
    
    
    //----------DRAW SECOND OPEN SPREAD: SCHOLAR/TAG LIST----------
    
    betweenListItems = 15;
    highlightBoxHeight = (pageHeight - topMargin * 2 - betweenListItems * 4)/5.0f;
    float padding = 5;
    
    
    
    //go through all the pages and draw out the text
    for(int i = 0; i < scholarData -> scholarList.size(); i++){
        
        ofPushMatrix();
        
        float boxX, x1, x2, y, w;
        
        //Set left page names and positions
        if(i < 5){

            ofTranslate(pageTexCoords[2]);

            boxX = page1LeftMargin;
            y = topMargin + (highlightBoxHeight + betweenListItems) * i;
            w = pageWidth - page1LeftMargin * 2;
            
            //stagger 2nd and 4th names to right align but only if we're
            //looking at scholar names
            if(i % 2 == 0 || !bViewingScholars){
                x1 = page1LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 3 - listFont.stringWidth(scholarData -> scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 3 - listFont.stringWidth(scholarData -> scholarList[i].nameBottom);
            }
            
        } else {
            
            //Set right page names and positions
            ofTranslate(pageTexCoords[3]);
            
            boxX = page2LeftMargin;
            y = topMargin + (highlightBoxHeight + betweenListItems) * (i - 5);
            w = pageWidth - page2LeftMargin - page1LeftMargin;
            
            //staggered names are odd on this page
            //Only stagger if viewing scholars
            if(i % 2 == 1 || !bViewingScholars){
                x1 = page2LeftMargin;
                x2 = x1;
            } else {
                x1 = pageWidth - page1LeftMargin * 2 - listFont.stringWidth(scholarData -> scholarList[i].nameTop);
                x2 = pageWidth - page1LeftMargin * 2 - listFont.stringWidth(scholarData -> scholarList[i].nameBottom);
            }
            
        }

        //draw the filigree thinDivider above all names except the 0th and 5th names
        if(i != 0 && i != 5){
            
            ofSetColor(bookFiligreeColor);
            thinDivider.draw(pageWidth/2, y - betweenListItems/2, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
            
        }
        
        
        //draw highlight box
        if(listHoverStates[i]){
            
            //Draw the plain highlight color if we're viewing scholars
            //draw the tag color if not
            if(bViewingScholars){

                ofSetColor(buttonHighlightColor);
                ofDrawRectangle(boxX, y, w, highlightBoxHeight);
                
                //set the text color
                ofSetColor(255);
                
            } else {
                
                ofSetColor( scholarData -> tagColorList[i] );
                ofDrawRectangle(boxX, y, w, highlightBoxHeight);
                
                //set the text color
                ofColor c;
                c.setHsb(scholarData -> tagColorList[i].getHue(), scholarData -> tagColorList[i].getSaturation(), 100);
                ofSetColor(c);
            }
            
            
            
            
        } else {
            //set color of non highlighted text
            ofSetColor(listTextColor);
        }
        
        //draw the list text
        //scholars if viewing scholars, tag list if not
        
        
        
        if(bViewingScholars){
            listFont.drawString(scholarData -> scholarList[i].nameTop, x1 + padding, y + padding + lineHeight);
            listFont.drawString(scholarData -> scholarList[i].nameBottom, x2 + padding, y + padding + lineHeight * 2 + betweenNames);
        } else {
            listFont.drawString(scholarData -> formattedTagList[i][0], x1 + padding, y + padding + lineHeight);
            listFont.drawString(scholarData -> formattedTagList[i][1], x2 + padding, y + padding + lineHeight * 2 + betweenNames);
        }

        
        ofPopMatrix();
    }

    //----------DRAW THIRD OPEN SPREAD: SCHOLAR INFO AND TAG SELECT----------
    //Title/divider are same for scholar and tag
    

    
    
    //left page
    ofPushMatrix();
    ofTranslate(pageTexCoords[4]);
    
    string titleTop;
    string titleBot;
    
    if(bViewingScholars){
        
        titleTop = scholarData -> scholarList[currentScholar].nameTop;
        titleBot = scholarData -> scholarList[currentScholar].nameBottom;
        
    } else {
        
        titleTop = scholarData -> formattedTagList[currentTag][0];
        titleBot = scholarData -> formattedTagList[currentTag][1];
        
    }
    
    
    ofSetColor(listTextColor);
    listFont.drawString(titleTop, page1LeftMargin, topMargin + lineHeight);
    listFont.drawString(titleBot, page1LeftMargin, topMargin + lineHeight * 2 + betweenNames * 2);
    
    //draw divider below title
    ofSetColor(listTextColor);
    divider.draw(pageWidth/2 - 10, topMargin + lineHeight * 2 + betweenNames + 20, divider.getWidth(), divider.getHeight() );
    
    //draw rest of page depending on what we're doing
    if(bViewingScholars){
        
        //text for page 4 options
        //multi lines are broken out into different elements of vector
        vector<string> scholarPageOptionText;
        scholarPageOptionText.resize(6);
        scholarPageOptionText[0] = "Show";
        scholarPageOptionText[1] = "Portrait";
        scholarPageOptionText[2] = "Show";
        scholarPageOptionText[3] = "Factsheet";
        scholarPageOptionText[4] = "Show Well-";
        scholarPageOptionText[5] = "Known Works";
        
//        scholarPageOptionText[6] = "Back to";
//        scholarPageOptionText[7] = "Scholar List";
        
        //divides the page into 4 sections and determines the highlight box height
        //after factoring margins and spaces
        betweenListItems = 24;
        highlightBoxHeight = (pageHeight - topMargin * 2 - betweenListItems * 3)/4.0f;
        
        //left page has 3 options
        for(int i = 0; i < 3; i++){
            
            float boxX, x1, x2, y, w;
            
            //positioning of elements and the highlight box
            boxX = page1LeftMargin;
            
            //plus one so it starts at the 2nd quarter of the page
            //(first quarter is the title) then does the 3rd and 4th
            y = topMargin + (highlightBoxHeight + betweenListItems) * (i + 1)  - 20;  //20 for fudge
            w = pageWidth - page1LeftMargin * 2;
            
            x1 = page1LeftMargin;
            x2 = x1;
            
            //draw the filigree thinDivider above all names except the first one
            if(i != 0){
                
                ofSetColor(bookFiligreeColor);
                thinDivider.draw(pageWidth/2, y - betweenListItems/2, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
                
            }
            
            //draw highlight box
            if(scholarOptionHoverStates[i]){
                ofSetColor(buttonHighlightColor);
                ofDrawRectangle(boxX, y, w, highlightBoxHeight);
                
                ofSetColor(255);
            } else {
                
                //if this option is selected, draw the highlight box but grey
                if(i == selectedScholarOption){
                    ofSetColor(buttonBaseColor);
                    ofDrawRectangle(boxX, y, w, highlightBoxHeight);

                    ofSetColor(activeTextColor);
                } else {
                    ofSetColor(staticTextColor);
                }
                
                
            }
            //add a few pix to the Y to make it look right
            activeTextFont.drawString(scholarPageOptionText[ i * 2 ]    , x1 + padding, 4 + y + padding + lineHeight);
            activeTextFont.drawString(scholarPageOptionText[ i * 2 + 1 ], x2 + padding, 4 + y + padding + lineHeight * 2 + betweenLines);
            
            
        }
        

        
    } else {
        
        //tag description text
        
        ofColor(staticTextColor);
        staticTextFont.drawString(tagDescription, page1LeftMargin, topMargin + lineHeight * 2 + betweenNames + 60);
        
        
        
    }

    
    ofPopMatrix();
    

    
    
    //right page
    ofPushMatrix();
    ofTranslate(pageTexCoords[5]);
    
    // tag button help, tag button and
    float tagHelpYPos = pageTopMarginSlider + 15;
    string help1 = "Highlight books";
    string help2 = bViewingScholars ? "from this scholar:" : "with this tag:";
    
    float help1Width = staticTextFont.stringWidth(help1);
    float help2Width = staticTextFont.stringWidth(help2);
    
    ofSetColor(staticTextColor);
    staticTextFont.drawString(help1, pageWidth/2 - help1Width/2, tagHelpYPos + lineHeight);
    staticTextFont.drawString(help2, pageWidth/2 - help2Width/2, tagHelpYPos + lineHeight * 2 + betweenLines);
    
    //draw tag button

    //draw Scholar button
    buttonTopMargin = tagHelpYPos + lineHeight * 2 + betweenLines + 10;
    buttonWidth = pageWidth - page1LeftMargin * 2 - 5;
    if(tagHover){
        ofSetColor(tagHoverColor);
        ofDrawRectangle(pageWidth/2 - buttonWidth/2, buttonTopMargin, buttonWidth, buttonHeight);
        
        if(bViewingScholars){
            ofSetColor(255);
        } else {
            ofSetColor(tagTextColor);
        }
    } else {
        ofSetColor(buttonBaseColor);
        ofDrawRectangle(pageWidth/2 - buttonWidth/2, buttonTopMargin, buttonWidth, buttonHeight);
        
        ofSetColor(activeTextColor);
    }
    
    float tagLine1Width = activeTextFont.stringWidth(tagLine1);
    float tagLine2Width = activeTextFont.stringWidth(tagLine2);
    
    activeTextFont.drawString(tagLine1, pageWidth/2 - tagLine1Width/2, buttonTopMargin + textTopPadding + stringHeight);
    activeTextFont.drawString(tagLine2, pageWidth/2 - tagLine2Width/2, buttonTopMargin + textTopPadding + stringHeight * 2 + betweenLines);
    
    
    
    //back to list button (and filigree)
    
    float backButtonYPos = 0.7 * pageHeight;
    string back1 = "Back to";
    string back2 = bViewingScholars ? "Scholar List" : "Tag List";
    
    float back1Width = activeTextFont.stringWidth(back1);
    float back2Width = activeTextFont.stringWidth(back2);
    
    ofSetColor(bookFiligreeColor);
    thinDivider.draw(pageWidth/2, backButtonYPos, thinDivider.getWidth(), thinDivider.getHeight() * 2 );
    
    //text color
    ofSetColor( activeTextColor.getLerped(ofColor(255), tagButtonColPct) );
    
    if(backToListHover){
        ofSetColor(buttonHighlightColor);
        ofDrawRectangle(page1LeftMargin, backButtonYPos + 10, pageWidth - page1LeftMargin * 2, highlightBoxHeight + 10);
        
        ofSetColor(255);
        
    } else {
        ofSetColor(activeTextColor);
    }
    
    activeTextFont.drawString(back1, pageWidth - page1LeftMargin - back1Width - 10, backButtonYPos + 20 + lineHeight);
    activeTextFont.drawString(back2, pageWidth - page1LeftMargin - back2Width - 10, backButtonYPos + 20 + lineHeight * 2 + betweenLines);

    
    
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
        
        
        //draw the filigree border on the left
        ofSetColor(255);
        filigreeBorder.draw(borderPos, filigreeWidthSlider, filigreeHeightSlider);
        
        
        
        //draw the Home button
        //Home button stuff
        homeButtonWidth = 52;
        homeButtonHeight = 18;
        homeButtonBottomMargin = 5;
        
        //position coords inverted (+X to the left)
        //this is top RIGHT corner of box
        homeButtonPos.set(borderPos.x + borderWidth/2 - homeButtonWidth/2, borderPos.y + borderHeight - homeButtonHeight - homeButtonBottomMargin);
        
        
        
        //start text pos at button pos then adjust to center align/center
        ofVec2f homeTextPos = homeButtonPos;
        string hText = "Home";
        float hTextW = homeButtonFont.stringWidth(hText);
        float hTextH = homeButtonFont.stringHeight(hText);
        float margin = (homeButtonHeight - hTextH)/2.0f;
        
        homeTextPos += ofVec2f( homeButtonWidth/2 + hTextW/2 , homeButtonHeight/2 + hTextH/2 );
        
        if(bShowHomeButton){
            
            if(homeButtonHover){
                ofFill();
                ofSetColor(255);
                ofDrawRectangle(homeButtonPos, homeButtonWidth, homeButtonHeight);
                
                //color for text below
                ofSetColor(48, 25, 14);
            } else {
                ofNoFill();
                ofSetColor(255);
                ofDrawRectangle(homeButtonPos, homeButtonWidth, homeButtonHeight);
                ofFill();
                
                //color for text below
                ofSetColor(255);
            }
            
            //invert text
            ofPushMatrix();
            ofTranslate(homeTextPos);
            ofScale(-1, -1);
            homeButtonFont.drawString(hText, 0, 0);
            ofPopMatrix();
            
        }
        
        
        
        //draw help text
        //space between lines
        float betLines = 3;
        
        //Text pos defined by which help text we're showing
        //(how many lines) and if the home button is present
        float verticalTextArea = bShowHomeButton ? borderHeight - homeButtonHeight - homeButtonBottomMargin : borderHeight ;
        float lineHeight = helpTextFont.stringWidth("A");
        
        int numLines = helpText[currentHelpText].size();
        float textHeight = numLines * lineHeight + (numLines - 1) * betLines;

        //add an extra line height since text draws from baseline
        float helpTextTopMargin = (verticalTextArea - textHeight)/2.0f + lineHeight;
        
        
        //invert the text to cancel out the text texture flipping
        ofPushMatrix();
        ofScale(-1, -1);
        
        for(int i = 0; i < helpText[currentHelpText].size(); i++){
            
            ofSetColor(255);
            string s = helpText[currentHelpText][i];
            ofVec2f thisLine(borderPos.x + borderWidth/2 + helpTextFont.stringWidth(s)/ 2.0f, borderPos.y + helpTextTopMargin + (lineHeight + betLines) * i);

            //invert the position to cancel out the scaling effect on position
            thisLine *= -1;
            ofPushMatrix();
            helpTextFont.drawString(s, thisLine.x, thisLine.y);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        

        
        
        //go through touches (for now mouseTouches, but eventually OSC data from camera)
        //draw cursors, do button region detection, etc.
        for(int i = 0; i < touches.size(); i++){
            
            ofVec3f p = touches[i].pos;
            
            //convert from normalized
            //flip X because of FBO texture flipping
            p.x = 1.0f - p.x;
            p.x *= deskWidth;
            p.y *= deskHeight;
            p.z = -50;
            
            float rad = ofMap(touches[i].dist, 0, 50, 7, 50, true);
            
            //touch is green if touching
            //otherwise, lero from white to green-ish
            if(touches[i].bIsTouching){

                ofSetColor(0, 255, 0);
                
            } else {
                
                float colorPct = ofMap(touches[i].dist, 0, 40, 1.0, 0.0, true);

                ofColor c(255);
                c.lerp(ofColor(0, 255, 0), colorPct);
                
                ofSetColor(c);
            }
            
            ofSetLineWidth(3);
            ofNoFill();
            ofDrawCircle(p, rad);
            
//            if(touches[i].bIsTouching){
//                ofSetColor(255, 0, 0);
//            } else {
//                ofSetColor(0);
//            }
//        
//            
//            ofDrawCircle((1.0f - touches[i].pos.x) * deskWidth, touches[i].pos.y * deskHeight, -50, 5);
            
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
    //Y inverted so draw accordingly
    float bookTopY = (1.0f - bookPageTopSlider) * deskHeight;
    float bookBotY = (1.0f - bookPageBottomSlider) * deskHeight;
    float bookLeftX = bookLeftBoundSlider * deskWidth;
    float bookCenterX = bookCenterSlider * deskWidth;
    
    //Book left edge
    ofDrawLine(bookLeftX, bookTopY, bookLeftX, bookBotY);
    //Book center
    ofDrawLine(bookCenterX, bookTopY, bookCenterX, bookBotY);
    
    //Top and bottom bounds
    ofDrawLine(bookLeftX, bookTopY, deskWidth, bookTopY);
    ofDrawLine(bookLeftX, bookBotY, deskWidth, bookBotY);
    
    //show the detection bounds of the home button
//    if(bShowHomeButton){
//        //home button bounds
//        float hTopY = (1.0f - homeTopY) * deskHeight;
//        float hBotY = (1.0f - homeBotY) * deskHeight;
//        float hLeftX = homeLeftX * deskWidth;
//        float hRightX = homeRightX * deskWidth;
//        
//        //left and right bounds
//        ofDrawLine(hLeftX, hTopY, hLeftX, hBotY);
//        ofDrawLine(hRightX, hTopY, hRightX, hBotY);
//        
//        //Top and bottom bounds
//        ofDrawLine(hLeftX, hTopY, hRightX, hTopY);
//        ofDrawLine(hLeftX, hBotY, hRightX, hBotY);
//    }
    
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
    
    
    ofVec2f min(0, 0);
    ofVec2f max(1920, 1200);
    
    gui.add(guiPosSlider.setup("Gui Position", min, min, max));
    
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
    
    gui.add(filigreeTopMarginSlider.setup("Filigree Top M.", 5, 0, 25));
    gui.add(filigreeLeftMarginSlider.setup("Filigree Left M.", 5, 0, 25));
    gui.add(filigreeWidthSlider.setup("Filigree Width", 150, 0, 200));
    gui.add(filigreeHeightSlider.setup("Filigree Height", 150, 0, 200));
    gui.add(helpTextTopSlider.setup("Help Text Top M.", 10, 0, 100));
    gui.add(colorLerpInSlider.setup("Btn Col LerpIN", 0.08f, 0.001, 0.3));
    gui.add(colorLerpOutSlider.setup("Btn Col LerpOUT", 0.02f, 0.001, 0.3));
    
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
    gui.add(printNormCoordsToggle.setup("Print Norm. Mouse", false));
    gui.add(bookLeftBoundSlider.setup("Book Left Bound", 0.6f, 0.0, 1.0));
    gui.add(bookCenterSlider.setup("Book Center", 0.31f, 0.0, 1.0));
    gui.add(bookPageTopSlider.setup("Book Top Bound", 0.1f, 0.0, 1.0));
    gui.add(bookPageBottomSlider.setup("Book Bottom Bound", 0.95f, 0.0, 1.0));
    gui.add(touchWaitSlider.setup("Wait after touch", 500, 10, 2000));
    gui.add(touchKillAgeSlider.setup("Time touches last", 0.25, 0.0, 1.0));
    
    
    maxRangeX = 100;
    maxRangeY = 320;
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
    
    gui.minimizeAll();
    
    
    loadSettings();
    setVariablesFromGui();
    
    //now that we've loaded, move the gui to where we last had it
    gui.setPosition(guiPosSlider -> x, guiPosSlider -> y);
    
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
    
    guiPosSlider = gui.getPosition();
    
    borderPos.set(deskWidth - filigreeLeftMarginSlider - filigreeWidthSlider, filigreeTopMarginSlider);
    borderWidth = filigreeWidthSlider;
    borderHeight = filigreeHeightSlider;

    
    //position coords of home button inverted (+X to the left)
    //this is top RIGHT corner of box
    homeButtonPos.set(borderPos.x + borderWidth/2 - homeButtonWidth/2, borderPos.y + borderHeight - homeButtonHeight - homeButtonBottomMargin);
    
    //normalized home button bounds
    homeTopY = homeButtonPos.y/deskHeight;
    homeBotY = (homeButtonPos.y + homeButtonHeight)/deskHeight;
    
    homeLeftX = 1.0f - (homeButtonPos.x + homeButtonWidth)/deskWidth;
    homeRightX = 1.0f - homeButtonPos.x/deskWidth;
    
    
    
}


void CenterBook::drawGui(){
    gui.draw();
}

void CenterBook::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
}

