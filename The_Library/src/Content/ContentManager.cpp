//
//  ContentManager.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 11/27/16.
//
//

#include "ContentManager.hpp"


ContentManager::ContentManager(){
    
    
}

void ContentManager::loadContent(){

    //open the directory with messages
    ofDirectory messageDir;
    messageDir.listDir("messages");
    
    
    for(int i = 0; i < (int)messageDir.size(); i++){
    
        //get each text file
        ofBuffer buff = ofBufferFromFile(messageDir.getPath(i));
        
        string name = "";
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
                } else {
                    message += line;
                }
                
                lineNum++;
            }

            Contribution c;
            c.setMessage(name, message);
            
            contributionList.push_back(c);

        }

    }
    
    //shuffle the vector so messages books dont
    //have the same order every time we run
    std::random_shuffle(contributionList.begin(), contributionList.end());
    

    //Print data from file for debug
//    for(int i = 0; i < contributionList.size(); i++){
//        cout << "Contibution [" << i << "] Name: " << contributionList[i].name << ", Message: " << contributionList[i].message <<endl;
//    }
    
}

void ContentManager::logNewContribution(string n, string msg){
    
    //create a message
    Contribution c;
    c.setMessage(n, msg);
    
    //push it to the list
    contributionList.push_back(c);
    
    ofNotifyEvent(newContributionEvt, c, this);
    
    //also create a new text file to store it for next time
//    saveContributionToFile(c);
    
    
}

void ContentManager::saveContributionToFile(Contribution _c){
    
    //also create a new text file to store it for next time
    string filename = "messages/" + ofGetTimestampString() + ".txt";
    
    ofBuffer buffer;
    buffer.append(_c.name);
    buffer.append("\n");
    buffer.append(_c.message);
    
    ofBufferToFile(filename, buffer);
    
    
}





