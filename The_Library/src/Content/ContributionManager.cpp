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

    //open the directory with messages
    ofDirectory messageDir;
    messageDir.listDir("messages");
    
    
    for(int i = 0; i < (int)messageDir.size(); i++){
    
        //get each text file
        ofBuffer buff = ofBufferFromFile(messageDir.getPath(i));
        
        string name = "";
        string tag = "";
        string message = "";

        
        if(buff.size()) {
            
            int lineNum = 0;

            //iterate through each line and pull out the data
            for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
        
                string line = *it;
                
                //first line is name
                //all others are the message
                if(lineNum == 0){
                    name = line;
                } else if(lineNum == 1){
                    tag = line;
                } else {
                    message += line;
                }
                
                lineNum++;
            }

            Contribution c;
            c.ID = contributionList.size();
            c.setMessage(name, tag, message);
            
            //make name the ID for debug
//            c.setMessage(ofToString(c.ID), tag, message);
            
            contributionList.push_back(c);
            
        }

    }
    
    //shuffle the vector so messages books dont
    //have the same order every time we run
//    std::random_shuffle(contributionList.begin(), contributionList.end());
    
    
}

void ContributionManager::logNewContribution(string n, string tag, string msg){
    
    //create a message
    Contribution c;
    c.ID = contributionList.size();
    c.setMessage(n, tag, msg);

    //make name the ID for debug
//    c.setMessage(ofToString(c.ID), tag, msg);
    
    //push it to the list
    contributionList.push_back(c);
    
    ofNotifyEvent(newContributionEvt, c, this);
    
    //also create a new text file to store it for next time
//    saveContributionToFile(c);
    
    
}

void ContributionManager::saveContributionToFile(Contribution _c){
    
    //also create a new text file to store it for next time
    string filename = "messages/" + ofGetTimestampString() + ".txt";
    
    ofBuffer buffer;
    buffer.append(_c.name);
    buffer.append("\n");
    buffer.append(_c.tag);
    buffer.append("\n");
    buffer.append(_c.message);
    
    ofBufferToFile(filename, buffer);
    
    
}





