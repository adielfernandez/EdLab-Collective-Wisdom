//
//  Contribution.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/27/16.
//
//

#ifndef Message_hpp
#define Message_hpp

#include <stdio.h>

#endif /* Contribution_hpp */


#include "ofMain.h"
#pragma once


class Contribution{
    
public:
    
    Contribution();
    
    void setMessage(string _name, string _tag, string _msg);
    
    string name;
    string tag;
    string message;
    
    //unique ID for reference
    //when cycling in old contributions
    int ID;
    
    //archived means not currently in a book
    bool bIsArchived = true;

    
    //only relevant to contributed messages
    unsigned int birthTime = 0;
    
    //only relevant to scholar messages
    bool bIsScholar = false;
    int scholarNum = -1;

    
};