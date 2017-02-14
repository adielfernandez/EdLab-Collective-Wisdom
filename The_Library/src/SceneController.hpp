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



#pragma once

#include "ofMain.h"
#include "Furniture/Wallpaper.hpp"
#include "Furniture/Frame.hpp"
#include "Furniture/Bookcase.hpp"

//Dont include CenterBook
//#include "Books/CenterBook.hpp"

//use forward declaration instead to
//clear circular include issue
class CenterBook;


struct SceneEvent{
    
    enum OBJECT_TYPE{
        
        WALLPAPER = 0,
        BOOKCASE_LEFT = 1,
        BOOKCASE_RIGHT = 2,
        FRAME = 3,
        CENTERBOOK = 4
        
    };
    
    OBJECT_TYPE type;
    ofVec2f pos;
    
    
};




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
    bool lockScene;
    
    bool lockBookcases;
    double lastBookcaseChangeTime;
    bool lockFrame;
    double lastFrameChangeTime;
    bool lockWallpaper;
    double lastWallpaperChangeTime;
    
    float wallpaperWait;
    float frameWait;
    float bookcaseWait;
    
    //how long has it been since
    //there's been a camera touch
    double *idleTimer;
    
    double lastChangeTime;
    float lockWaitTime;
    
    float idleTriggerWaitTime;
    
    
    vector<vector<int>> textureCombos;
    int lastTextureCombo;
    
    
    void onRedecorateEvent( SceneEvent &se );
};




#endif /* SceneController_hpp */

