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
    
    //create the ornaments first so we
    //know how many books need to fit around them
    
    int numOrnaments = 6;
    int totalOrnamentBookWidths = 0;
    
    for(int i = 0; i < numOrnaments; i++){
        Ornament o;
        
        //4 different ornament types
        o.setup(i);
        
        totalOrnamentBookWidths += o.numBooksWide;
        
        ornaments.push_back(o);
    }
    
    
    //Number of shelves we'll create
    numShelves = 6;
    
    //Find this number experimentally after setting
    //ornaments on the shelf and seeing how many books fit
    int numBooks = maxNumBooks - totalOrnamentBookWidths;
    
    cout << "Total books taken up by ornaments: " << totalOrnamentBookWidths << endl;
    cout << "Total number of books: " << numBooks << endl;
    
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
            numInThisGroup = round(ofRandom(2,4));
            
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
        
//        cout << "Loading Book " << i << " - Place in this group " << placeInThisGroup << ", model " << thisBookType << ", texture " << thisTexType << ", font " << thisFontType << endl;
        
        Book b;
        b.loadModel(thisBookType, thisTexType, thisFontType);
        books.push_back(b);
        
        placeInThisGroup++;
        
    }
    
}

void BookController::setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase){

    leftBookcase = leftCase;
    rightBookcase = rightCase;
    
}

void BookController::setScholarData(ScholarData *sData){
    
    scholarData = sData;
    
}

void BookController::setup(vector<Contribution> *cList){
    
    contributionList = cList;
    
    //load images with manual file names
    ofDirectory texDir;
    texDir.listDir("books/textures");
    
    for(int i = 0; i < (int)texDir.size(); i++){
        
        ofTexture t;
        ofLoadImage(t, texDir.getPath(i));
        
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
    
    
    lastNewBookEvent = 0;
    lastRecycleTime = 0.0f;
    
    //SETUP THE GUI
    setupGui();
    lastBookSettingsUpdate = 0;

    
    ofVec3f shelfStart, shelfEnd;
    ofVec3f dirToShelfEnd;
    ofVec3f midShelfBottom, midShelfTop;
    
    //distance from one book to the next
    float bookSpacing = 0;
    
    //space between first book and left edge of shelf
    float leftPadding = 1.5;
    
    int bookCounter = 0;
    int booksThisShelf = 0;
    
    //convenience vector of flags for
    //if the shelf has an item or not
    vector<bool> shelfHasItem;
    shelfHasItem.assign(6, false);

    for(int i = 0; i < ornaments.size(); i++){
        shelfHasItem[ ornaments[i].shelfNum ] = true;
    }
    

    
    int ornamentToPlace = 0;
    
    float maxShelfWidth = bookWidth * 16;
    
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
        
        //Figure out how many books to place on this shelf
//        booksThisShelf += numBooksPerShelf;
        
        if( shelfHasItem[j] ){
            booksThisShelf += (maxBooksPerShelf - ornaments[ornamentToPlace].numBooksWide);
            
        } else {
            booksThisShelf += maxBooksPerShelf;
        }
        
        
        ofVec2f currentShelfPos = shelfStart;
        
        int thisShelfStart = bookCounter;
        
        //TRICKY: This loop goes one PAST how many books on the current shelf
        //So we'll check at the end of every row to see if there's an ornament there
        //If there is, place it. If not, do nothing.
        //Either way, continue without incrementing bookCounter
        for(int i = bookCounter; i < booksThisShelf + 1; i++){
            
            //place the ornament on the shelf if we're at that place
            int placeThisShelf = i - thisShelfStart;
            
            if( shelfHasItem[j] && placeThisShelf == ornaments[ornamentToPlace].placeOnShelf ){
                
                ornaments[ornamentToPlace].setShelfPosition(currentShelfPos);
                
                //push the current shelf pos past the ornament
                currentShelfPos.x += ornaments[ornamentToPlace].gapWidth;

                
                //this ornament has been placed,
                //get ready to place the next one
                ornamentToPlace++;
                
                //mark this shelf as not having an ornament
                shelfHasItem[j] = false;
                
            }
            
            //If we're after the last book on the shelf and we've checked for/set up any ornaments
            //skip across the rest of the loop
            if(i == booksThisShelf){
                continue;
            }
            
            //now that all the positions and defaults have been set, set it up
            books[i].setup(&textures[ books[i].texType ], &fonts[ books[i].fontType ], &UIFont );
            
            ofVec3f stored;
            ofVec3f display;
            int overallShelfNum = j;
            int bookNum = i;
            
            //after setup (once book has found its own dimensions),
            //set its position in the bookcase but pushed toward viewer
            stored.set(currentShelfPos.x, currentShelfPos.y, -50);
            
            //figure out the display position of this book: the center
            //of the current shelf, raised so book is centered
            display = (shelfStart + shelfEnd)/2.0f;
            
            //X: shifted to the right in the shelf to make room for the tag button on the left
            //Y: upward a tad to move it vertically off the shelf
            //Z: shift the book out towards the viewer
            display += ofVec3f(52, -5, -70);
            
            
            //give books their locations
            books[i].setLocation(stored, display, overallShelfNum, bookNum);
            
            //give the book buttons the icon images and the position of the shelf corners
            books[i].setupUI(&buttonIcons, leftOrRight -> shelfCorners[shelfNum]);

            
            //add the book thickness to the shelf to place the next book
            currentShelfPos += dirToShelfEnd * (books[i].thickness + bookSpacing);
            
            
            //Add the contributions from the list to the book
            //if we don't have enough contributions, mark extra books as bIsUnused
            if(i < contributionList -> size()){
                
                //mark book as used
                books[i].bIsUnused = false;
                
                //get tag number and color from the tagList in scholar data to set up the book
                int tagNum;
                ofColor tagCol;
                
                string thisTag = (*contributionList)[i].tag;
                
                //go through the tagList and see which one this tag is
                for(int i = 0; i < scholarData -> tagList.size(); i++){
                    if(thisTag.compare(scholarData -> tagList[i]) == 0){
                        tagNum = i;
                    }
                }
                
                tagCol = scholarData -> tagColorList[tagNum];

                
                //set up the content
                //give a reference to the individual contribution in the list
                books[i].setupContent( (*contributionList)[i] , tagNum, tagCol);
                books[i].putInShelf();
                
                //also mark this message as no longer archived
                (*contributionList)[i].bIsArchived = false;
                
            } else {
                
                books[i].bIsUnused = true;
                
                
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
     
        //if we're not active AND we're not in a new book event AND
        //the touch is true, check for book triggers
        if(!books[i].bIsActive && !books[i].bIsNewBookEvt && ofGetMousePressed()){
            
            //only check books that are not on active shelves
            if(shelfOverlays[books[i].shelfNum].bIsInUse == false){
                
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

        //source of the tag ribbons
        ofVec3f src;

        //first check if it was the centerBook tagButton
        if(b.bIsCenterBookButton){
            
            //src based on centerbook button position (should make this a gui setting)
            src = ofVec3f( 990, 960, -80);
            
            
        } else {

            //src position based on typical book button positions
            BookUIButton *bttn = &books[b.bookNum].tagButton;
            src = bttn -> displayedPos + ofVec3f(bttn -> buttonWidth/2, bttn -> buttonHeight/2, -80);
        }
        
        //go through all the books and trigger a ribbon from that
        //book's tag button to all the books that share the same tag
        for(int i = 0; i < books.size(); i++){
            
            //check if the book has the same tag, but make sure
            //it's not the same as the src book
            if( b.bookNum != i && books[i].tagNum == b.tagNum){
                
                TagRibbon tr;
                
                ofVec3f dst = books[i].storedPos + ofVec3f(books[i].thickness/2.0f, - 135);

                tr.setup(src, dst, numPointsSlider, b.tagCol);
                tr.dstBookNum = i;
                
                //helper function to deliver all the gui
                //settings to the ribbon before sending it off
                giveSettingsToRibbon( &tr );
                
                tagRibbons.push_back(tr);
                
            }
            
        }
        
    }
    

    
}

void BookController::addToLibrary( Contribution& c ){
 
    int fillThisIndex;
    
    //go through the book vector, if we have any unused books available,
    //trigger those, if not, recycle an existing one
    bool emptyBookAvailable = false;
    
    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == true){
            
            emptyBookAvailable = true;
            
            //mark book as used
            books[i].bIsUnused = false;
            
            fillThisIndex = i;
            
            break;
        }
    }
    
    
    //See if any unused books are available
    if( emptyBookAvailable == false ){
        
        //no unused books available, recycle an existing one
        //pick a random number and find a book that is available
        //doing it this way instead of using a for loop
        //prevents reusing the same few books at the beginning of the vector
        
        int timesChecked = 0;
        
        do {
            
            int checkIndex = floor( ofRandom(books.size()) );
            
            //look for a book that is available (not active or spawning)
            //and look for a shelf that is not active so it doesn't interrupt
            //someone reading a book
            int thisBookShelf = books[checkIndex].shelfNum;
            
            bool shelfInUse = shelfOverlays[thisBookShelf].bIsInUse;
            
            if( !shelfInUse && !books[checkIndex].bIsActive && !books[checkIndex].bIsNewBookEvt ){
                
                //if we're recycling a used book, we need to mark the
                //contribution that it currently has as archived before replacing it
                
                //find the book in the contributionList that matches the ID of this
                //message then mark it
                for(int i = 0; i < contributionList -> size(); i++){
                    
                    if( (*contributionList)[i].ID == books[checkIndex].userContribution.ID ){
                        
                        (*contributionList)[i].bIsArchived = true;
                        
                        break;
                        
                    }
                    
                }
                
                
                
                //jot it down so we can reset it below
                fillThisIndex = checkIndex;
                
            }
            
            timesChecked++;
            
            
        } while ( timesChecked < 30 );
        
    }

    //if we've gotten this far,
    //get tag number and color from the tagList in scholar data to set up the book
    int tagNum;
    ofColor tagCol;
    
    for(int i = 0; i < scholarData -> tagList.size(); i++){
        if( c.tag.compare(scholarData -> tagList[i]) == 0 ){
            tagNum = i;
        }
    }
    
    tagCol = scholarData -> tagColorList[tagNum];
    
    //set up the content
    books[fillThisIndex].setupContent( c , tagNum, tagCol);
    
    //Find the message in the contributionList and mark as NOT archived since
    //the message is in a book. We need to do this since the book's user
    //contribution is a copy, not the live message we're keeping track of
    for(int i = 0; i < contributionList -> size(); i++){
        
        if( (*contributionList)[i].ID == c.ID ){
            
            (*contributionList)[i].bIsArchived = false;
            break;
            
        }
        
    }
    
    
    books[fillThisIndex].triggerNewBookEvt();
    
    lastNewBookEvent = ofGetElapsedTimef();
    
}

void BookController::onNewContribution( Contribution& c ){
    
    
    //check if it has been long enough since the last event or if there
    //is an available bookshelves to add a book to.
    //if not, add the contribution to the queue and wait
    if(ofGetElapsedTimef() - lastNewBookEvent > newBookIntervalSlider){
        
        addToLibrary( c );
    
    } else {
        
        //if no available books found (coming in too fast or all shelves
        //are occupied), store the index of the book in the queue
        incomingQueue.push_back( c );
        
    }
        
}


void BookController::update(){
    
    //If we're pressing the gui button to update books, update them all
    //but only do it a few times per second
    if( bIsGuiActive && updateBooksToggle && ofGetElapsedTimef() - lastBookSettingsUpdate > 0.2f){
        
        setBookVarsFromGui();
        
        guiPosSlider = gui.getPosition();
        
        
        lastBookSettingsUpdate = ofGetElapsedTimef();
    }
    
    //set this to false, the drawGui() method will set it to true later
    bIsGuiActive = false;
    
    
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
    
    //update the ornaments
    for(int i = 0; i < ornaments.size(); i++) {
        ornaments[i].update();
    }
    
    //it's been long enough...
    if( ofGetElapsedTimef() - lastNewBookEvent > newBookIntervalSlider ){
       
        //check if there's anything in the queue
        if( !incomingQueue.empty() ){
            
            //trigger a new book event with the oldest one
            addToLibrary( *incomingQueue.begin() );
            
            //then remove the oldest one (the first element in the vector)
            incomingQueue.pop_front();
        
        } else {
            
            //the queue is empty, so check if there are books in the archive
            //and if it's been long enough for a recycling event
            if( contributionList -> size() > books.size() ){
                
                //AND it's been long enough...
                if( ofGetElapsedTimef() - lastRecycleTime > archiveRecycleSlider){
                    
                    //            cout << "Triggering recycle event" << endl;
                    
                    //-----BRING ONE BACK FROM THE DEAD/ARCHIVE-----
                    //Go through all the contributions and compile a short list of all the
                    //archived messages into a vector. Then go through that vector
                    //and randomly pick one (It's the easiest way to randomly pick from the
                    //archived ones without having to sort the contribution list)
                    //Then send that message to the onNewContribution()
                    //method to assign it to a new book
                    
                    vector<int> archivedIndices;
                    
                    for(int i = 0; i < contributionList -> size(); i++){
                        if( (*contributionList)[i].bIsArchived ){
                            archivedIndices.push_back(i);
                        }
                    }
                    
                    if( archivedIndices.size() > 0 ){
                        
                        int randIndex = floor( ofRandom(archivedIndices.size()) );
                        int indexToRecycle = archivedIndices[randIndex];
                        
                        //send to get recycled, this method will take care of
                        //switching the archival stats of the old/new messages
                        addToLibrary( (*contributionList)[indexToRecycle] );
                        
                    }
                    
                    
                    
                    lastRecycleTime = ofGetElapsedTimef();
                }
                
            }
            
        }
    }
    
    
    
    //update any tagRibbons we have. Iterate through it backwards
    //so we don't cause any problems when we delete elements
    for(int i = tagRibbons.size() - 1; i >= 0; i--){

        if( !tagRibbons[i].bIsDead ){
            
            tagRibbons[i].update();
            
            if( tagRibbons[i].bTouchDown ){
                books[ tagRibbons[i].dstBookNum ].showTaglet();
            }
            
        } else {
            
            //if it's dead remove it
            tagRibbons.erase(tagRibbons.begin() + i);
            
        }
        
        
    }
    


    
}

void BookController::draw(){
    
    //disable arbitrary textures for all the books
    ofDisableArbTex();

    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    ofTranslate(0, 0, -30);
    

    //draw the books
    for(int i = 0; i < books.size(); i++){
        if(books[i].bIsUnused == false){
            books[i].draw();
        }
    }
    
    
    //draw the shelf overlays
    for(int i = 0; i < shelfOverlays.size(); i++){
        shelfOverlays[i].draw();
    }

    //draw the ornaments
    for(int i = 0; i < ornaments.size(); i++) {
        ornaments[i].draw();
    }

    
    //draw any tagRibbons we have
    for(int i = 0; i < tagRibbons.size(); i++){
        tagRibbons[i].draw();
    }
    
    
    ofPopMatrix();
    
    ofDisableDepthTest();
//    ofDisableAlphaBlending();
    
    ofEnableArbTex();
    
}




void BookController::setBookVarsFromGui(){
    
    //go through each book and give them the right settings
    for(int i = 0; i < books.size(); i++){
        
        books[i].spawnDuration = spawnDurationSlider;
        books[i].spawnPosBackEasing = spawnPosBackEaseSlider;
        books[i].tagletDuration = tagletDurationSlider;
        books[i].pageLerpSpeed = pageFlipSpeedSlider;
        books[i].totalBookOpenTime = bookOpenTimeSlider;
        books[i].totalBookCloseTime = bookCloseTimeSlider;
        books[i].setButtonSpeeds(buttonLerpSpeedSlider);
        
        books[i].spawnRibbons.numRibbonsToDraw = numRibbonsSlider;
        books[i].spawnRibbons.orbitSpeed = orbitSpeedSlider;
        books[i].spawnRibbons.numSegments = numSegmentsSlider;
        books[i].spawnRibbons.totalLength = ribbonLengthSlider;
        books[i].spawnRibbons.ribbonWidth = ribbonWidthSlider;
        books[i].spawnRibbons.ribbonTaper = ribbonTaperSlider;
        books[i].spawnRibbons.baseRad = baseRadSlider;
        books[i].spawnRibbons.noiseAmp = noiseAmplitudeSlider;
        books[i].spawnRibbons.noiseSpeed = noiseSpeedSlider;
        books[i].spawnRibbons.nScale = noiseScaleSlider;
        books[i].spawnRibbons.colorRange = colorRangeSlider;
        books[i].spawnRibbons.shrinkTime = shrinkTimeSlider;
        books[i].spawnRibbons.useNoise = useNoiseToggle;
        books[i].spawnRibbons.staggerAmt = staggerAmountSlider;
        books[i].spawnRibbons.bDrawWireframe = drawWireframeToggle;
    }

    for(int i = 0; i < shelfOverlays.size(); i++){
        
        shelfOverlays[i].openDelay = openDelaySlider;
        shelfOverlays[i].closeDelay = closeDelaySlider;
        shelfOverlays[i].openDuration = openDurationSlider;
        shelfOverlays[i].closeDuration = closeDurationSlider;
        
    }
    
    
}

void BookController::giveSettingsToRibbon(TagRibbon *tr){
    
    tr -> pctApex = pctApexSlider;
    tr -> dt = segmentDTSlider;
    tr -> duration = tagRibbonDurationSlider;
    tr -> arcHeight = arcHeightSlider;
    
    tr -> bDrawWireframe = tagRibbonWireframeToggle;
    tr -> ribbonWidth = tagWidthSlider;
    tr -> ribbonTaper = tagTaperSlider;
    tr -> colorRange = tagColorRangeSlider;
    
    tr -> bUseNoise = tagUseNoiseToggle;
    tr -> noiseAmp = tagNoiseAmplitudeSlider;
    tr -> noiseSpeed = tagNoiseSpeedSlider;
    tr -> noiseScale = tagNoiseScaleSlider;
    
}

void BookController::setupGui(){
    
    
    guiName = "bookController";
    
    
    filePath = "settings/";
    gui.setup(guiName, filePath + guiName + ".xml", 0, 0);
    
    ofVec2f min(0, 0);
    ofVec2f max(1920, 1200);
    
    gui.add(guiPosSlider.setup("Gui Position", min, min, max));

    
    gui.add(settingsLabel.setup("  LIBRARY SETTINGS", ""));
    
    gui.add(archiveRecycleSlider.setup("Archive Interval", 60.0f, 1.0f, 300.0f));
    gui.add(newBookIntervalSlider.setup("New Book Interval", 2.0f, 0.2f, 10.0f));
    
    gui.add(booksLabel.setup("  BOOK SETTINGS", ""));
    gui.add(updateBooksToggle.setup("Update Book Vars", false));
    gui.add(tagletDurationSlider.setup("Tag duration", 6.0f, 2.0f, 10.0f));
    gui.add(pageFlipSpeedSlider.setup("Page Flip Speed", 0.05, 0.001f, 0.5f));
    gui.add(bookOpenTimeSlider.setup("Book Open Time", 4.2, 1.0f, 8.0f));
    gui.add(bookCloseTimeSlider.setup("Book Close Time", 4.2, 1.0f, 8.0f));
    gui.add(buttonLerpSpeedSlider.setup("Button Speed", 0.08, 0.005f, 0.3f));
    
    gui.add(spawnSettingsLabel.setup("  SPAWN SETTINGS", ""));
    gui.add(spawnDurationSlider.setup("Spawn duration", 4.0f, 0.5f, 10.0f));
    gui.add(spawnPosBackEaseSlider.setup("Pos Back Ease", 1.0f, 0.0f, 3.0f));
    
    gui.add(ribbonSettingsLabel.setup("  RIBBON SETTINGS", ""));
    gui.add(numRibbonsSlider.setup("Num ribbons", 5, 1, 10));
    gui.add(orbitSpeedSlider.setup("Orbit speed", 2.0f, 0.1f, 10.0f));
    gui.add(ribbonLengthSlider.setup("Ribbon length", 135, 10, 360));
    gui.add(ribbonWidthSlider.setup("Ribbon width", 20, 1, 60));
    gui.add(ribbonTaperSlider.setup("Ribbon taper", 0.2f, 0.0f, 1.0f));
    
    gui.add(numSegmentsSlider.setup("Num segments", 15, 2, 40));
    gui.add(colorRangeSlider.setup("Color range", 20, 0, 50));
    gui.add(baseRadSlider.setup("Base radius", 100, 0, 200));
    gui.add(shrinkTimeSlider.setup("Shrink time", 0.5f, 0.0f, 5.0f));
    gui.add(staggerAmountSlider.setup("Stagger radii", 10.0f, 0.0f, 40.0f));
    gui.add(drawWireframeToggle.setup("Draw wireframe", false));
    
    gui.add(useNoiseToggle.setup("Use Noise", true));
    gui.add(noiseAmplitudeSlider.setup("Noise amplitude", 10.0f, 0.0f, 30));
    gui.add(noiseSpeedSlider.setup("Noise speed", 0.01f, 0.0001f, 4.0f));
    gui.add(noiseScaleSlider.setup("Noise scale", 0.01f, 0.0001f, 0.5f));
    
    gui.add(tagRibbonLabel.setup("  TAG RIBBONS", ""));
    gui.add(tagRibbonWireframeToggle.setup("Draw wireframe", false));
    gui.add(numPointsSlider.setup("Num points", 10, 2, 30));
    gui.add(pctApexSlider.setup("Pct dist Apex", 0.5f, 0.0f, 1.0f));
    gui.add(arcHeightSlider.setup("Arc height", 100.0f, 0.0f, 300.0f));
    gui.add(segmentDTSlider.setup("Segment dt", 0.1f, 0.001f, 1.0f));
    gui.add(tagRibbonDurationSlider.setup("Duration", 1.5f, 0.01f, 5.0f));
    gui.add(tagWidthSlider.setup("Ribbon width", 40.0f, 1.0f, 60.0f));
    gui.add(tagTaperSlider.setup("Ribbon taper", 0.1f, 0.0f, 1.0f));
    gui.add(tagColorRangeSlider.setup("Color range", 25.0f, 0.00f, 50.0f));
    gui.add(tagUseNoiseToggle.setup("Use Noise", true));
    gui.add(tagNoiseAmplitudeSlider.setup("Tag noise amp", 10.0f, 0.0f, 50.0f));
    gui.add(tagNoiseSpeedSlider.setup("Tag noise speed", 0.01f, 0.0001f, 4.0f));
    gui.add(tagNoiseScaleSlider.setup("Tag noise scale", 0.01f, 0.0001f, 1.0f));
    
    gui.add(shelfOverlayLabel.setup("  SHELF OVERLAYS", ""));
    gui.add(openDelaySlider.setup("Open delay", 0.5f, 0.0f, 3.0f));
    gui.add(openDurationSlider.setup("Open duration", 2.0f, 0.001f, 4.0f));
    gui.add(closeDelaySlider.setup("Close delay", 0.5f, 0.0f, 3.0f));
    gui.add(closeDurationSlider.setup("Close duration", 2.0f, 0.001f, 4.0f));
    
    gui.setHeaderBackgroundColor(ofColor(255));
    
    //color applies to gui title only
    gui.setTextColor(ofColor(0));
    
    settingsLabel.setBackgroundColor(ofColor(255));
    booksLabel.setBackgroundColor(ofColor(255));
    spawnSettingsLabel.setBackgroundColor(ofColor(255));
    ribbonSettingsLabel.setBackgroundColor(ofColor(255));
    tagRibbonLabel.setBackgroundColor(ofColor(255));
    shelfOverlayLabel.setBackgroundColor(ofColor(255));
    
    //this changes the color of all the labels
    settingsLabel.setDefaultTextColor(ofColor(0));
    
    gui.setPosition(ofGetWidth()/2 - gui.getWidth()/2, 30);
    
    bIsGuiActive = true;
    
    gui.minimizeAll();
    
    loadSettings();
    
    //now that we've loaded, move the gui to where we last had it
    gui.setPosition(guiPosSlider -> x, guiPosSlider -> y);
    
}

void BookController::loadSettings(){
    
    gui.loadFromFile( filePath + guiName + ".xml");
    
    setBookVarsFromGui();
    
}

void BookController::saveSettings(){
    
    gui.saveToFile(filePath + guiName + ".xml");
    
}


void BookController::drawGui(){
    gui.draw();
    bIsGuiActive = true;
}

void BookController::drawGui(int x, int y){
    gui.setPosition(x, y);
    gui.draw();
    bIsGuiActive = true;
}
