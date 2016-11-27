//
//  BookController.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/17/16.
//
//

#ifndef BookController_hpp
#define BookController_hpp

#include <stdio.h>

#endif /* BookController_hpp */

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Book.hpp"
#include "Bookcase.hpp"

#pragma once


class BookController{
    
public:
    
    BookController();
    
    void loadModels();
    void setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase);
    void setup();
    void update();
    void draw();
    
    void checkMouseBookTrigger(int x, int y);
    
    vector<ofTrueTypeFont> fonts;
    
    vector<Book> books;
    
    //textures: book cover/page textures
    vector<ofTexture> textures;
    
    //get pointers to the bookcase where we'll
    //be putting the books
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;
    
    int numBooksPerShelf;
    int numShelves;

    vector<bool> bShelvesInUse;

    void bookReturnedEvt();
    
};



