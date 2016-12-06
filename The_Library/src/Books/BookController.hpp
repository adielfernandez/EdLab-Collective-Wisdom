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
#include "../Furniture/Bookcase.hpp"
#include "../Content/Contribution.hpp"
#include "../Content/ContentManager.hpp"

#pragma once


class BookController{
    
public:
    
    BookController();
    
    void loadModels();
    void setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase);
    void setup(vector<Contribution> *cList);
    void update();
    void draw();
    
    void checkMouseEvents(int x, int y);
    void checkMouseBookTrigger(int x, int y);
    void checkActiveBookButtons(int x, int y);
    
    
    //Content for all the books
    vector<Contribution> *contributionList;
    
    void onNewContribution( Contribution& c );
    
    //get pointers to the bookcase where we'll
    //be putting the books
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;

    
    //Books vector and Assets
    vector<Book> books;
    vector<ofTexture> textures;
    vector<ofTrueTypeFont> fonts;
    
    //Book placement and arrangment
    int numBooksPerShelf;
    int numShelves;
    
};



