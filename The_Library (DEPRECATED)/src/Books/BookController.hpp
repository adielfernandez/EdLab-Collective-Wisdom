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

#pragma once

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
#include "TagRibbon.hpp"

#include "Touch.hpp"



class BookController{
    
public:
    
    BookController();
    
    void loadModels();
    void setBookCaseRefs(Bookcase *leftCase, Bookcase *rightCase);
    void setScholarData(ScholarData *sData);
    void setup(vector<Contribution> *cList);
    void update();
    void draw();
    
    void checkTouchEvents(int x, int y, bool touchState);
    
    ScholarData *scholarData;
    
    //Content for all the books
    vector<Contribution> *contributionList;
    
    //event triggered by Content Manager
    //when a new contribution is received
    void onNewContribution( Contribution& c );

    //Method for adding a book, whether
    //its a new contribution or archive recycle
    void addToLibrary( Contribution& c );
    
    
    list<Contribution> incomingQueue;
    double lastNewBookEvent;
    
    double lastRecycleTime;
    
    
    //stores the touch data
    vector<Touch> touches;
    void receiveTouch(Touch t);
    
    
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
    
    const int maxNumBooks = 108;
    const int maxBooksPerShelf = 18;

    vector<Ornament> ornaments;
    vector<TagRibbon> tagRibbons;

    
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
    
    ofxToggle useNoiseToggle;
    ofxFloatSlider noiseAmplitudeSlider;
    ofxFloatSlider noiseSpeedSlider;
    ofxFloatSlider noiseScaleSlider;
    
    ofxLabel tagRibbonLabel;
    ofxToggle tagRibbonWireframeToggle;
    ofxFloatSlider staggerTimeSlider;
    ofxIntSlider numPointsSlider;
    ofxFloatSlider pctApexSlider;
    ofxFloatSlider arcHeightSlider;
    ofxFloatSlider segmentDTSlider;
    ofxFloatSlider tagRibbonDurationSlider;
    ofxFloatSlider tagWidthSlider;
    ofxFloatSlider tagTaperSlider;
    ofxFloatSlider tagColorRangeSlider;
    ofxToggle tagUseNoiseToggle;
    ofxFloatSlider tagNoiseAmplitudeSlider;
    ofxFloatSlider tagNoiseSpeedSlider;
    ofxFloatSlider tagNoiseScaleSlider;
    
    ofxLabel shelfOverlayLabel;
    ofxFloatSlider openDelaySlider;
    ofxFloatSlider openDurationSlider;
    ofxFloatSlider closeDelaySlider;
    ofxFloatSlider closeDurationSlider;
    
    ofxLabel touchSettingsLabel;
    ofxFloatSlider touchKillAgeSlider;
    
    void giveSettingsToRibbon(TagRibbon *tr);
    
    double lastBookSettingsUpdate;
    
    
};








