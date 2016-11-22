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
    numBooksPerShelf = 29;
    numShelves = 6;
    
    int numBooks = numBooksPerShelf * numShelves;
    
    for(int i = 0; i < numBooks; i++){
        
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
    
    ofVec3f shelfStart;
    ofVec3f dirToShelfEnd;
    

    
    //distance from one book to the next
    float bookSpacing = 1;
    float leftPadding = 1.5;
    
    int bookCounter = 0;
    int booksThisShelf = 0;
    
    //set up books on both bookcases
    for(int j = 0; j < numShelves; j++){
        
        if(j == 0){
            
            shelfStart = leftBookcase -> shelf1Start;
            dirToShelfEnd = (leftBookcase -> shelf1End) - (leftBookcase -> shelf1Start);
            
        } else if(j == 1) {

            shelfStart = leftBookcase -> shelf2Start;
            dirToShelfEnd = (leftBookcase -> shelf2End) - (leftBookcase -> shelf2Start);
            
        } else if(j == 2) {
            
            shelfStart = leftBookcase -> shelf3Start;
            dirToShelfEnd = (leftBookcase -> shelf3End) - (leftBookcase -> shelf3Start);
            
        } else if(j == 3) {
           
            shelfStart = rightBookcase -> shelf1Start;
            dirToShelfEnd = (rightBookcase -> shelf1End) - (rightBookcase -> shelf1Start);
            
        } else if(j == 4) {
            
            shelfStart = rightBookcase -> shelf2Start;
            dirToShelfEnd = (rightBookcase -> shelf2End) - (rightBookcase -> shelf2Start);
            
        } else if(j == 5) {
            
            shelfStart = rightBookcase -> shelf3Start;
            dirToShelfEnd = (rightBookcase -> shelf3End) - (rightBookcase -> shelf3Start);
            
        }
        
        
        //get the direction vector from the left side of the shelf to the right
        //then place the books on the shelf along that line, just in case the mapping
        //is noticeably off the horizontal
        dirToShelfEnd.normalize();

        //add a little padding between the left-most book and the left bookcase wall
        shelfStart += dirToShelfEnd * leftPadding;
        
        //move to the next shelf on the next loop
        booksThisShelf += numBooksPerShelf;
        
        for(int i = bookCounter; i < booksThisShelf; i++){
            
            books[i].setup(&textures[1]);
            
            //get the place of the current book on its own shelf
            int bookNumThisShelf = i % numBooksPerShelf;
            
            //after setup (once book has found its own dimensions),
            //set its position in the bookcase
            books[i].pos = shelfStart + dirToShelfEnd * bookNumThisShelf * (books[i].thickness + bookSpacing);
            
            cout << "Book " << i << " pos: " << books[i].pos << endl;
            
            //move on to next book
            bookCounter++;
        }
    }

    activeBooks.resize(3);
    int rand = (int)ofRandom(books.size() - 3);
    activeBooks[0] = rand;
    activeBooks[1] = rand + 1;
    activeBooks[2] = rand + 2;
    
}

void BookController::update(){

    //randomize the books
    if(ofGetElapsedTimeMillis() % 5000 < 10){
        int rand = (int)ofRandom(books.size() - 3);
        activeBooks[0] = rand;
        activeBooks[1] = rand + 1;
        activeBooks[2] = rand + 2;
        cout << "randomize!" << endl;
    }
    
    
    //go through the active books and see if
    for(int i = 0; i < activeBooks.size(); i++){
        
        books[ activeBooks[i] ].update();
    }
    
}

void BookController::draw(){
    
//    ofDisableArbTex();

    
    for(int i = 0; i < activeBooks.size(); i++){
        books[ activeBooks[i] ].draw();
    }
    
//    ofEnableArbTex();
    
    
    
}