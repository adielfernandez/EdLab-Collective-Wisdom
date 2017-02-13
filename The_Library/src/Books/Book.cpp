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
    }
    
//    else {
//        model.loadModel("books/bookTall.fbx", false);
//    }

    

    
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
    //But this number was aquired visually as well
    depth = 77.0833;

    
    //model positioning (intrinsic to AssImp model) to make bottom
    //left corner of spine the model's origin. Values acquired visually
    //since AssImp model does not give reliable size/scaling.
    const float modelX = -0.0477083;
    const float modelY = -0.0147917;
    
    //3 Book models have different heights
    const float shortModelZ = -0.0682016;
    const float mediumModelZ = -0.0757602;
    const float tallModelZ = -0.0833189;
    
    if(bookType == 0){
        model.setPosition(modelX, modelY, shortModelZ);
    } else if(bookType == 1){
        model.setPosition(modelX, modelY, mediumModelZ);
    } else {
        model.setPosition(modelX, modelY, tallModelZ);
    }

    
    //--------------------BOOK TEXTURING--------------------
    
    textureFBO.allocate(tex -> getWidth(), tex -> getHeight());
    
    
    textureFBO.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    tex -> draw(0, 0);
    textureFBO.end();
    
    
    //Get a texture for the spine that has the width/height we need
    //but remapped from the texture. To do this, we'll get a subsection
    //of the texture as a mesh, create an FBO, draw the mesh into the FBO then steal THAT texture.
    spineMesh = tex -> getMeshForSubsection(0, -height, -1, thickness, height, spineTexCoordX, spineTexCoordY, spineTexWidth, spineTexHeight, true, OF_RECTMODE_CORNER);

    
    
    
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
//    tagList.resize(10);
//    tagList[0] = "Arts&Humanities";
//    tagList[1] = "Health&Behavior";
//    tagList[2] = "Psychology&Cognition";
//    tagList[3] = "STEM";
//    tagList[4] = "Literacy&Language";
//    tagList[5] = "Curriculum";
//    tagList[6] = "Assessment";
//    tagList[7] = "Administration&Policy";
//    tagList[8] = "Culture";
//    tagList[9] = "Other Stuff";
    

    
    bShowTaglet = false;
    bFadeOutTaglet = true;
    tagTrans = 0;
    tagletDuration = 6.0f;
    spineTrans = 0;

    
    //--------------------ANIMATION--------------------
    model.setLoopStateForAllAnimations(OF_LOOP_NONE);
    model.setPausedForAllAnimations(true);
    
    //disable the model textures so we can use our own
    model.disableTextures();
    

    
    //don't play, we'll manually scrub through the animation
    //    model.playAllAnimations();
    
    bIsUnused = true;
    bIsActive = false;
    bIsInShelf = false;
    bIsDisplayed = false;
    bIsNewBookEvt = false;
    
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
    
    //start flatness out as 1.0
    flattenAmt = 1.0;
    

    //spawning effect
    spawnDuration = 4.0f;
    spawnPosBackEasing = 1.0f;
    
}

void Book::setLocation(ofVec3f stored, ofVec3f display, int sNum, int bNum){
    
    
    
    storedPos = stored;
    displayPos = display;
    
    shelfNum = sNum;
    bookNum = bNum;
    
    float sideShift = -100;
    pulledOutPos = stored + ofVec3f(sideShift, 0, -depth);
    
    //book spawns off screen directly above it's stored position
    spawnPos.set(storedPos.x, - 100, storedPos.z);
    
    pos = storedPos;
    
    
}

void Book::setupUI(vector<ofImage> *_icons, vector<ofVec3f> shelfPts){
    
    icons = _icons;
    
    float shelfHeight = 140; //ish
    
    //Buttons are drawn relative to the book's position on the shelf (lower left corner)
    //draw the next button on top, then prev, then exit on bottom
    exitButton.setup( 0, displayPos);
    prevButton.setup( 1, displayPos);
    nextButton.setup( 2, displayPos);
    
    exitButton.setLibraryInfo(shelfPts, shelfNum, bookNum);
    prevButton.setLibraryInfo(shelfPts, shelfNum, bookNum);
    nextButton.setLibraryInfo(shelfPts, shelfNum, bookNum);
    tagButton.setLibraryInfo(shelfPts, shelfNum, bookNum);
    
    //set button icons
    exitButton.setIcons( &(*icons)[0], &(*icons)[1] );
    prevButton.setIcons( &(*icons)[2], &(*icons)[3] );
    nextButton.setIcons( &(*icons)[4], &(*icons)[5] );
    
    //tag button will finish setting up in the setupContent() method
    tagButton.setup(3, displayPos);

    
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

void Book::setButtonSpeeds(float lerpSpeed){
    
    nextButton.buttonLerpSpeed = lerpSpeed;
    prevButton.buttonLerpSpeed = lerpSpeed;
    exitButton.buttonLerpSpeed = lerpSpeed;
    tagButton.buttonLerpSpeed = lerpSpeed;
    
}

bool Book::checkButtonsForClicks(int x, int y, bool touchState){
    
    nextButton.checkForClicks(x, y, touchState);
    prevButton.checkForClicks(x, y, touchState);
    exitButton.checkForClicks(x, y, touchState);
    tagButton.checkForClicks(x, y, touchState);
    
}


void Book::setupContent(Contribution c, string tagText1, string tagText2, int _tagNum, ofColor _tagCol){
    
    userContribution = c;
    tagNum = _tagNum;
    tagCol = _tagCol;
    
    //set whether or not the book is a scholar book and what scholar it is
    bIsScholarBook = userContribution.bIsScholar;
    scholarNum = userContribution.scholarNum;
        
    
    //mark this contribution as not archived
    userContribution.bIsArchived = false;
    
    //setup the tag button
    tagButton.setFont( UIFont );
    tagButton.setTag( tagText1, tagText2, tagNum, tagCol);
    
    formatTextForDisplay();
    drawContentToTexture();
    
    //set up the spawning effect now that we have a color
    spawnRibbons.setup(tagCol);
    
}

//only called on books that are placeed in shelf
//at program start, without an animation
void Book::putInShelf(){
    
    bIsInShelf = true;
    spineTrans = 255;
    
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
    bIsInShelf = false;
    bIsDisplayed = false;
    bIsClosing = false;
    
    animStartTime = ofGetElapsedTimef();
    
}

void Book::triggerNewBookEvt(){
    
    bIsNewBookEvt = true;
    bIsUnused = false;

    bIsActive = false;
    
    bIsInShelf = true;
    
    animStartTime = ofGetElapsedTimef();
    
    
    spawnAngleZ = 360;
    spawnAngleX = 90;
    
    //start the book at the fully open position so
    //we can animate it to the beginning
    animPos = animationSpread3;
    
    //reset the spawn effect ribbons
    spawnRibbons.reset();
    
}

void Book::update(){
    


    if( bIsNewBookEvt ){
        
        //if the book is already in the shelf we need to fade it out
        //then animate it in, otherwise the book is bIsUnused=true and ready to be used
        
        float fadeOutOffsetTime = 0;
        
        if( bIsInShelf ){
            
            //if the book is not already transparent, fade it out before moving on
            if( spineTrans != 0 ){
                
                //The book is already in the shelf but inactive
                double now = ofGetElapsedTimef();
                
                float fadeOutTime = animStartTime + 1.0f;
                
                spineTrans = ofxeasing::map_clamp(now, animStartTime, fadeOutTime, 255, 0, &ofxeasing::linear::easeOut);
                
                
                
            } else {
                
                //if we've disappeared we're ready to spawn
                //set the book to the spawn position
                bIsInShelf = false;
                bIsActive = true;
                
                pos = spawnPos;
                
                //this will center the book on it's axis so it rotates
                //nicer during the animation
                toCenter.set(depth/2, thickness/2, height/2);
                
                //reset the animation start time to now so
                //the next step animates correctly
                animStartTime = ofGetElapsedTimef();
                
                //setup the spawnRibbons
                
                

            }
            
        } else {
            
            //Book is not in the shelf so it's a new book coming in.
            //animate the book into the scene
            

            double now = ofGetElapsedTimef();
            
            //The time we'll get from spawn position to the pulled out position
            float spawnEndTime = animStartTime + spawnDuration;

            //update the spawn effect and send it a countdown
            //until the effect is over
            spawnRibbons.update( spawnEndTime - now );

            
            if(now < spawnEndTime){
                
                
                //book is now closed, animate back to pulled out position
                float backEasePct = ofxeasing::map_clamp(now, animStartTime, spawnEndTime, 0.0, 1.0, &ofxeasing::back::easeOut_s, spawnPosBackEasing);
                
                //interpolate to the stored position but compensate for the book center shift
                ofVec3f backToCenter(thickness/2, -height/2, depth/2);
                
                pos = spawnPos.getInterpolated(storedPos + backToCenter, backEasePct);
                
                //start at a large rot angle so the book spins into position on the way down
                float anglePct = ofxeasing::map_clamp(now, animStartTime, spawnEndTime, 0.0, 1.0, &ofxeasing::cubic::easeOut);
                currentRotZ = ofLerp(spawnAngleZ, storedRotZ, anglePct);
                currentRotX = ofLerp(spawnAngleX, storedRotX, anglePct);

                
                //force the book forward so it clears the shelf while animating
                pos.z = -200;
                
                
                //if we want to open the book during the fly in:
//                //animation needs to end a tad earlier so book is closed when it goes into the shelf
//                double animationEndTime = pulledOutPosTime - 0.5f;
//                float animPct = ofxeasing::map_clamp(now, animStartTime, animationEndTime, 0.0, 1.0, &ofxeasing::linear::easeOut);
//                animPos = ofLerp(animationSpread3, animationStart, animPct);
//                
//                model.update();
//                model.setPositionForAllAnimations(animPos);
                
                
            } else {
                
                //put the book back at the correct Z
                pos = storedPos;
                
                //animation is finished
                bIsNewBookEvt = false;
                bIsActive = false;
                bIsDisplayed = false;
                
                bIsInShelf = true;
                spineTrans = 255;
                
                
            }
            
            


            
        }
        
        
        
        
        
    }
    
    
    //update if we're active but NOT in a new book spawning event
    if( bIsActive && !bIsNewBookEvt ){
        
        //update buttons
        nextButton.update();
        prevButton.update();
        exitButton.update();
        tagButton.update();
        


        // bIsDisplayed: runs through book being pulled out to display
        // position and being put back in the shelf
        if(!bIsDisplayed){

            //ANIMATION TO PULL BOOK OUT
            double now = ofGetElapsedTimef();

            //set up the timing of each segment as a percentage
            //of one value: total open time. These all add up to 1.0
            //(numbers found experimentally)
            float pullOutPct = 0.166666667;
            float displayPct = 0.357142857;
            float firstPagePct = 0.476190476;
            
            //book is pulled out of shelf
            float pullOutTime = animStartTime + pullOutPct * totalBookOpenTime;

            //book is in flat display position
            float displayReadyTime = pullOutTime + displayPct * totalBookOpenTime;

            //Book is flipped to first open pages
            float firstPageTime = displayReadyTime + firstPagePct * totalBookOpenTime;

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
                
                //bring out the buttons
                showButtons();
                
            } else {
                
                //no more timers/animations to set, just do these last things before
                //going to the book displayed state
                bIsDisplayed = true;
                
                
                //prepare the NEXT state to pick up where this one leaves off
                targetAnimPos = animPos;
                currentOpenPage = 0;
                
            }
            
            //Only update the model after we're in display position
            if(now > displayReadyTime){
                bNeedsUpdate = true;
            } else {
                bNeedsUpdate = false;
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
                    
                    
                    //This is the page flipping segment. Only update if
                    //we're not at our desired page
                    if( abs( animPos - targetAnimPos ) > 0.01){
                        
                        bNeedsUpdate = true;
                        
                        //then lerp to that position
                        animPos = ofLerp(animPos, targetAnimPos, pageLerpSpeed);
                        
                    } else {
                        bNeedsUpdate = false;
                    }
                    
                    
                } else {
                    
                    //we're exiting, tell the book to close
                    bIsClosing = true;
                    
                    //note the start time of the closing animation
                    closingStartTime = ofGetElapsedTimef();
                    
                    //put away the buttons
                    hideButtons();
                    
                }
                

                
                
                
            } else {
                
                //we're closing, go through the animation of putting the book back
                double now = ofGetElapsedTimef();
                
                //animation segments defined as percentage of
                //total book close time. these all add up to 1.0
                float closeTimePct = 0.404761905;
                float backToPulledPct = 0.357142857;
                float putBackPct = 0.238095238;
                
                //Book snaps closed
                float closeBookTime = closingStartTime + closeTimePct * totalBookCloseTime;
                
                //book goes back to pulled out position
                float backToPulledOutTime = closeBookTime + backToPulledPct * totalBookCloseTime;
                
                //Book is inserted back into shelf
                float putBackTime = backToPulledOutTime + putBackPct * totalBookCloseTime;
                
                //update the model for each of the animation steps to close the book
                bNeedsUpdate = true;
                
                if(now < closeBookTime){
                    
                    //do a linear mapping of the animation and clamp it
                    //start at wherever we are and close the book by moving to the start position
                    animPos = ofMap(now, closingStartTime, closeBookTime, animPos, animationStart, true);
                    flattenAmt = ofMap(now, closingStartTime, closeBookTime, flatScale, 1.0f);
                    

                    
                } else if(now < backToPulledOutTime){
                    
                    //from this point on the book is closed and doesn't need to be updated
                    bNeedsUpdate = false;
                    
                    
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
                    bIsInShelf = true;
                    
                }
                
                
            }
            
        }
        
        if( bNeedsUpdate ){
            model.setPositionForAllAnimations(animPos);
            model.update();
        }
        
        
    }
    
    
    if( bIsInShelf ){
        
        //if we're showing the taglet, fade it in
        //if we're not, fade it out
        if(bShowTaglet){

            //fade the color in if we're showing the tag
            tagTrans = ofLerp(tagTrans, 255, 0.075);
            
            //if enough time has passed put the taglet away
            if(ofGetElapsedTimef() - tagletStartTime > tagletDuration){
                
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
        
        //debug tools using the mouse
        
        
        //now rotate book to orient it properly depending on the state
        //See constants: displayRotX, storedRotX, etc.
        ofRotateX(currentRotX);
        ofRotateZ(currentRotZ);
        
        
        //if we're in the book spawning sequence, move the book so that
        //it draws from it's center to make the rotations look less awkward
        if(bIsNewBookEvt){
            spawnRibbons.draw();
            
            ofPushMatrix();
            ofTranslate(toCenter);
            
        }
        
        //scale the model up (with a little extra in the X to add more viewable reading area.
        //Also, squash the book down to minimize the curvature
        //making text close to the spine easier to read
        ofScale(currentScale * widthScale, currentScale * flattenAmt, currentScale);
        
        
        if(textureFBO.isAllocated()) textureFBO.getTexture().bind();
        
        ofSetColor(255);
        model.drawFaces();
        
        if(textureFBO.isAllocated()) textureFBO.getTexture().unbind();
        
        //close out the spawning event translation
        if( bIsNewBookEvt ) ofPopMatrix();
        
        //draw an axis for debugging
//        ofSetLineWidth(2);
//        ofDrawAxis(100);
        
        
        ofPopMatrix();
        
        //draw buttons
        nextButton.draw();
        prevButton.draw();
        exitButton.draw();
        tagButton.draw();
        
        
        
    }
    
    
    if( bIsInShelf ){
        
        //placeholder image of the spine when the book is in the shelf
        
        ofPushMatrix();
        
        //translate the book to the position on the shelf
        //with the bottom left corner of the spine at pos
        ofTranslate(pos);

        if(tex -> isAllocated()) tex -> bind();
        
        ofSetColor(255, spineTrans);
        spineMesh.draw();
        if(tex -> isAllocated()) tex -> unbind();
        
        
        // ----- Draw Taglet -----
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




