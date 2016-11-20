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

void Book::setup(int type, ofTexture *_tex){

    bookType = type;
    tex = _tex;
    
    
    //Start working with model
    //NOTE: Assimp models do not draw on their mesh vertices
    //Scaling and translations do not affect the mesh vertices themselves
    //So trying to find things like the dimensions of the model need to be
    //done visually since finding the min/max XYZ bounds will always return
    //the original bounds of the model, pre scaling/translation
    
//    if(bookType == 0){
//        model.loadModel("books/bookShort.fbx", false);
//    } else if(bookType == 1){
//    } else {
//        model.loadModel("books/bookTall.fbx", false);
//    }
    
    model.loadModel("books/bookMedium.fbx", false);
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    model.setPausedForAllAnimations(true);
    
    
    modelScale = 0.2;
    model.setScale(modelScale, modelScale, modelScale);
    model.setPosition(0, 0, 0);
    
    //rotate mesh 0 by 90 degrees in X axis
    //NOTE: this method doesn't allow chained rotations
    //(i.e. rotate in X THEN rotate in y, etc.)
    //it's easier to do it in run time with multiple calls to ofRotate()
    //    model.setRotation(0, 90, 1, 0, 0);
    
    int totalVerts;
    maxX = -100000;
    minX =  100000;
    maxY = -100000;
    minY =  100000;
    maxZ = -100000;
    minZ =  100000;
    
    
    //figure out the average XYZ of all the vertices
    cout << "Num Meshes in model: " << model.getNumMeshes() << endl;
    
//    ofMesh m0 = model.getMesh(0);
//    for(int i = 0; i < m0.getNumVertices(); i++){
//        
//        ofVec3f thisVert = m0.getVertex(i);
//        
//        if(thisVert.x > maxX) maxX = thisVert.x;
//        if(thisVert.x < minX) minX = thisVert.x;
//        
//        if(thisVert.y > maxY) maxY = thisVert.y;
//        if(thisVert.y < minY) minY = thisVert.y;
//        
//        if(thisVert.z > maxZ) maxZ = thisVert.z;
//        if(thisVert.z < minZ) minZ = thisVert.z;
//        
//    }
    
    
    
//    cout << "Min X: " << minX << endl;
//    cout << "Max X: " << maxX << endl;
//    cout << "Min Y: " << minY << endl;
//    cout << "Max Y: " << maxY << endl;
//    cout << "Min Z: " << minZ << endl;
//    cout << "Max Z: " << maxZ << endl;
    
    //negate the x and y so that dimensions line up with
    //model's actual spatial orientation
    minX *= -1;
    maxX *= -1;
    minY *= -1;
    maxY *= -1;
    
    realMaxX = maxX * vertScale * modelScale;
    realMinX = minX * vertScale * modelScale;
    realMaxY = maxY * vertScale * modelScale;
    realMinY = minY * vertScale * modelScale;
    realMaxZ = maxZ * vertScale * modelScale;
    realMinZ = minZ * vertScale * modelScale;
    
    //print real pixel dimensions
//    cout << "realMin X: " << realMinX << endl;
//    cout << "realMax X: " << realMaxX << endl;
//    cout << "realMin Y: " << realMinY << endl;
//    cout << "realMax Y: " << realMaxY << endl;
//    cout << "realMin Z: " << realMinZ << endl;
//    cout << "realMax Z: " << realMaxZ << endl;
    
    //store the real dimensions
    modelRealDim.set(realMaxX + abs(realMinX), realMaxY + abs(realMinY), realMaxZ + abs(realMinZ));
    
}

void Book::update(){
    
    
    model.update();
    
    
    
}

void Book::draw(){
    
    ofPushMatrix();
    
    //rotate and translate book so that it has the lower left corner of the spine
    //as its origin and is oriented properly: book stored in shelf
    ofRotateX(90);
    ofRotateY(180);
    ofTranslate(0, 0, -realMaxZ);
    
    //Now translate the book to the position on the shelf
    //and orient it at the right angle (if we're stored on the shelf or being read)
    ofTranslate(pos);
    
    
    
    model.disableTextures();
    if(liveTexture.isAllocated()) liveTexture.getTexture().bind();
    
    model.drawFaces();
    
    if(liveTexture.isAllocated()) liveTexture.getTexture().unbind();
    
    
    
    ofPopMatrix();
    //#ifndef TARGET_PROGRAMMABLE_GL
    //    glEnable(GL_NORMALIZE);
    //#endif

    
    
}



