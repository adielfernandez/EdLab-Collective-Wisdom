//
//  ScholarData.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/3/17.
//
//

#ifndef ScholarData_hpp
#define ScholarData_hpp

#include <stdio.h>

#endif /* ScholarData_hpp */

#include "ofMain.h"

#pragma once


struct Scholar{
    
    //name
    string nameTop;
    string nameBottom;
    string nameFull;
    
    //info
    string dates;
    string tag;
    string factSheet;
    ofColor tagCol;

    //their number in the list
    int id;
    
    //FOR USE LATER:
    //the scholar's room data
    int bookcaseTexNum;
    int frameTexNum;
    int wallpaperTexNum;
    
};


class ScholarData{
    
public:
    
    ScholarData();
    void setupData();
    
    //for splitting up tags into
    void formatTags();
    
    const int numScholars = 10;
    
    vector<string> tagList;
    vector<ofColor> tagColorList;
    vector<Scholar> scholarList;
    

};






