//
//  Book.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/15/16.
//
//

#include "Book.hpp"



Book::Book(){
    
}

void Book::loadModel(int bType, int tType, int fType){

    bookType = bType;
    texType = tType;
    fontType = fType;
    
    if(bookType == 0){
        model.loadModel("books/bookShort.fbx", false);
    } else if(bookType == 1){
        model.loadModel("books/bookMedium.fbx", false);
    } else {
        model.loadModel("books/bookTall.fbx", false);
    }

    

    
}


void Book::setup(ofTexture *_tex, ofTrueTypeFont *_bookFont, ofTrueTypeFont *_UIFont){

    tex = _tex;
    
    font = _bookFont;
    UIFont = _UIFont;
    
    
    
    //--------------------BOOK POSITIONING AND SCALING--------------------
    
    bPrintDebug = false;
    if(bPrintDebug){
        
        //figure out the minimum and maximum dimensions
        float maxX = -100000;
        float minX =  100000;
        float maxY = -100000;
        float minY =  100000;
        float maxZ = -100000;
        float minZ =  100000;
        
        
        ofMesh m0 = model.getMesh(0);
        for(int i = 0; i < m0.getNumVertices(); i++){
            
            ofVec3f thisVert = m0.getVertex(i);
            
            if(thisVert.x > maxX) maxX = thisVert.x;
            if(thisVert.x < minX) minX = thisVert.x;
            
            if(thisVert.y > maxY) maxY = thisVert.y;
            if(thisVert.y < minY) minY = thisVert.y;
            
            if(thisVert.z > maxZ) maxZ = thisVert.z;
            if(thisVert.z < minZ) minZ = thisVert.z;
            
        }
        
        //Print model sizing for debug
        //values listed above
        cout << "Min X: " << minX << endl;
        cout << "Max X: " << maxX << endl;
        cout << "Min Y: " << minY << endl;
        cout << "Max Y: " << maxY << endl;
        cout << "Min Z: " << minZ << endl;
        cout << "Max Z: " << maxZ << endl;
        
        cout << "Model Scale: " << modelScale << endl;
        cout << "Thickness: " << thickness << endl;
        cout << "Depth: " << depth << endl;
        cout << "Height: " << height << endl;

    }
    
    
    
    
    model.setScaleNormalization(false);
    model.setScale(1.0, 1.0, 1.0); //unnecessary

    
    //-----------------------------------------
    //    define modelScale and dimensions
    //-----------------------------------------
    
    //Since model Scale and dimensions are mostly hardcoded to give good values
    //bulkScale scales all of them equally to make minor adjustments
    float bulkScale = 1.0f;
    
    //Scale is figured out visually since AssImp Model does not give reliable sizing
    modelScale = 819.219 * bulkScale;
    
    currentScale = modelScale;
    
    
    //Real pixel dimensions determined visually at the scale above (819.219)
    //Short book
    //x = 24.2906, y= 111.599
    //Medium book
    //x = 24.2344, y= 124.109
    //Tall book
    //x = 18.401, y = 136.316
    
    
    if(bookType == 0) {
        
        height = 111.599;
        thickness = 24.2906;

        
    } else if(bookType == 1) {
        
        height = 124.10;
        thickness = 24.2906;
        
    } else {
        
        height = 136.316;
        thickness = 18.401;
        
    }

    //make display scale different depending on model goal is to have book fill shelf height, so
    //height * displayScale should be a little less than the shelf height (approx. 140)
    //so that the book isn't so big it overlaps with the buttons
    displayScale = modelScale * (135.0f/height);
    

    height *= bulkScale;
    thickness *= bulkScale;
    
    
    //depth is almost arbitrary since camera ortho is disabled
    //just pick a usable number so book pulls out of shelf properly
    depth = 100;

    
    //model positioned at bottom left corner of spine
    //values acquired visually since AssImp model does not give reliable size/scaling
    if(bookType == 0){
        model.setPosition(-0.0477083, -0.0147917, -0.0682016);
    } else if(bookType == 1){
        model.setPosition(-0.0477083, -0.0147917, -0.0757602);
    } else {
        model.setPosition(-0.0477083, -0.0147917, -0.0833189);
    }

    
    //--------------------BOOK TEXTURING--------------------
    
    textureFBO.allocate(tex -> getWidth(), tex -> getHeight());
    
    
    textureFBO.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    tex -> draw(0, 0);
    textureFBO.end();
    
    
    //create a mesh from the texture to make the
    //spine placeholder for when the book is inactive
    //Texture dimensions: 512px x 512px
    float texCoordX = 118.397; //position of spine in texture map
    float texCoordY = 13.254;
    float texWidth = 22.125;
    float texHeight = 189.917;
    spineMesh = tex -> getMeshForSubsection(0, -height, 0, thickness, height, texCoordX, texCoordY, texWidth, texHeight, true, OF_RECTMODE_CORNER);
    
    
    //--------------------Book Text and Page Layouts--------------------
    
    //Texture coordinages and dimensions of the page regions
    //within the book texture (tex = 512px x 512px)
    
    pageTexCoords.resize(6);
    
    
    pageTexCoords[0].set(261.299,  21.838);
    pageTexCoords[1].set(385.824,  21.838);
    pageTexCoords[2].set( 14.281, 216.057);
    pageTexCoords[3].set(140.605, 215.922);
    pageTexCoords[4].set(263.491, 215.922);
    pageTexCoords[5].set(386.88 , 215.419);
    
    pageWidth = 114.16;
    pageHeight = 186.273;

    
    //book tag list and coloring
    tagList.resize(9);
    tagList[0] = "Arts&Humanities";
    tagList[1] = "Health&Behavior";
    tagList[2] = "Psychology&Cognition";
    tagList[3] = "STEM";
    tagList[4] = "Literacy&Language";
    tagList[5] = "Curriculum";
    tagList[6] = "Assessment";
    tagList[7] = "Administration&Policy";
    tagList[8] = "Culture";
    
    bShowTaglet = false;
    bFadeOutTaglet = true;
    tagTrans = 0;

    
    //--------------------ANIMATION--------------------
    model.setLoopStateForAllAnimations(OF_LOOP_NONE);
    model.setPausedForAllAnimations(true);
    
    //don't play, we'll manually scrub through the animation
    //    model.playAllAnimations();
    
    bIsUnused = true;
    bIsActive = false;
    bIsDisplayed = false;
    
    //Rotation/translation variables to move book
    //from stored position to display position
    
    //initialize to stored position
    currentRotX = storedRotX;
    currentRotZ = storedRotZ;
    
    
    //pos is set by the Book Controller before setup runs

    
    animStartTime = 0.0;
    animPos = 0.0;
    
    
    currentOpenPage = 0;
    pageLerpSpeed = 0.05;
    
    //Stretch the book width a little to add more readable surface area
    //this will also inversely scale the text to return to normal
    widthScale = 1.0;
    flatScale = 0.5;
    flattenAmt = flatScale;
    
    //setup the buttons
    //nevermind, this is called from the book controller
//    setupUI();
    
}

void Book::setupUI(vector<ofImage> *_icons, vector<ofVec3f> shelfPts){
    
    icons = _icons;
    
    float shelfHeight = 140; //ish
    
    //Buttons are drawn relative to the book's position on the shelf (lower left corner)
    //draw the next button on top, then prev, then exit on bottom
    exitButton.setup( 0, displayPos, shelfPts, shelfNum, bookID );
    prevButton.setup( 1, displayPos, shelfPts, shelfNum, bookID );
    nextButton.setup( 2, displayPos, shelfPts, shelfNum, bookID );
    
    //set button icons
    exitButton.setIcons( &(*icons)[0], &(*icons)[1] );
    prevButton.setIcons( &(*icons)[2], &(*icons)[3] );
    nextButton.setIcons( &(*icons)[4], &(*icons)[5] );
    
    
    //tagButton does not use the third parameter
    //tag button will finish setting up in the setupContent() method
    tagButton.setup(3, displayPos, shelfPts, shelfNum, bookID );
    
}

void Book::showButtons(){

    nextButton.show();
    prevButton.show();
    exitButton.show();
    tagButton.show();
    
}

void Book::hideButtons(){
    
    nextButton.hide();
    prevButton.hide();
    exitButton.hide();
    tagButton.hide();
    
}

bool Book::checkButtonsForClicks(int x, int y, bool touchState){
    
    nextButton.checkForClicks(x, y, touchState);
    prevButton.checkForClicks(x, y, touchState);
    exitButton.checkForClicks(x, y, touchState);
    tagButton.checkForClicks(x, y, touchState);
    
}


void Book::setupContent(Contribution c){
    
    userContribution = c;

    
    //find out which tag we have
    for(int i = 0; i < tagList.size(); i++){
        if(userContribution.tag.compare(tagList[i]) == 0){
            tagNum = i;
        }
    }
    
    //set the color of this tag
    //split up the HSB spectrum based on how many tags we have
    int hueDiff = 255 / tagList.size();
    tagCol.setHsb( tagNum * hueDiff, 165, 255);
    
//    cout << "Book[" << bookID <<"] Tag #" << tagNum << ", Color: " << tagCol << endl;
    
    //setup the tag button
    tagButton.setTag(userContribution.tag, tagNum, tagCol, UIFont);
    
    
    formatTextForDisplay();
    drawContentToTexture();
    
}

void Book::formatTextForDisplay(){
    
    //Go through the userContribution text and split it up
    //into different pages based on line length and number
    //of lines on page. Do checks to see if a whole word fits
    //on a line, if not drop it to the next line by adding a '\n'

    pageText.clear();
    
    //prepare the total string: intro to author, name, ***, user message
    //the *** will make a page break in the text wrapping code below
    string total;
    total = "Contribution Authored by: " + userContribution.name + " *** " + userContribution.message;
    
    
    
    //split the whole string into a vector of words
    vector<string> words = ofSplitString(total, " ");
    
    const int numCharsPerLine = 15;
    const int linesPerPage = 8;
    
    int currentLine = 0;
    int charsLeftInLine = numCharsPerLine;
    
    string textThisPage = "";
    
    for(int i = 0; i < words.size(); i++){
        
        //first check for the page break between the
        //author page and rest of content
        if( words[i].compare("***") == 0 ) {
            
            pageText.push_back(textThisPage);
            
            //Do not add *** to page, just make text blank
            textThisPage = "";
            currentLine = 0;
            charsLeftInLine = numCharsPerLine;
            
            
        } else {
            
            //check if the number of characters left in the current line
            //is greater than or equal to the current word, add it
            if(words[i].length() <= charsLeftInLine){
                
                //add the word
                textThisPage += words[i];
                charsLeftInLine -= words[i].length();
                
                //if there's no more room on the line...
            } else {
                
                //and we're not at the bottom of the page
                if(currentLine < linesPerPage){
                    
                    //go to next line
                    textThisPage += "\n";
                    currentLine++;
                    charsLeftInLine = numCharsPerLine;
                    
                    //add the word
                    textThisPage += words[i];
                    charsLeftInLine -= words[i].length();
                    
                    //if we ARE at the bottom of the page...
                } else {
                    
                    //add it to the pageText vector
                    pageText.push_back(textThisPage);
                    
                    //add word to the fresh page
                    textThisPage = words[i];
                    currentLine = 0;
                    charsLeftInLine -= words[i].length();
                    
                }
                
            }
            
            //add a space
            textThisPage += " ";
            charsLeftInLine--;
            
            //if thats the end of this line
            //go to the next line
            if(charsLeftInLine <= 0){
                textThisPage += "\n";
                currentLine++;
                charsLeftInLine = numCharsPerLine;
            }
            
            //if we're on the last word but havent reached the end of the page yet,
            //add the text to the vector
            if(i == words.size() - 1){
                pageText.push_back(textThisPage);
            }
        }
    }
    
//    for(int i = 0; i < pageText.size(); i++){
//        cout << "Page: " << i << endl;
//        cout << pageText[i] << endl;
//        cout << "\n" << endl;
//    }
    
    //how many pages do we have open to us
    if(pageText.size() <= 2){
        pageSpreadsAvailable = 1;
    } else if( pageText.size() <= 4){
        pageSpreadsAvailable = 2;
    } else {
        pageSpreadsAvailable = 3;
    }
    


    
}

void Book::drawContentToTexture(){
    
    //-----DRAW TEXT ONTO TEXTURE-----
    
    textureFBO.begin();
    
    //clear the FBO from the last frame
    ofClear(255, 255, 255, 255);
    
    //draw the base texture first
    ofSetColor(255);
    tex -> draw(0, 0);
    
//    ofDisableDepthTest();
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
        font -> drawString(pageText[i], 0, 0); //-lineHeight);
        
        ofPopMatrix();
        
    }
//    ofEnableDepthTest();
    
    textureFBO.end();

}


void Book::triggerDisplay(){
    
    bIsActive = true;
    bIsDisplayed = false;
    bIsClosing = false;
    
    animStartTime = ofGetElapsedTimef();
    
}

void Book::update(){
    

    
    
    
    if(bIsActive){
        
        //update buttons
        nextButton.update();
        prevButton.update();
        exitButton.update();
        tagButton.update();
        
        model.update();

//        drawContentToTexture();

        if(!bIsDisplayed){

            //ANIMATION TO PULL BOOK OUT
            double now = ofGetElapsedTimef();

            //book is pulled out of shelf
            float pullOutTime = animStartTime + 0.7f;

            //book is in flat display position
            float displayReadyTime = pullOutTime + 1.5f;

            //Book is flipped to first open pages
            float firstPageTime = displayReadyTime + 2.0f;

            if(now < pullOutTime){

                //Z axis: "easeOut" starts fast and slows to finish
                //PULL OUT of shelf
                float pctZ = ofxeasing::map_clamp(now, animStartTime, pullOutTime, 0.0f, 1.0f, &ofxeasing::quad::easeOut);

                //X axis: "easeIn" starts slow and finishes fast
                //move book to the LEFT
                float pctX = ofxeasing::map_clamp(now, animStartTime, pullOutTime, 0.0f, 1.0f, &ofxeasing::linear::easeIn);

                pos.z = ofLerp(storedPos.z, pulledOutPos.z, pctZ);
                pos.x = ofLerp(storedPos.x, pulledOutPos.x, pctX);

                currentRotZ = ofLerp(storedRotZ, displayRotZ, pctX);

            } else if(now < displayReadyTime){


                float pct = ofxeasing::map_clamp(now, pullOutTime, displayReadyTime, 0, 1.0, &ofxeasing::cubic::easeOut);

                //ease position and angle
                pos = pulledOutPos.getInterpolated(displayPos, pct);

                currentScale = ofLerp(modelScale, displayScale, pct);
                

                

            } else if(now < firstPageTime){

                //do a linear mapping of the animation and clamp it
                animPos = ofMap(now, displayReadyTime, firstPageTime, animationStart, animationSpread1, true);

                //also ease into the flattened scale to make the text
                //close to the spine easier to read
                flattenAmt = ofMap(now, displayReadyTime, firstPageTime, 1.0, flatScale);
                
                //no more timers/animations to set, just do these last things before
                //going to the book displayed state
                
                bIsDisplayed = true;
                
                //bring out the buttons
                showButtons();
                
                //prepare the NEXT state to pick up where this one leaves off
                targetAnimPos = animPos;
                currentOpenPage = 0;
                
            }
        
        } else {


            
            
            //the book is now open and ready!
            //i.e. bIsDisplayed is true
            
            //if we're not closing, check for user buttons
            if(!bIsClosing){
                

                
                bool exiting = false;
                
                //check to see if buttons are pressed
                //check exit first, since this one overrides the others
                if(exitButton.buttonState){
                    
                    exiting = true;
                    exitButton.buttonState = false;
                    
                    prevButton.bIsUnavailable = true;
                    nextButton.bIsUnavailable = true;
                    
                }
                
                if(!exiting){
                    
                    //make buttons available or unavailable
                    if(currentOpenPage == 0){
                        prevButton.bIsUnavailable = true;
                    } else {
                        prevButton.bIsUnavailable = false;
                    }
                    
                    //if we're on the last available spread
                    if(currentOpenPage == pageSpreadsAvailable - 1){
                        nextButton.bIsUnavailable = true;
                    } else {
                        nextButton.bIsUnavailable = false;
                    }
                    
                    
                    
                    //check buttons but make sure we don't go too far
                    //so check how many pages are available
                    //and handle button unavailable states
                    if(nextButton.buttonState && !nextButton.bIsUnavailable){
                        
                        //move forward
                        currentOpenPage++;
                        nextButton.buttonState = false;
                        
                    } else if(prevButton.buttonState && !prevButton.bIsUnavailable){
                        
                        //move back
                        currentOpenPage--;
                        prevButton.buttonState = false;
                    }
                    
                    //get the right animation position for the action
                    if(currentOpenPage == 0){
                        targetAnimPos = animationSpread1;
                    } else if(currentOpenPage == 1){
                        targetAnimPos = animationSpread2;
                    } else if(currentOpenPage == 2){
                        targetAnimPos = animationSpread3;
                    }
                    
                } else {
                    
                    //close book by going backwards to the beginning
                    targetAnimPos = animationStart;
                    
                    //if we're at the end (more or less)
                    if(targetAnimPos < 0.001){
                        
                        targetAnimPos = 0;
                        
                        bIsClosing = true;
                        closingStartTime = ofGetElapsedTimef();
                        
                    }
                    
                    //put away the buttons
                    hideButtons();
                    
                }
                
                //then lerp to that position
                animPos = ofLerp(animPos, targetAnimPos, pageLerpSpeed);
                
            } else {
                
                //we're closing, go through the animation of putting the book back
                double now = ofGetElapsedTimef();
                
                
                //Book snaps closed
                float closeBookTime = closingStartTime + 1.7f;
                
                //book goes back to pulled out position
                float backToPulledOutTime = closeBookTime + 1.5f;
                
                //Book is inserted back into shelf
                float putBackTime = backToPulledOutTime + 1.0f;
                
                if(now < closeBookTime){
                    
                    //do a linear mapping of the animation and clamp it
                    //start at wherever we are and close the book by moving to the start position
                    animPos = ofMap(now, closingStartTime, closeBookTime, animPos, animationStart, true);
                    flattenAmt = ofMap(now, closingStartTime, closeBookTime, flatScale, 1.0f);
                    

                    
                } else if(now < backToPulledOutTime){
                    
                    //book is now closed, animate back to pulled out pos and stored angles
                    float pct = ofxeasing::map_clamp(now, closeBookTime, backToPulledOutTime, 0.0, 1.0, &ofxeasing::back::easeOut_s, 0.5);
                    
                    //ease position and angle
                    pos = displayPos.getInterpolated(pulledOutPos, pct);
                    
                    currentScale = ofLerp(displayScale, modelScale, pct);
                    
                } else if(now < putBackTime + 0.1f){
                    //add in a tenth of a second to allow books to finish last
                    //bit of interpolation
                    
                    
                    //Z axis: "easeOut" starts fast and slows to finish
                    float pctZ = ofxeasing::map_clamp(now, backToPulledOutTime, putBackTime, 0.0f, 1.0f, &ofxeasing::linear::easeIn);
                    
                    //X axis: "easeIn" starts slow and finishes fast
                    float pctX = ofxeasing::map_clamp(now, backToPulledOutTime, putBackTime, 0.0f, 1.0f, &ofxeasing::linear::easeOut);
                    
                    pos.z = ofLerp(pulledOutPos.z, storedPos.z, pctZ);
                    pos.x = ofLerp(pulledOutPos.x, storedPos.x, pctX);
                    
                    currentRotZ = ofLerp(displayRotZ, storedRotZ, pctX);
                    
                    
                } else {
                    
                    //The book is now back stored back in the shelf
                    //mark as inactive
                    bIsDisplayed = false;
                    bIsActive = false;
                    
                }
                
                
            }
            
        }
        
        
        
        
        model.setPositionForAllAnimations(animPos);
        
        
        

        
    } else {
        //bIsActive = false:
        
        //if we're showing the taglet, fade it in
        //if we're not, fade it out
        if(bShowTaglet){

            //fade the color in if we're showing the tag
            tagTrans = ofLerp(tagTrans, 255, 0.075);
            
            //if enough time has passed put the taglet away
            if(ofGetElapsedTimef() - tagletStartTime > 5.0f){
                
                bShowTaglet = false;

                //and prepare to fade it out when it becomes hidden again
                bFadeOutTaglet = true;
            }
            
            
        } else {
            
            //if we're not showing but still fading out...
            if(bFadeOutTaglet){
                
                //lerp till it's transparent
                tagTrans = ofLerp(tagTrans, -1, 0.1);
                
                //if we're done fading, turn it off to save a few cpu cycles
                if(tagTrans < 0){
                    bFadeOutTaglet = false;
                }
                
            }
            
        }

        
    }
    
    
}

void Book::showTaglet(){
    
    tagletStartTime = ofGetElapsedTimef();
    bShowTaglet = true;
    
}


void Book::draw(){



    
        if(bIsActive){
            
            ofPushMatrix();
                
            //translate the book to the position on the shelf
            //with the bottom left corner of the spine at pos
            //also push it forward so it clears the rotating wallpaper tiles
            ofTranslate(pos);
            
            
            //        float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), -90, 90);
            //        float z = ofMap(ofGetMouseY(), 0, ofGetHeight(), -90, 90);
            //        cout << "XZ: " << x << ", " << z << endl;
            

            //now rotate and translate book so that it has the lower left corner of the spine
            //as its origin and is oriented properly: book stored in shelf
            ofRotateX(currentRotX);
            ofRotateZ(currentRotZ);
            
            
            
            //scale the model up (with a little extra in the X to add more viewable reading area.
            //Also, squash the book down to minimize the curvature
            //making text close to the spine easier to read
            ofScale(currentScale * widthScale, currentScale * flattenAmt, currentScale);
            
            //disable the model textures so we can use our own
            model.disableTextures();
            ofDisableArbTex();
            if(textureFBO.isAllocated()) textureFBO.getTexture().bind();
            
            ofSetColor(255);
            model.drawFaces();
            
            if(textureFBO.isAllocated()) textureFBO.getTexture().unbind();

            ofPopMatrix();

            //draw buttons
            nextButton.draw();
            prevButton.draw();
            exitButton.draw();
            tagButton.draw();
            

            
        } else {
            
            //draw inactive placeholder (an image of the spine
            
            ofPushMatrix();
                
            //translate the book to the position on the shelf
            //with the bottom left corner of the spine at pos
            ofTranslate(pos);
                
            if(tex -> isAllocated()) tex -> bind();
            ofSetColor(255);
            spineMesh.draw();
            if(tex -> isAllocated()) tex -> unbind();
            


            
            //only draw if we're showing if intentionally
            //or it is being faded out
            if(bShowTaglet || bFadeOutTaglet){
                float amp = 7.5;
                float shelfHeight = 145;
                float yHeight = -shelfHeight + amp * sin( ofGetElapsedTimef() * 3.2 );
                float triHeight = 20;
                ofSetColor(tagCol, tagTrans);
                int z = -5;
                ofDrawTriangle(0, yHeight, z, thickness, yHeight, z, thickness/2.0, yHeight + triHeight, z);
            }
            
            ofPopMatrix();
            
        }
        
        
    
    
}




