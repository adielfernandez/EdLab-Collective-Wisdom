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
    texType = bType;
    
    if(bookType == 0){
        model.loadModel("books/bookShort.fbx", false);
    } else if(bookType == 1){
        model.loadModel("books/bookMedium.fbx", false);
    } else {
        model.loadModel("books/bookTall.fbx", false);
    }

    

    
}


void Book::setup(ofTexture *_tex){

    tex = _tex;
    
    bPrintDebug = false;
    
    
    //figure out the minimum and maximum dimensions
    maxX = -100000;
    minX =  100000;
    maxY = -100000;
    minY =  100000;
    maxZ = -100000;
    minZ =  100000;
    
    
    
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
    
    

    
    /*
     
     BOOK DIMENSIONS:
     Medium: height-thickness aspect = 5:1
     Short: same thickeness as medium, 10% shorter
     Tall: 10% taller than medium, 75% of the width
     
     
     //When model scale = 1.0,
     The pixel dimensions are below
     
     Min X: 0.0384481
     Max X: 13.3184
     Min Y: 0.0603343
     Max Y: 1.82742
     Min Z: -10.4535
     Max Z: 9.24601
     
     For a book height of 130px, the dimensions are:
     realMin X: 0.253725
     realMax X: 87.89
     realMin Y: 0.398155
     realMax Y: 12.0594
     realMin Z: -68.9841
     realMax Z: 61.0159
     Thickness: 12.4576
     Depth: 88.1437
     Height: 130
     
     */
    
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

    
    //-------------------------------------
    //define modelScale according to height
    //-------------------------------------
    
    //Scale is figured out visually since AssImp Model does not give reliable sizing
    //Scale factor (as a function of book height)
    scaleFactor = 7.29;
    
    //Set the scale so that the middle size book (bookMedium.fbx)
    //sizes to approximately 120px. Then other books will
    //scale to their own heights
    float desiredHeight = 120;
    modelScale = desiredHeight * scaleFactor;
    
    
    realMaxX = maxX * modelScale;
    realMinX = minX * modelScale;
    realMaxY = maxY * modelScale;
    realMinY = minY * modelScale;
    realMaxZ = maxZ * modelScale;
    realMinZ = minZ * modelScale;
    
    //store the "real" dimensions, i.e. apparent dimensions
    //first get the depth and thickness as proportion of height.

    float modelHeight = abs(realMaxZ) + abs(realMinZ);
    float modelThickness = abs(realMaxY) + abs(realMinY);
    float modelDepth = abs(realMaxX) + abs(realMinX);

    float thicknessRatio = modelThickness/modelHeight;
    float depthRatio = modelDepth/modelHeight;
    
    height = desiredHeight;
    thickness = desiredHeight * thicknessRatio;
    depth = desiredHeight * depthRatio;

    
    if(bPrintDebug){
        //Print model sizing for debug
        //values listed above
        cout << "Min X: " << minX << endl;
        cout << "Max X: " << maxX << endl;
        cout << "Min Y: " << minY << endl;
        cout << "Max Y: " << maxY << endl;
        cout << "Min Z: " << minZ << endl;
        cout << "Max Z: " << maxZ << endl;
        
        //print real pixel dimensions
        cout << "realMin X: " << realMinX << endl;
        cout << "realMax X: " << realMaxX << endl;
        cout << "realMin Y: " << realMinY << endl;
        cout << "realMax Y: " << realMaxY << endl;
        cout << "realMin Z: " << realMinZ << endl;
        cout << "realMax Z: " << realMaxZ << endl;
        
        cout << "Model Scale: " << modelScale << endl;
        cout << "Thickness: " << thickness << endl;
        cout << "Depth: " << depth << endl;
        cout << "Height: " << height << endl;
    }
    
    //model positioned at bottom left corner of spine
    //values acquired visually since AssImp model does not give reliable size/scaling
//    float m = ofMap(ofGetMouseY(), 0, ofGetHeight(), -0.1, 0);
//    cout << m << endl;
    model.setPosition(-0.0473501, -0.0148567, -0.0682016);
    
    

    
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

//        ofSetLineWidth(2);
//        ofDrawAxis(100);
//
//        ofScale(6, 6, 6);
        
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



