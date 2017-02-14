//
//  SceneController.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 2/13/17.
//
//

#ifndef SceneController_hpp
#define SceneController_hpp

#include <stdio.h>

#endif /* SceneController_hpp */

#include "ofMain.h"
#include "Furniture/Wallpaper.hpp"
#include "Furniture/Frame.hpp"
#include "Furniture/Bookcase.hpp"
#include "Books/CenterBook.hpp"
#pragma once

class SceneController{
    
public:
    
    SceneController();
    
    
    void setup();
    void update();
    
    void setBookcaseRefs( Bookcase *left, Bookcase *right );
    void setFrameRef( Frame *fr );
    void setWallpaperRef( Wallpaper *paper );
    void setCenterBookRef( CenterBook *book );
    
    void setIdleTimerRef( double * idle );
    
    void setTextureCombo(int num);
    void triggerAllWave(int x, int y, int num);
    
    Bookcase *leftBookcase;
    Bookcase *rightBookcase;
    CenterBook *centerBook;
    Frame *frame;
    Wallpaper *wallpaper;
    
    //While on, no other transition, changes
    //effects will be listened to
    bool lock;
    
    
    double *idleTimer;
    
    unsigned long long lastChangeTime;
    int waitTime;
    
    
    void onRedecorateEvent( bool &e );
    
    
    vector<vector<int>> textureCombos;
    
};






