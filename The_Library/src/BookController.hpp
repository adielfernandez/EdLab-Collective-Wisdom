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
    void setup(Bookcase *leftCase, Bookcase *rightCase);
    void update();
    void draw();
    
    Book singleBook;
    
    vector<Book> books;
    vector<ofTexture> textures;
    
    
    //get pointers to the bookcase where we'll
    //be putting the books
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;
    
    
};