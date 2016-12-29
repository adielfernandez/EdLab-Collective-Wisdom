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
    numBooksPerShelf = 0;
    numShelves = 1;
    
    int numBooks = numBooksPerShelf * numShelves;
    
    
//    ofDirectory texDir;
//    texDir.listDir("books/textures");
//    
//    ofDirectory fontDir;
//    fontDir.listDir("fonts");
    
    //GROUPING ALGORITHM
    //Populates shelves with small groupings of different book types and colors
    //3 styles of book, short, medium, tall
    //Various fonts and textures
    //Each group has a consistent model, texture and font
    int numBookTypes = 2;
    int numTexTypes = 7; //texDir.size();
    int numFontTypes = 3; //fontDir.size();
    
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
        
        cout << "Loading Book " << i << " - Place in this group " << placeInThisGroup << ", model " << thisBookType << ", texture " << thisTexType << ", font " << thisFontType << endl;
        
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
    fontDir.listDir("bookFonts");
    
    for(int i = 0; i < (int)fontDir.size(); i++){
        
        ofTrueTypeFont f;
        f.load(fontDir.getPath(i), 11);
        
        fonts.push_back(f);
    }
    
    UIFont.load("assets/interface/Century-gothic-bold.ttf", 15);

    //decrease letter spacing a bit to fit more text into tight spaces
    UIFont.setLetterSpacing(0.9);

    
    buttonIcons.resize(6);
    buttonIcons[0].load("assets/interface/buttons/exit.png");
    buttonIcons[1].load("assets/interface/buttons/exitHover.png");
    buttonIcons[2].load("assets/interface/buttons/left.png");
    buttonIcons[3].load("assets/interface/buttons/leftHover.png");
    buttonIcons[4].load("assets/interface/buttons/right.png");
    buttonIcons[5].load("assets/interface/buttons/rightHover.png");
    
    //set the buttons to draw from the middle
    for(int i = 0; i < buttonIcons.size(); i++){
        buttonIcons[i].setAnchorPercent(0.5f, 0.5f);
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
        
        //half way from bottom left corner to bottom right corner
        midShelfBottom = (leftOrRight -> shelfCorners[shelfNum][0] + leftOrRight -> shelfCorners[shelfNum][1])/2.0f;
        
        //half way from top left corner to top right corner
        midShelfTop = (leftOrRight -> shelfCorners[shelfNum][2] + leftOrRight -> shelfCorners[shelfNum][3])/2.0f;

        //while we're going through each shelf, set the shelf overlays
        
        ShelfOverlay so;
        
        //top left corner
        so.pos = leftOrRight -> shelfCorners[shelfNum][3];
        
        //center of shelf
        //so.pos = midShelfTop + (midShelfBottom - midShelfTop)/2.0;
        
        ofVec2f w = shelfEnd - shelfStart;
        so.width = w.length();
        ofVec2f h = midShelfBottom - midShelfTop;
        so.height = h.length();
        
        shelfOverlays.push_back(so);

        
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
            books[i].setup(&textures[ books[i].texType ], &fonts[ books[i].fontType ], &UIFont );
            
            //get the place of the current book on its own shelf
            int bookNumThisShelf = i % numBooksPerShelf;
            
            //after setup (once book has found its own dimensions),
            //set its position in the bookcase
            books[i].pos = currentShelfPos;
            
            //move book out away from wall to clear the wallpaper tiles
            books[i].pos.z -= 50;
            
            //add the book thickness to the shelf to place the next book
            currentShelfPos += dirToShelfEnd * (books[i].thickness + bookSpacing);
            
            //figure out the display position of this book: the center
            //of the current shelf, raised so book is centered
            books[i].displayPos = (shelfStart + shelfEnd)/2.0f;

            //shift the book out (z) towards the viewer and shifted to the right
            books[i].displayPos += ofVec3f(52, 0, books[i].pos.z - 20);
            
            //shift the book up (y) a bit to center it and out (z) towards the viewer
//            books[i].displayPos += ofVec3f(0, books[i].h  eight/2.0, -20);
            
            books[i].storedPos = books[i].pos;
            
            float sideShift = -100; //ofMap(pctOfShelf, 0.0, 1.0, -50, -lengthOfShelf/3.0f);
            
            //shift the position to the left, move it up so it's centered (like displayPos)
            //and pull it out the depth of the book
            books[i].pulledOutPos = books[i].pos + ofVec3f(sideShift, 0, -(books[i].depth));

            books[i].shelfNum = j;
            books[i].bookID = i;
            
            //give the book buttons the icon images and the position of the shelf corners
            books[i].setupUI(&buttonIcons, leftOrRight -> shelfCorners[shelfNum]);
            
            //Add the contributions from the list to the book
            //if we don't have enough contributions, mark extra books as bIsUnused
            if(i < contributionList -> size()){
                
                //mark book as used
                books[i].bIsUnused = false;
                
                //set up the content
                books[i].setupContent((*contributionList)[i]);
                
            } else {
                
                books[i].bIsUnused = true;
                
                //-----REMOVE THIS WHEN DONE TESTING!!!-----
                
                //copy the contribution into the book
//                books[i].userContribution = (*contributionList)[0];
//                
//                //and format the text to be displayed
//                //print the book number too
//                books[i].formatTextForDisplay();
                
            }
            
            
            //add event listeners to each of the buttons so the book controller
            //can do stuff when buttons are pressed
            ofAddListener(books[i].prevButton.newButtonClickEvt, this, &BookController::onButtonClickEvt);
            ofAddListener(books[i].nextButton.newButtonClickEvt, this, &BookController::onButtonClickEvt);
            ofAddListener(books[i].exitButton.newButtonClickEvt, this, &BookController::onButtonClickEvt);
            ofAddListener(books[i].tagButton.newButtonClickEvt, this, &BookController::onButtonClickEvt);
            
            
            //move on to next book
            bookCounter++;
        }
        

        
    }


    
    
    

    
}

void BookController::checkMouseEvents(int x, int y){
    

    //Check all books for mouse presence
    for(int i = 0; i < books.size(); i++){
     
        
        //if we're not active AND the touch is true, check for book triggers
        if(!books[i].bIsActive && ofGetMousePressed()){
            
            //only check books that are not on active shelves
            if(shelfOverlays[books[i].shelfNum].bIsActive == false){
                
                //see if mouse is inside book
                //remember: book pos is lower left corner
                if(x > books[i].pos.x && x < books[i].pos.x + books[i].thickness
                   && y < books[i].pos.y && y > books[i].pos.y - books[i].height){
                    
                    //book is triggered
                    books[i].triggerDisplay();
                    
                    //mark this shelf as active and trigger the shelf overlay
                    shelfOverlays[books[i].shelfNum].activate();

                    
                }

            }
            
        } else {
            
            //if we ARE active, check for buttons
            books[i].checkButtonsForClicks(x, y, ofGetMousePressed());

                        
        }
        
        
        
    }
    
    
    
}

//event triggered only for exit and tag buttons
void BookController::onButtonClickEvt(ButtonEvent &b){
    
    //if it's an exit button event, close that book and deactivate that shelfOverlay
    if(b.type == 0){
        shelfOverlays[b.shelfNum].deactivate();
        
    } else if(b.type == 3){
        
        
        //go through all the books and activate the tags
        for(int i = 0; i < books.size(); i++){
            
            if(books[i].tagNum == b.tagNum){
                books[i].showTaglet();
        
            }
            
            
        }
        
        
        
    }
    
    //now check for tag activations!
    
    
    

    
    
}

void BookController::onNewContribution( Contribution& c ){
    
    //go through the book vector, look for the first unused book
    //then mark it as used and add the contribution to it
    for(int i = 0; i < books.size(); i++){
        
        if(books[i].bIsUnused == true){
            
            //mark book as used
            books[i].bIsUnused = false;
            
            //set up the content
            books[i].setupContent((*contributionList)[i]);
            
            //now break out of the for loop
            //so we only trigger one book
            break;
        }
        
    }
    
}


void BookController::update(){
    
    
    //update all books
    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == false){
            
            books[i].update();
        }
    }
    
    //this will eventually be looping through camera touches
    checkMouseEvents(ofGetMouseX(), ofGetMouseY());
    
    
    //update the shelf overlays
    for(int i = 0; i < shelfOverlays.size(); i++){
        shelfOverlays[i].update();
    }
    
    
    
}

void BookController::draw(){
    

    
    ofEnableDepthTest();
    
    
    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == false){
            books[i].draw();
        }
    }
    
    for(int i = 0; i < shelfOverlays.size(); i++){
        shelfOverlays[i].draw();
    }

    ofDisableDepthTest();
    
    
    
    
}