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
#include "ShelfOverlay.hpp"
#include "../Furniture/Bookcase.hpp"
#include "../Content/Contribution.hpp"
#include "../Content/ContentManager.hpp"
#include "BookUIButton.hpp"


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
    
    
    //Content for all the books
    vector<Contribution> *contributionList;
    
    //event triggered by Content Manager
    //when a new contribution is received
    void onNewContribution( Contribution& c );
    
    void onButtonClickEvt(ButtonEvent &b);
    
    //get pointers to the bookcase where we'll
    //be putting the books
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;

    
    //Books vector and Assets
    vector<Book> books;
    vector<ofTexture> textures;
    vector<ofTrueTypeFont> fonts;
    vector<ofImage> buttonIcons;
    
    //Book placement and arrangment
    int numBooksPerShelf;
    int numShelves;
    
    //-----SHELF OVERLAYS AND TRACKING
    //for keeping track of which shelves are in use
    vector<ShelfOverlay> shelfOverlays;
    
};








