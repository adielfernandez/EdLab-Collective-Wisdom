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
    
    //Maximum number of books that can be held by all 6 shelves is ...
    numBooksPerShelf = 1;
    numShelves = 1;
    
    int numBooks = numBooksPerShelf * numShelves;
    
    
//    ofDirectory texDir;
//    texDir.listDir("books/textures");
    
    //GROUPING ALGORITHM
    //Populates shelves with small groupings of different book types and colors
    //3 styles of book, short, medium, tall
    //Various fonts and textures
    //Each group has a consistent model, texture and font
    int numBookTypes = 3;
    int numTexTypes = 7;
    int numFontTypes = 5;
    
    int placeInThisGroup = 0;
    int numInThisGroup = round(ofRandom(2,3));
    int groupBookType = floor(ofRandom(numBookTypes));
    int groupTexType = floor(ofRandom(numTexTypes));
    int groupFontType = floor(ofRandom(numFontTypes));
    
    int lastGroupBookType = groupBookType;
    int lastGroupTexType = groupTexType;
    int lastGroupFontType = groupFontType;
    
    //Model and texture used by this book
    int thisBookType = 0;
    int thisTexType = 0;
    int thisFontType = 0;
    
    for(int i = 0; i < numBooks; i++){
        
        if(placeInThisGroup < numInThisGroup){
            
            thisBookType = groupBookType;
            thisTexType = groupTexType;
            thisFontType = groupFontType;
            
        } else {
            
            //New book grouping, make new properties
            numInThisGroup = round(ofRandom(2,5));
            
            groupBookType = floor(ofRandom(numBookTypes));
            groupTexType = floor(ofRandom(numTexTypes));
            groupFontType = floor(ofRandom(numFontTypes));

            //If it's the same as last time, add one (wrap if too far)
            if(groupBookType == lastGroupBookType){
                groupBookType++;
                if(groupBookType >= numBookTypes) groupBookType %= numBookTypes;
                
            }
            if(groupTexType == lastGroupTexType){
                groupTexType++;
                if(groupTexType >= numTexTypes) groupTexType %= numTexTypes;
            }
            if(groupFontType == lastGroupFontType){
                groupFontType++;
                if(groupFontType >= numFontTypes) groupFontType %= numFontTypes;
            }
            
            placeInThisGroup = 0;
            thisBookType = groupBookType;
            thisTexType = groupTexType;
            thisFontType = groupFontType;
            
            lastGroupTexType = groupTexType;
            lastGroupBookType = groupBookType;
            lastGroupFontType = groupFontType;
        }
        
        cout << "Loading Book " << i << " - Place in this group " << placeInThisGroup << ", model " << thisBookType << ", texture " << thisTexType << endl;
        
        Book b;
        b.loadModel(thisBookType, thisTexType, thisFontType);

        
        books.push_back(b);
        
        //book type
//        thisBookType++;
//        if(thisBookType == 3) thisBookType = 0;
//        
        placeInThisGroup++;
        
    }
    
}

void BookController::setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase){

    leftBookcase = leftCase;
    rightBookcase = rightCase;
    
}

void BookController::setup(vector<Contribution> *cList){
    
    contributionList = cList;
    
    //load images with manual file names
    ofDirectory texDir;
    texDir.listDir("books/textures");
    
    for(int i = 0; i < (int)texDir.size(); i++){
        
        ofImage img;
        img.load(texDir.getPath(i));
        
        ofTexture t;
        t = img.getTexture();
        
        textures.push_back(t);
        
    }
    
    ofDirectory fontDir;
    fontDir.listDir("fonts");
    
    for(int i = 0; i < (int)fontDir.size(); i++){
        
        ofTrueTypeFont f;
        f.load(fontDir.getPath(i), 25);
        
        fonts.push_back(f);
        
        
        
    }
    
    
    
    ofVec3f shelfStart, shelfEnd;
    ofVec3f dirToShelfEnd;
    ofVec3f midShelfBottom, midShelfTop;
    
    //distance from one book to the next
    float bookSpacing = 1;
    float leftPadding = 1.5;
    
    int bookCounter = 0;
    int booksThisShelf = 0;
    
    //set up books on both bookcases
    for(int j = 0; j < numShelves; j++){
        
        
        //first three shelves in LEFT bookcase j = 0,1,2
        //second three shelves are in RIGHT bookcase (j - 3) = 0,1,2
        int shelfNum = (j < 3) ? j : j - 3;
        
        Bookcase *leftOrRight = (j < 3) ? leftOrRight = leftBookcase : leftOrRight = rightBookcase;
        
        shelfStart = leftOrRight -> shelfCorners[shelfNum][0];
        dirToShelfEnd = (leftOrRight -> shelfCorners[shelfNum][1]) - (leftOrRight -> shelfCorners[shelfNum][0]);
        shelfEnd = leftOrRight -> shelfCorners[shelfNum][1];
        
        midShelfBottom = (leftOrRight -> shelfCorners[shelfNum][0] + leftOrRight -> shelfCorners[shelfNum][1])/2.0f;
        midShelfTop = (leftOrRight -> shelfCorners[shelfNum][2] + leftOrRight -> shelfCorners[shelfNum][3])/2.0f;

        
        
        //get the direction vector from the left side of the shelf to the right
        //then place the books on the shelf along that line, just in case the mapping
        //is noticeably off the horizontal
        dirToShelfEnd.normalize();

        //add a little padding between the left-most book and the left bookcase wall
        shelfStart += dirToShelfEnd * leftPadding;
        
        //move to the next shelf on the next loop
        booksThisShelf += numBooksPerShelf;
        
        ofVec2f currentShelfPos = shelfStart;
        
        for(int i = bookCounter; i < booksThisShelf; i++){
            
            //now that all the positions and defaults have been set, set it up
            books[i].setup(&textures[ books[i].texType ], &fonts[ books[i].fontType ] );
            
            //get the place of the current book on its own shelf
            int bookNumThisShelf = i % numBooksPerShelf;
            
            //after setup (once book has found its own dimensions),
            //set its position in the bookcase
            books[i].pos = currentShelfPos;
            //add the book thickness to the shelf to place the next book
            currentShelfPos += dirToShelfEnd * (books[i].thickness + bookSpacing);
            
            //figure out the display position of this book: the center
            //of the current shelf, raised so book is centered
            books[i].displayPos = (midShelfBottom + midShelfTop)/2.0f;

            //shift the book up (y) a bit to center it and out (z) towards the viewer
            books[i].displayPos += ofVec3f(0, books[i].height/2.0, -20);
            
            books[i].storedPos = books[i].pos;
            
            float sideShift = -100; //ofMap(pctOfShelf, 0.0, 1.0, -50, -lengthOfShelf/3.0f);
            
            //shift the position to the left, move it up so it's centered (like displayPos)
            //and pull it out the depth of the book
            books[i].pulledOutPos = books[i].pos + ofVec3f(sideShift, 0, -(books[i].depth));

            books[i].shelfNum = j;

            
            //Add the contributions from the list to the book
            //if we don't have enough contributions, mark extra books as bIsUnused
            if(i < contributionList -> size()){
                
                //mark book as used
                books[i].bIsUnused = false;
                
                //copy the contribution into the book
                books[i].userContribution = (*contributionList)[i];
                
                //and format the text to be displayed
                //print the book number too
                cout << "-----BOOK # " << i << "-----" <<endl;
                books[i].formatTextForDisplay();
                
            } else {
                
                books[i].bIsUnused = true;
                
            }
            
            //move on to next book
            bookCounter++;
        }
    }

    
    
    

    
}

void BookController::checkMouseBookTrigger(int x, int y){
    
    
    //Check all books for mouse presence
    for(int i = 0; i < books.size(); i++){
        
        //see if mouse is inside book
        //remember: book pos is lower left corner
        if(x > books[i].pos.x && x < books[i].pos.x + books[i].thickness
           && y < books[i].pos.y && y > books[i].pos.y - books[i].height){
            
            books[i].triggerDisplay();
            
            
        }
        
    }
    
}

void BookController::onNewContribution( Contribution& c ){
    
    cout << "New Message Received by book controller" << endl;
    cout << c.name << ", " << c.message << endl;
    
}


void BookController::update(){
    
    
    //update all books
    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == false){
            books[i].update();
        }
    }
    
}

void BookController::draw(){
    

    
//    ofDisableArbTex();


    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == false){
            books[i].draw();
        }
    }
    
    
//    ofEnableArbTex();
    
    
    
}