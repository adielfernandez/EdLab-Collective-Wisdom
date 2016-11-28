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


void Book::setup(ofTexture *_tex, ofTrueTypeFont *_font){

    tex = _tex;
    font = _font;
    
    
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
    displayScale = modelScale * 1.25;
    
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
        height = 124.109;
        thickness = 24.2906;
    } else {
        height = 136.316;
        thickness = 18.401;
    }

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
    
    cout << "TEX WIDTH: " <<tex -> getWidth() << endl;
    
    textureFBO.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    tex -> draw(0, 0);
    textureFBO.end();
    
    
    
    //resize all the texCoords for a 500x500 texture
    float texScaleDown = 1.0f;
    
    
    
    //create a mesh from the texture to make the
    //spine placeholder for when the book is inactive
    //Texture dimensions: 1000px x 1000px
    float texCoordX = 230.225 * texScaleDown; //position of spine in texture map
    float texCoordY = 25.027 * texScaleDown;
    float texWidth = 45.75 * texScaleDown;
    float texHeight = 371.833 * texScaleDown;
    spineMesh = tex -> getMeshForSubsection(0, -height, 0, thickness, height, texCoordX, texCoordY, texWidth, texHeight, true, OF_RECTMODE_CORNER);
    
    
    //--------------------Book Text and Page Layouts--------------------
    
    //Texture coordinages and dimensions of the page regions
    //within the book texture (tex = 1000px x 1000px)
    
    pageTexCoords.resize(6);
    
    pageTexCoords[0].set(510.354, 42.588);
    pageTexCoords[1].set(754.459, 42.588);
    pageTexCoords[2].set(  28.292, 422.088);
    pageTexCoords[3].set( 272.959, 422.088);
    pageTexCoords[4].set(513.625, 422.088);
    pageTexCoords[5].set(753.959, 422.088);
    
    for(int i = 0; i < pageTexCoords.size(); i++){
        pageTexCoords[i] *= texScaleDown;
    }
    
    
    pageWidth = 223.105 * texScaleDown;
    pageHeight = 363.878 * texScaleDown;


    
    //--------------------ANIMATION--------------------
    model.setLoopStateForAllAnimations(OF_LOOP_NONE);
    model.setPausedForAllAnimations(true);
    
    //don't play, we'll manually scrub through the animation
    //    model.playAllAnimations();
    
    bIsUnused = true;
    bIsActive = true;
    bIsAnimating = false;
    
    //Rotation/translation variables to move book
    //from stored position to display position
    
    //initialize to stored position
    currentRotX = storedRotX;
    currentRotZ = storedRotZ;
    
    
    //pos is set by the Book Controller before setup runs

    
    animStartTime = 0.0;
    animPos = 0.0;
    
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
    
}


void Book::triggerDisplay(){
    
    bIsActive = true;
    bIsAnimating = true;
    
    animStartTime = ofGetElapsedTimef();
    
}

void Book::update(){


    
    
    if(bIsActive){
        model.update();
        
        if(bIsAnimating){
            
            //----------------------------------------------------------//
            //--------------------ANIMATION SEQUENCE--------------------//
            //----------------------------------------------------------//
            double now = ofGetElapsedTimef();
            
            //book is pulled out of shelf
            float pullOutTime = animStartTime + 0.7f;
            
            //book is in flat display position
            float displayReadyTime = pullOutTime + 1.5f;
            
            //Book is flipped to first open pages
            float firstPageTime = displayReadyTime + 2.0f;
            
            float bookWait = firstPageTime + 20.0f; //wait with book open
            
            //Book is flipped to second open pages
            float secondPageTime = bookWait + 1.0f;
            
            //Book snaps closed
            float closeBookTime = secondPageTime + 1.0f;
            
            //book goes back to pulled out position
            float backToPulledOutTime = closeBookTime + 1.5f;
            
            //Book is inserted back into shelf
            float putBackTime = backToPulledOutTime + 1.0f;
            
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
                animPos = ofMap(now, displayReadyTime, firstPageTime, animationStart, animFirstPages, true);
            
            } else if(now < bookWait){
                
                animPos = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0.0, 1.0);
                
            } else if(now < secondPageTime){
                
                //do a linear mapping of the animation and clamp it
                animPos = ofMap(now, firstPageTime, secondPageTime, animFirstPages, animSecondPages, true);
                
            } else if(now < closeBookTime){
                
                //do a linear mapping of the animation and clamp it
                animPos = ofMap(now, secondPageTime, closeBookTime, animSecondPages, animationEnd, true);
                
            } else if(now < backToPulledOutTime){
                
                //book is now closed, animate back to pulled out pos and stored angles
                animPos = animationStart;
                
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
                bIsAnimating = false;
                bIsActive = false;
            }
            
            model.setPositionForAllAnimations(animPos);
            
            
            //-----DRAW TEXT ONTO TEXTURE
            textureFBO.begin();
            ofSetColor(255);
            tex -> draw(0, 0);
            ofDisableDepthTest();
            //go through all the pages and draw them out
            for(int i = 0; i < pageText.size(); i++){
                
                //if for some reason there is too much text (freak bug)
                //hard limit to 6 pages
                if(i < pageTexCoords.size()){
                    
                    ofVec2f textPos;
                    textPos = pageTexCoords[i];
                    float pageMargin = 15;
                    float lineHeight = font -> stringHeight("A");
                    textPos += ofVec2f(pageMargin, pageMargin + lineHeight);
                    
                    ofSetColor(0);
                    font -> drawString(pageText[i], textPos.x, textPos.y);
                    
                }
                
            }
            ofEnableDepthTest();
//            ofSetColor(0, 128, 255);
//            ofDrawRectangle(0, 0, textureFBO.getWidth(), textureFBO.getHeight());
            
            textureFBO.end();
            
            
            
        
        } else {
            
            //if we're not displaying
            
            
        } //close bIsAnimating else loop
        
    } //close bIsActive loop
    
    
    
    
}

void Book::draw(){

    
    ofPushMatrix();{
        
        
        //translate the book to the position on the shelf
        //with the bottom left corner of the spine at pos
        ofTranslate(pos);
        
//        //draw axes and scale up so we can position things visually more precisely
//        //See "model.setPosition(...)" in setup
//        ofTranslate(0, 300, 0);
//        ofSetLineWidth(1);
//        ofDrawAxis(100);
//        ofScale(3, 3, 3);
//        
//        
//        float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), 17, 27);
//        float y = ofMap(ofGetMouseY(), 0, ofGetHeight(), 100, 150);
//        cout << x << ", " << y << endl;
//        ofSetColor(255, 200, 0);
//        ofDrawRectangle(0, 0, x, -y);
        

        
        if(bIsActive){
            
            //with the bottom left corner of the spine at pos
//            ofTranslate(thickness/2, -height/2, depth/2);
            
            //        float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), -90, 90);
            //        float z = ofMap(ofGetMouseY(), 0, ofGetHeight(), -90, 90);
            //        cout << "XZ: " << x << ", " << z << endl;
            
            //now rotate and translate book so that it has the lower left corner of the spine
            //as its origin and is oriented properly: book stored in shelf
            ofRotateX(currentRotX);
            ofRotateZ(currentRotZ);
            
            
            
            //scale the model up
            ofScale(currentScale, currentScale, currentScale);
            
            //disable the model textures so we can use our own
            model.disableTextures();
            if(textureFBO.isAllocated()) textureFBO.getTexture().bind();
            
            ofSetColor(255);
            model.drawFaces();
            
            if(textureFBO.isAllocated()) textureFBO.getTexture().unbind();

            
        } else {
            
            //draw inactive placeholder (an image of the spine
            
            
            if(tex -> isAllocated()) tex -> bind();
            ofSetColor(255);
            spineMesh.draw();
            if(tex -> isAllocated()) tex -> unbind();

            
        }
        
        
    }ofPopMatrix();
    
    
}



