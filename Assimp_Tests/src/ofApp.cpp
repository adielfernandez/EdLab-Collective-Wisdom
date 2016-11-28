#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    model.loadModel("newlyMapped/bookTall.fbx", false);
//    model.loadModel("converted/bookMedium.fbx", false);
//    model.loadModel("converted/bookShort.fbx", false);
//    model.loadModel("ship/Ship N181113.3DS", false);
    
/*
 
 
 DIMENSIONS
 SHORT:
 Min X: -836
 Max X: 836
 Min Y: -264.067
 Max Y: 264.067
 Min Z: -1328
 Max Z: 1328
 
 MEDIUM:
 Min X: -836
 Max X: 836
 Min Y: -264.067
 Max Y: 264.067
 Min Z: -1328
 Max Z: 1328
 
 TALL:
 Min X: -836
 Max X: 836
 Min Y: -264.067
 Max Y: 264.067
 Min Z: -1328
 Max Z: 1328
 
 
 */
    
    
    
    
    
//    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
    
    drawOnTex = false;
    meshNum = 0;
    
    //get original texture
    origTex = model.getTextureForMesh(meshNum);
    
//    ofImage t;
//    t.load("bust/tex.jpg");
//    
//    origTex = t.getTexture();
    
    //copy it to an FBO we'll actually draw with
    newSkin.allocate(origTex.getWidth(), origTex.getHeight());
    
    newSkin.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    
//    t.draw(0, 0);
    model.getTextureForMesh(meshNum).draw(0, 0);
    
    newSkin.end();
    
    
    
    
    //NOTE: Assimp scaling does not affect vertices themselves. In addition
    //scaling using .setScale() does not give reliable results. So, set scale
    //normalization to false and use your own scaling with push/pop matrices.
    model.setScaleNormalization(false);
    model.setScale(1.0f, 1.0f, 1.0f);
    model.setPosition(0, 0, 0);
    

    
    cout << "Number of meshes: " << model.getNumMeshes() << endl;
    
    //print a bunch of data on each mesh
    for(int i = 0; i < model.getNumMeshes(); i++){
        
        cout << "Mesh[" << i << "] Num Verts: " << model.getMesh(i).getNumVertices() << endl;
        
    }
    
    cout << "Model Position: " << model.getPosition() << endl;
    cout << "Model Scene Center: " << model.getSceneCenter() << endl;
    
    int totalVerts;
    maxX = -100000;
    minX =  100000;
    maxY = -100000;
    minY =  100000;
    maxZ = -100000;
    minZ =  100000;
    
    
    //figure out the average XYZ of all the vertices
    for(int i = 0; i < model.getNumMeshes(); i++){
        
        ofMesh thisMesh = model.getMesh(i);
        

        for(int j = 0; j < thisMesh.getNumVertices(); j++){
            
            ofVec3f thisVert = thisMesh.getVertex(j);
            meshCenter += thisVert;
            
            if(thisVert.x > maxX) maxX = thisVert.x;
            if(thisVert.x < minX) minX = thisVert.x;
            
            if(thisVert.y > maxY) maxY = thisVert.y;
            if(thisVert.y < minY) minY = thisVert.y;
            
            if(thisVert.z > maxZ) maxZ = thisVert.z;
            if(thisVert.z < minZ) minZ = thisVert.z;
            
        }

        
        totalVerts += thisMesh.getNumVertices();
    }
    
    meshCenter /= totalVerts;
    
    cout << "Model Total Num Verts: " << totalVerts << endl;
    cout << "Model MESH Cummulative Center: " << meshCenter << endl;
    
    cout << "Min X: " << minX << endl;
    cout << "Max X: " << maxX << endl;
    cout << "Min Y: " << minY << endl;
    cout << "Max Y: " << maxY << endl;
    cout << "Min Z: " << minZ << endl;
    cout << "Max Z: " << maxZ << endl;

    float actualThickness = abs(maxY) + abs(minY);
    float actualDepth = abs(maxX) + abs(minX);
    float actualHeight = abs(maxZ) + abs(minZ);
    
    cout << "Actual Thickness: " << actualThickness << endl;
    cout << "Actual Depth: " << actualDepth << endl;
    cout << "Actual Height: " << actualHeight << endl;

    
    
//    float desiredBookHeight = 200;
//    modelScale = desiredBookHeight/ ( abs(minZ) + abs(maxZ) );
//    cout << "Desired Scale: " << modelScale << endl;
    
    //Scale is figured out visually
    //Scale of 661.697 yields book height = 100
    //Scale of 1323.394 yields book height = 200
    modelScale = 661.697;
    
    realMaxX = maxX * modelScale;
    realMinX = minX * modelScale;
    realMaxY = maxY * modelScale;
    realMinY = minY * modelScale;
    realMaxZ = maxZ * modelScale;
    realMinZ = minZ * modelScale;
    
    //store the real dimensions
    //Model's orientation within its own coordinate system is different
    //so Y = thickness, etc.
    float thickness = abs(realMaxY) + abs(realMinY);
    float depth = abs(realMaxX) + abs(realMinX);
    float height = abs(realMaxZ) + abs(realMinZ);

    
    realMaxX = maxX * modelScale;
    realMinX = minX * modelScale;
    realMaxY = maxY * modelScale;
    realMinY = minY * modelScale;
    realMaxZ = maxZ * modelScale;
    realMinZ = minZ * modelScale;
    
    //print real pixel dimensions
    cout << "Model Scale: " << modelScale << endl;
    
    cout << "realMin X: " << realMinX << endl;
    cout << "realMax X: " << realMaxX << endl;
    cout << "realMin Y: " << realMinY << endl;
    cout << "realMax Y: " << realMaxY << endl;
    cout << "realMin Z: " << realMinZ << endl;
    cout << "realMax Z: " << realMaxZ << endl;
    
    cout << "Thickness: " << thickness << endl;
    cout << "Depth: " << depth << endl;
    cout << "Height: " << height << endl;
    
    //-----LIGHTING & CAMERA-----
    
    // Directional Lights emit light based on their orientation, regardless of their position //
    frontLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
    frontLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    frontLight.setDirectional();
    
    // set the direction of the light
    frontLight.setOrientation( ofVec3f(90, 0, 0) );
    
    backLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
    backLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    backLight.setDirectional();
    
    // set the direction of the light
    backLight.setOrientation( ofVec3f(270, 0, 0) );

    spineLight.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
    spineLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    spineLight.setDirectional();
    
    // set the direction of the light
    spineLight.setOrientation( ofVec3f(180, 90, 0) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 128 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    cam.setNearClip(0);
    cam.setFarClip(1000000);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    model.update();
    
    if(bAnimateMouse) {
        animationPosition = mouseY / (float)ofGetHeight();
        model.setPositionForAllAnimations(animationPosition);
    }
    
    

//    modelScale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1300, 1400);
    
    //Lighting Angle finder (to get quaternion angles)
    
//    float angle1 = ofMap(mouseX, 0, ofGetWidth(), 0, 360);
//    float angle2 = ofMap(mouseY, 0, ofGetHeight(), 0, 360);
//    spineLight.setOrientation( ofVec3f(angle1, angle2, 0) );
// 
//    cout << angle1 << ", " << angle2 <<endl;

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofEnableDepthTest();
//    ofEnableLighting();
//#ifndef TARGET_PROGRAMMABLE_GL
//    glShadeModel(GL_SMOOTH); //some model / light stuff
//#endif
    
//    frontLight.enable();
//    backLight.enable();
//    spineLight.enable();
    cam.begin();
//    ofEnableSeparateSpecularLight();

    
    ofPushMatrix();
    ofScale(modelScale, modelScale, modelScale);
//    ofScale(1.0, 1.0, 1.0);
    //rotate and translate book so that it is oriented as it
    //would be in a bookshelf facing the user
//    ofRotateX(-90);
//    ofRotateZ(90);
//    ofTranslate(0, 0, realMinZ);
    
    model.disableTextures();
    if(newSkin.isAllocated()) newSkin.getTexture().bind();
    
    model.drawFaces();

    if(newSkin.isAllocated()) newSkin.getTexture().unbind();
    
    //draw local axes
    ofDrawAxis(300);
    
    ofPopMatrix();
//#ifndef TARGET_PROGRAMMABLE_GL
//    glEnable(GL_NORMALIZE);
//#endif
    
//    ofDisableDepthTest();
//    frontLight.disable();
//    backLight.disable();
//    spineLight.disable();
//    ofDisableLighting();

    ofDrawAxis(1000);

    
    //Draw at ACTUAL coordinates from vertices
    float lineLength = 2000;
    ofSetLineWidth(1);
    ofSetColor(255, 200, 0);
    //in Y=minY plane
    ofDrawLine(minX, minY, -lineLength, minX, minY, lineLength);
    ofDrawLine(maxX, minY, -lineLength, maxX, minY, lineLength);
    ofDrawLine(-lineLength, minY, minZ, lineLength, minY, minZ);
    ofDrawLine(-lineLength, minY, maxZ, lineLength, minY, maxZ);
    
    //in Y=maxY plane
    ofDrawLine(minX, maxY, -lineLength, minX, maxY, lineLength);
    ofDrawLine(maxX, maxY, -lineLength, maxX, maxY, lineLength);
    ofDrawLine(-lineLength, maxY, minZ, lineLength, maxY, minZ);
    ofDrawLine(-lineLength, maxY, maxZ, lineLength, maxY, maxZ);

    
    //Draw at scaled coordinates
    ofSetColor(0, 255, 0);
    //in Y=minY plane
    ofDrawLine(realMinX, realMinY, -lineLength, realMinX, realMinY, lineLength);
    ofDrawLine(realMaxX, realMinY, -lineLength, realMaxX, realMinY, lineLength);
    ofDrawLine(-lineLength, realMinY, realMinZ, lineLength, realMinY, realMinZ);
    ofDrawLine(-lineLength, realMinY, realMaxZ, lineLength, realMinY, realMaxZ);
    
    //in Y=realMaxY plane
    ofDrawLine(realMinX, realMaxY, -lineLength, realMinX, realMaxY, lineLength);
    ofDrawLine(realMaxX, realMaxY, -lineLength, realMaxX, realMaxY, lineLength);
    ofDrawLine(-lineLength, realMaxY, realMinZ, lineLength, realMaxY, realMinZ);
    ofDrawLine(-lineLength, realMaxY, realMaxZ, lineLength, realMaxY, realMaxZ);

    
    cam.end();
    
//    ofDisableSeparateSpecularLight();
    
    
    
    string s;
    
    s += "fps: "+ ofToString(ofGetFrameRate(), 2) + "\n";
    s += "Spacebar to trigger animation\n";
    s += "Drag to control animation with mouseY\n";
    s += "Num animations found: " + ofToString(model.getAnimationCount()) + "\n";
    s += "Current Mesh Num: " + ofToString(meshNum) + " out of " + ofToString(model.getNumMeshes()) + "\n";
    s += "Tex Dim: " + ofToString(origTex.getWidth()) + ", " + ofToString(origTex.getHeight()) + "\n";
    s += "NewSkin Dim: " + ofToString(newSkin.getWidth()) + ", " + ofToString(newSkin.getHeight()) + "\n";
    s += "Right click to draw on texture\n";
    s += "Hold shift to rotate model with mouse X\n";
    
    
    ofSetColor(255);
    ofDrawBitmapString(s, 10, 15);
    
//    ofDrawBitmapString(ofToString(x) + ", " + ofToString(y), mouseX, mouseY);
    
    float fboScale = 0.4;
    
    ofVec2f fboDrawPos(0, ofGetHeight() - fboScale * newSkin.getHeight());
    
    ofSetColor(255);
    if(newSkin.isAllocated())
    newSkin.draw(fboDrawPos.x, fboDrawPos.y, fboScale * newSkin.getWidth(), fboScale * newSkin.getHeight());
    
    if(drawOnTex && ofGetMousePressed()){
        if(mouseX > fboDrawPos.x && mouseX < fboScale * newSkin.getWidth() + fboDrawPos.x && mouseY > fboDrawPos.y && mouseY < fboScale * newSkin.getHeight() + fboDrawPos.y){
            
            newSkin.begin();
            ofSetColor(0, 255, 0, 255);

            int drawSize = 10;
            
            ofDrawEllipse(mouseX/fboScale, (mouseY - fboDrawPos.y)/fboScale, drawSize/fboScale, drawSize/fboScale);
            
            newSkin.end();
            
            
        }
    }
    
    if(bAnimateMouse){
        ofDrawBitmapStringHighlight("Anim Pos: " + ofToString(animationPosition), mouseX + 10, mouseY + 20);
    } else {
        ofDrawBitmapStringHighlight("modelScale: " + ofToString(modelScale), mouseX, mouseY - 10);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' '){
        bAnimate = !bAnimate;
    } else if(key == 'c'){
        newSkin.begin();
        ofClear(255, 255, 255, 255);
        ofSetColor(255);
        origTex.draw(0, 0);
        newSkin.end();
    } else if(key == OF_KEY_SHIFT){
        drawOnTex = true;
        cam.disableMouseInput();
    } else if(key == 'a'){
        
        // pause all animations, so we can scrub through them manually.
        model.setPausedForAllAnimations(true);
        
        bAnimateMouse = true;
        
    }
    
    
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    if(key == '1'){
        model.loadModel("newlyMapped/bookShort.fbx", false);
    } else if(key == '2'){
        model.loadModel("newlyMapped/bookMedium.fbx", false);
    } else if(key == '3'){
        model.loadModel("newlyMapped/bookTall.fbx", false);
    }
    
    
    bAnimateMouse = false;
    
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
        
    }
    

    
    drawOnTex = false;
    cam.enableMouseInput();
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // scrub through aninations manually.
    animationPosition = y / (float)ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    


    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // unpause animations when finished scrubbing.

    bAnimateMouse = false;

    drawOnTex = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
