//
//  ContributionManager.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/27/16.
//
//

#include "ContributionManager.hpp"


ContributionManager::ContributionManager(){
    
    
}

void ContributionManager::loadContent(){

    
    //First add in all the scholars
    ofDirectory scholarDir;
    scholarDir.listDir("scholarMessages");
    
    
    for(int i = 0; i < (int)scholarDir.size(); i++){
        
        //get each text file
        ofBuffer buff = ofBufferFromFile(scholarDir.getPath(i));
        
        string name = "";
        int scholarNum = 0;
        string tag = "";
        string message = "";
        
        if(buff.size()) {
            
            int lineNum = 0;
            
            //iterate through each line and pull out the data
            for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
                
                string line = *it;
                
                //cut up the message into it's relevant parts
                if(lineNum == 0){
                    name = line;
                } else if(lineNum == 1){
                    scholarNum = ofToInt(line);
                } else if(lineNum == 2){
                    tag = line;
                } else {
                    message += line;
                }
                
                lineNum++;
            }
            
            Contribution c;
            c.ID = contributionList.size();
            c.setMessage(name, tag, message);
            c.bIsScholar = true;
            c.scholarNum = scholarNum;
            
            contributionList.push_back(c);
            
        }
        
    }
    
    
    //Now go through and add in the user messages
    ofDirectory messageDir;
    messageDir.listDir("messages");
    
    
    for(int i = 0; i < (int)messageDir.size(); i++){
    
        //get each text file
        ofBuffer buff = ofBufferFromFile(messageDir.getPath(i));
        
        string name = "";
        string tag = "";
        string message = "";
        unsigned int birthTime = 0;
        
        if(buff.size()) {
            
            int lineNum = 0;

            //iterate through each line and pull out the data
            for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
        
                string line = *it;
                
                //cut up the message into it's relevant parts
                if(lineNum == 0){
                    name = line;
                } else if(lineNum == 1){
                    tag = line;
                } else if(lineNum == 2){
                    birthTime = ofToInt(line);
                } else {
                    message += line;
                }
                
                lineNum++;
            }

            Contribution c;
            c.ID = contributionList.size();
            c.setMessage(name, tag, message);
            
            contributionList.push_back(c);
            
        }

    }
    
    
    
    //shuffle the vector so messages books dont have the same order every time we run
    //If we have more contributions than books in the library, only shuffle the first
    //portion of them to ensure that the scholars in the library don't get pushed into
    //the archive when all the books are filled up

    cout << "Number of contributions: " << contributionList.size() << endl;
    
    if(contributionList.size() > numBookModels){
        
        //sor the first bunch
        std::random_shuffle(contributionList.begin(), contributionList.begin() + (numBookModels - 1));
        
        
    } else {
        
        //we've got enough books for each message, shuffle all of them
        std::random_shuffle(contributionList.begin(), contributionList.end());
        
    }
    

    
    

    
    
    

    
    
}

void ContributionManager::logNewContribution(string n, string tag, string msg){
    
    //create a message
    Contribution c;
    c.ID = contributionList.size();
    c.setMessage(n, tag, msg);
    c.birthTime = ofGetUnixTime();
    
    //push it to the list
    contributionList.push_back(c);
    
    
    ofNotifyEvent(newContributionEvt, c, this);
    
    //also create a new text file to store it for next time
    saveContributionToFile(c);
    
    
}

void ContributionManager::saveContributionToFile(Contribution _c){
    
    //also create a new text file to store it for next time
    string filename = "messages/" + ofGetTimestampString() + ".txt";
    
    ofBuffer buffer;
    buffer.append(_c.name);
    buffer.append("\n");
    buffer.append(_c.tag);
    buffer.append("\n");
    buffer.append(ofToString(_c.birthTime));
    buffer.append("\n");
    buffer.append(_c.message);
    
    ofBufferToFile(filename, buffer);
    
    
    
}





