//
//  BookController.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/17/16.
//
//

#include "BookController.hpp"


BookController::BookController(){
    
}

void BookController::loadModels(){
    
    for(int i = 0; i < 1; i++){
        
        //book type
        int bookType = 1;  //make a grouping algorithm but for now set to working model
        
        
        Book b;
        b.loadModel(bookType);
        
        books.push_back(b);
        
    }
    
}

void BookController::setup(Bookcase *leftCase, Bookcase *rightCase){
    
    leftBookcase = leftCase;
    rightBookcase = rightCase;
    
    
    //load images with manual file names
    //loading with ofDirectory conflicts with ofxAssimp
    ofDirectory texDir;
    texDir.listDir("books/images");
    
    for(int i = 0; i < (int)texDir.size(); i++){
        
        ofImage img;
        img.load(texDir.getPath(i));
        
        ofTexture t;
        t = img.getTexture();
        
        textures.push_back(t);
        
    }
    
    ofVec3f placeOnShelf;
    placeOnShelf = leftBookcase -> shelf1Pos + ofVec3f(10, 0, 0);

    
    
    //set up books
    for(int i = 0; i < books.size(); i++){
        
        books[i].setup(&textures[1]);
        
        //after setup (once book has found its own dimensions),
        //set its position in the bookcase
        books[i].pos = placeOnShelf;
        
        cout << "Book " << i << " pos: " << placeOnShelf << endl;
        
        placeOnShelf.x += books[i].thickness;
        placeOnShelf.z = 5;
        
    }
    

    
}

void BookController::update(){


    
    for(int i = 0; i < books.size(); i++){
        books[i].update();
    }
    
    
}

void BookController::draw(){
    
//    ofDisableArbTex();

    for(int i = 0; i < books.size(); i++){
        books[i].draw();
    }
//    ofEnableArbTex();
    
    
    
}