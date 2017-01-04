//
//  ContributionManager.hpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/27/16.
//
//

#ifndef ContributionManager_hpp
#define ContributionManager_hpp

#include <stdio.h>

#endif /* ContributionManager_hpp */


#include "ofMain.h"
#include "Contribution.hpp"
#pragma once


class ContributionManager{
    
public:
    
    ContributionManager();
    
    void loadContent();
    void saveContributionToFile(Contribution _c);
    
    void logNewContribution(string n, string tag, string msg);
    
    vector<Contribution> contributionList;
    
    ofEvent<Contribution> newContributionEvt;
    
    
};