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

void Book::loadModel(int bType, int tType){

    bookType = bType;
    texType = tType;
    
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
    
    

    
    //Start working with model
    model.setLoopStateForAllAnimations(OF_LOOP_NONE);
    model.setPausedForAllAnimations(true);
    
    //don't play, we'll manually scrub through the animation
//    model.playAllAnimations();

    
    //NOTE: Assimp scaling does not affect vertices themselves. In addition
    //scaling using .setScale() does not give reliable results. So, set scale
    //normalization to false and use your own scaling with push/pop matrices.
    
    model.setScaleNormalization(false);
    model.setScale(1.0, 1.0, 1.0); //unnecessary

    
    //-----------------------------------------
    //    define modelScale and dimensions
    //-----------------------------------------
    
    //Since model Scale and dimensions are mostly hardcoded to give good values
    //bulkScale scales all of them equally to make minor adjustments
    float bulkScale = 1.0f;
    
    //Scale is figured out visually since AssImp Model does not give reliable sizing
    //Scale factor (as a function of book height)
//    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), 600, 900);
//    modelScale = x;
//    cout << modelScale << endl;
    modelScale = 819.219 * bulkScale;
    
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
//    float x = ofMap(ofGetMouseX(), 0, ofGetWidth(), -0.1, 0);
//    float y = ofMap(ofGetMouseY(), 0, ofGetHeight(), -0.1, 0);
//    model.setPosition(-0.0477083, -0.0147917, -0.0833189);
//    cout << x << ", " << y << endl;
    if(bookType == 0){
        model.setPosition(-0.0477083, -0.0147917, -0.0682016);
    } else if(bookType == 1){
        model.setPosition(-0.0477083, -0.0147917, -0.0757602);
    } else {
        model.setPosition(-0.0477083, -0.0147917, -0.0833189);
    }

    
    textureFBO.allocate(tex -> getWidth(), tex -> getHeight());
    
    textureFBO.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    tex -> draw(0, 0);
    
    textureFBO.end();
    
    
    //create a mesh from the texture to make the
    //spine placeholder for when the book is inactive
    float texCoordX = 515; //position of spine in texture map
    float texCoordY = 99;
    float texWidth = 86;
    float texHeight = 710;
    spineMesh = tex -> getMeshForSubsection(0, -height, 0, thickness, height, texCoordX, texCoordY, texWidth, texHeight, true, OF_RECTMODE_CORNER);
    
    
    
    //Texture coordinages and dimensions of the page regions
    //within the book texture
    pageTexCoords.resize(6);
    
    pageTexCoords[0].set(1118.503, 112.136);
    pageTexCoords[1].set(1584.182, 112.136);
    pageTexCoords[2].set(  57.516, 863.598);
    pageTexCoords[3].set( 559.016, 863.598);
    pageTexCoords[4].set(1050.016, 862.695);
    pageTexCoords[5].set(1542.516, 862.695);
    
    pageDims.resize(6);
    
    pageDims[0].set(431.985, 695.695);
    pageDims[1].set(431.985, 695.695);
    pageDims[2].set(459.516, 746.028);
    pageDims[3].set(459.516, 746.028);
    pageDims[4].set(459.516, 746.028);
    pageDims[5].set(459.516, 746.028);
    
    bIsActive = false;
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
            
            //Book is flipped to second open pages
            float secondPageTime = firstPageTime + 1.0f;
            
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
                
            } else if(now < firstPageTime){
                
                //do a linear mapping of the animation and clamp it
                animPos = ofMap(now, displayReadyTime, firstPageTime, animationStart, animFirstPages, true);
                
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
            
            
        } else {
            
            //if we're not displaying
            
            
        }
        
        
        
    }
    
    
    
    
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
            ofScale(modelScale, modelScale, modelScale);
            
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



