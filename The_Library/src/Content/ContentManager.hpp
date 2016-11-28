//
//  ContentManager.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/27/16.
//
//

#ifndef ContentManager_hpp
#define ContentManager_hpp

#include <stdio.h>

#endif /* ContentManager_hpp */


#include "ofMain.h"
#include "Contribution.hpp"
#pragma once


class ContentManager{
    
public:
    
    ContentManager();
    
    void loadContent();
    void saveContributionToFile(Contribution _c);
    
    void logNewContribution(string n, string msg);
    
    vector<Contribution> contributionList;
    
    ofEvent<Contribution> newContributionEvt;
    
    
};