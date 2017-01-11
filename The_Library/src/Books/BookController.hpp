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
#include "ofxGui.h"
#include "Book.hpp"
#include "ShelfOverlay.hpp"
#include "../Furniture/Bookcase.hpp"
#include "../Content/Contribution.hpp"
#include "../Content/ContributionManager.hpp"
#include "BookUIButton.hpp"

#include "../Content/ScholarData.hpp"

#include "../Ornaments/Ornament.hpp"

#pragma once


class BookController{
    
public:
    
    BookController();
    
    void loadModels();
    void setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase);
    void setScholarData(ScholarData *sData);
    void setup(vector<Contribution> *cList);
    void update();
    void draw();
    
    void checkMouseEvents(int x, int y);
    
    ScholarData *scholarData;
    
    //Content for all the books
    vector<Contribution> *contributionList;
    
    //event triggered by Content Manager
    //when a new contribution is received
    void onNewContribution( Contribution& c );
    
    list<Contribution> incomingQueue;
    double lastNewBookEvent;
    
    int availableBooks;
    double lastRecycleTime;
    
    
    
    
    void onButtonClickEvt(ButtonEvent &b);
    
    //get pointers to the bookcase where we'll
    //be putting the books
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;

    
    //Books vector and Assets
    vector<Book> books;
    vector<ofTexture> textures;
    vector<ofTrueTypeFont> fonts;
    ofTrueTypeFont UIFont;
    vector<ofImage> buttonIcons;
    
    //Book placement and arrangment
    int numShelves;
    const float bookWidth = 24.2906;
    const int maxNumBooks = 96;
    const int maxBooksPerShelf = 16;

    vector<Ornament> ornaments;

    
    
    //-----SHELF OVERLAYS AND TRACKING
    //for keeping track of which shelves are in use
    vector<ShelfOverlay> shelfOverlays;
    
    //GUI setup
    //-----GUI SETUP-----
    void setupGui();
    void drawGui();
    void drawGui(int x, int y);
    void saveSettings();
    void loadSettings();
    void setBookVarsFromGui();
    bool bIsGuiActive;
    
    ofxPanel gui;
    string guiName;
    string filePath;
    
    ofxVec2Slider guiPosSlider;
    
    ofxLabel settingsLabel;
    ofxFloatSlider archiveRecycleSlider;
    ofxFloatSlider newBookIntervalSlider;
    
    ofxLabel booksLabel;
    ofxToggle updateBooksToggle;
    ofxFloatSlider tagletDurationSlider;
    ofxFloatSlider pageFlipSpeedSlider;
    ofxFloatSlider bookOpenTimeSlider;
    ofxFloatSlider bookCloseTimeSlider;
    ofxFloatSlider buttonLerpSpeedSlider;

    ofxLabel spawnSettingsLabel;
    ofxFloatSlider spawnDurationSlider;
    ofxFloatSlider spawnPosBackEaseSlider;
    
    ofxLabel ribbonSettingsLabel;
    ofxIntSlider numRibbonsSlider;
    ofxFloatSlider orbitSpeedSlider;
    ofxIntSlider numSegmentsSlider;
    ofxIntSlider ribbonLengthSlider;
    ofxIntSlider ribbonWidthSlider;
    ofxFloatSlider ribbonTaperSlider;
    ofxIntSlider baseRadSlider;
    ofxIntSlider colorRangeSlider;
    ofxFloatSlider shrinkTimeSlider;
    ofxFloatSlider staggerAmountSlider;
    ofxToggle drawWireframeToggle;
    
    ofxLabel noiseLabel;
    ofxToggle useNoiseToggle;
    ofxFloatSlider noiseAmplitudeSlider;
    ofxFloatSlider noiseSpeedSlider;
    ofxFloatSlider noiseScaleSlider;
    
    ofxLabel shelfOverlayLabel;
    ofxFloatSlider openDelaySlider;
    ofxFloatSlider openDurationSlider;
    ofxFloatSlider closeDelaySlider;
    ofxFloatSlider closeDurationSlider;
    
    double lastBookSettingsUpdate;
    
    
};








