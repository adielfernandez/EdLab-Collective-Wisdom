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
    
    void setMessage(string _name, string _msg);
    
    string name;
    string message;
    
    
//    void operator= (const Contribution &C){
//        
//    }
    
    
};