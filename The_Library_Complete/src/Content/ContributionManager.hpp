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
#include "ScholarData.hpp"

#pragma once


class ContributionManager{
    
public:
    
    ContributionManager();
    
    void loadContent(ScholarData *data);
    void saveContributionToFile(Contribution _c);
    
    void logNewContribution(string n, int tagNum, string msg);
    
    vector<Contribution> contributionList;
    
    ScholarData *scholarData;
    
    const int numBookModels = 77;
    
    ofEvent<Contribution> newContributionEvt;
    
    
};