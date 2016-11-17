#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    

    model.loadModel("working/book07.fbx", false);
    
    
    
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
    
    //copy it to an FBO we'll actually draw with
    newSkin.allocate(origTex.getWidth(), origTex.getHeight());
    
    newSkin.begin();
    ofClear(255, 255, 255, 255);
    ofSetColor(255);
    model.getTextureForMesh(meshNum).draw(0, 0);
    
    newSkin.end();
    
    
    
    //Start working with model
    //NOTE: Assimp models do not draw on their mesh vertices
    //Scaling and translations do not affect the mesh vertices themselves
    //So trying to find things like the dimensions of the model need to be
    //done visually since finding the min/max XYZ bounds will always return
    //the original bounds of the model, pre scaling/translation
    
    modelScale = 0.2;
    model.setScale(modelScale, modelScale, modelScale);
    model.setPosition(0, 0, 0);
    
    //rotate mesh 0 by 90 degrees in X axis
    //NOTE: this method doesn't allow chained rotations
    //(i.e. rotate in X THEN rotate in y, etc.)
    //it's easier to do it in run time with multiple calls to ofRotate()
//    model.setRotation(0, 90, 1, 0, 0);
    
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

    //negate the x and y
    minX *= -1;
    maxX *= -1;
    minY *= -1;
    maxY *= -1;
    
    scale = 32.4672;
    realMaxX = maxX * scale * modelScale;
    realMinX = minX * scale * modelScale;
    realMaxY = maxY * scale * modelScale;
    realMinY = minY * scale * modelScale;
    realMaxZ = maxZ * scale * modelScale;
    realMinZ = minZ * scale * modelScale;
    
    //print real pixel dimensions
    cout << "realMin X: " << realMinX << endl;
    cout << "realMax X: " << realMaxX << endl;
    cout << "realMin Y: " << realMinY << endl;
    cout << "realMax Y: " << realMaxY << endl;
    cout << "realMin Z: " << realMinZ << endl;
    cout << "realMax Z: " << realMaxZ << endl;
    
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
    cam.setFarClip(100000);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    model.update();
    
    if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    }
    
    
    if(bMouseScale){
        scale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 50);
    }
    
    
    realMaxX = maxX * scale * modelScale;
    realMinX = minX * scale * modelScale;
    realMaxY = maxY * scale * modelScale;
    realMinY = minY * scale * modelScale;
    realMaxZ = maxZ * scale * modelScale;
    realMinZ = minZ * scale * modelScale;
    
    
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
    ofEnableLighting();
//#ifndef TARGET_PROGRAMMABLE_GL
//    glShadeModel(GL_SMOOTH); //some model / light stuff
//#endif
    
    frontLight.enable();
    backLight.enable();
    spineLight.enable();
    cam.begin();
    ofEnableSeparateSpecularLight();

    
    ofPushMatrix();
    
    //rotate and translate book so that it
//    ofRotateX(90);
//    ofRotateY(180);
//    ofTranslate(0, 0, -realMaxZ);
    
    model.disableTextures();
    if(newSkin.isAllocated()) newSkin.getTexture().bind();
    
    model.drawFaces();

    if(newSkin.isAllocated()) newSkin.getTexture().unbind();
    

    
    ofPopMatrix();
//#ifndef TARGET_PROGRAMMABLE_GL
//    glEnable(GL_NORMALIZE);
//#endif
    
    ofDisableDepthTest();
    frontLight.disable();
    backLight.disable();
    spineLight.disable();
    ofDisableLighting();

    ofDrawAxis(1000);
    
    
    model.getMesh(0);
    
    
    //draw lines to find the bounds of the book
    //in Y=minY plane
    ofSetLineWidth(1);
    ofSetColor(255, 0, 255);
    ofDrawLine(realMinX, realMinY, -1000, realMinX, realMinY, 1000);
    ofDrawLine(realMaxX, realMinY, -1000, realMaxX, realMinY, 1000);
    ofDrawLine(-1000, realMinY, realMinZ, 1000, realMinY, realMinZ);
    ofDrawLine(-1000, realMinY, realMaxZ, 1000, realMinY, realMaxZ);
    
    //in Y=maxY plane
    ofPushMatrix();
    ofTranslate(0, realMaxY, 0);
    ofDrawLine(realMinX, realMinY, -1000, realMinX, realMinY, 1000);
    ofDrawLine(realMaxX, realMinY, -1000, realMaxX, realMinY, 1000);
    ofDrawLine(-1000, realMinY, realMinZ, 1000, realMinY, realMinZ);
    ofDrawLine(-1000, realMinY, realMaxZ, 1000, realMinY, realMaxZ);
    
    ofPopMatrix();
    
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
    
    float fboScale = 0.2;
    
    ofVec2f fboDrawPos(0, ofGetHeight() - fboScale * newSkin.getHeight());
    
    ofSetColor(255);
    if(newSkin.isAllocated())
    newSkin.draw(fboDrawPos.x, fboDrawPos.y, fboScale * newSkin.getWidth(), fboScale * newSkin.getHeight());
    
    if(drawOnTex && ofGetMousePressed()){
        if(mouseX > fboDrawPos.x && mouseX < fboScale * newSkin.getWidth() + fboDrawPos.x && mouseY > fboDrawPos.y && mouseY < fboScale * newSkin.getHeight() + fboDrawPos.y){
            
            newSkin.begin();
            ofSetColor(0, 255, 0, 255);

            int drawSize = 20;
            
            ofDrawEllipse(mouseX/fboScale, (mouseY - fboDrawPos.y)/fboScale, drawSize/fboScale, drawSize/fboScale);
            
            newSkin.end();
            
            
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    ofPoint modelPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75);
    
    
    if(key == ' '){
        bAnimate = !bAnimate;
    } else if(key == 'c'){
        newSkin.begin();
        ofSetColor(255);
        origTex.draw(0, 0);
        newSkin.end();
    } else if(key == OF_KEY_SHIFT){
        drawOnTex = true;
        cam.disableMouseInput();
    } else if(key == 'a'){
        
        // pause all animations, so we can scrub through them manually.
        model.setPausedForAllAnimations(true);
        animationPosition = mouseY / (float)ofGetHeight();
        bAnimateMouse = true;
        
    } else if(key == 's'){
        bMouseScale = true;
    }
    
    
    
    
    else if(key == OF_KEY_RIGHT){
        meshNum++;
        
        if(meshNum > model.getNumMeshes()) meshNum = 0;
        
    }  else if(key == OF_KEY_LEFT){
        meshNum--;

        if(meshNum < 0) meshNum = model.getNumMeshes();
    
    }
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    bAnimateMouse = false;
    
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
        
    }
    
    if(key == 's'){
        
        bMouseScale = false;
        cout << "Scale: " << scale << endl;
        cout << "realMin X: " << realMinX << endl;
        cout << "realMax X: " << realMaxX << endl;
        cout << "realMin Y: " << realMinY << endl;
        cout << "realMax Y: " << realMaxY << endl;
        cout << "realMin Z: " << realMinZ << endl;
        cout << "realMax Z: " << realMaxZ << endl;
        
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
