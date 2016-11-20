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

void BookController::setup(Bookcase *leftCase, Bookcase *rightCase){
    
    leftBookcase = leftCase;
    rightBookcase = rightCase;
    
    
    //load up all the book textures
    ofDirectory dir;
    
    dir.listDir("books/images/");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector with correct # of images
    if( dir.size() ){
        textures.assign(dir.size(), ofTexture());
    }
    
    for(int i = 0; i < (int)dir.size(); i++){
        
        ofImage img;
        img.load(dir.getPath(i));
        
        textures[i] = img.getTexture();
        
    }
    
    ofVec3f placeOnShelf;
    placeOnShelf = leftBookcase -> shelf1Pos;

    
    
    //set up books
    for(int i = 0; i < 1; i++){
        
        //book type
        int bookType = 1;  //make a grouping algorithm but for now set to working model
        
        
//        Book b;
//        b.setup(bookType, &textures[0]);
//        
//        //after setup (once book has found its own dimensions),
//        //set its position in the bookcase
//        b.pos = placeOnShelf;
//        
//        placeOnShelf += b.modelRealDim.y;
//        
//        books.push_back(b);
        
    }
    
    singleBook.setup(1, &textures[0]);
    
    //after setup (once book has found its own dimensions),
    //set its position in the bookcase
    singleBook.pos = placeOnShelf;
    
}

void BookController::update(){

//    for(int i = 0; i < books.size(); i++){
//        books[i].update();
//    }
    
    
}

void BookController::draw(){
    
    
//    for(int i = 0; i < books.size(); i++){
//        books[i].draw();
//    }
    
    
    
}