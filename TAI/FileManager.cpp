//
//  FileManager.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "FileManager.hpp"


FileManager::FileManager(){

}

Automaton* FileManager::load(string const src){
    Automaton* a = new Automaton();
    ifstream file(src);
    int origin, destination, nbSymb, nbState, nbTrans, temp;
    char symbol;
    
    if(!file)
        return NULL;
    
    file >> nbSymb;
    a->setSymbols(nbSymb);
    
    file >> nbState;
    
    file >> origin;
    
    for(int i=0;i<origin;i++){
        temp = -1;
        file >> temp;
        if(temp != -1)
            a->addEntry(temp);
    }
    
    file >> destination;
    
    for(int i=0;i<destination;i++){
        temp = -1;
        file >> temp;
        if(temp != -1)
            a->addExit(temp);
    }
    
    file >> nbTrans;
    
    for(int i=0;i<nbTrans;i++){
        file >> origin;
        file >> symbol;
        file >> destination;
        a->addTransition(origin, symbol, destination);
    }    
    
    file.close();
    
    return a;
}

bool FileManager::save(Automaton &a, string const src){
    
    vector<State*> temp;
    vector<vector<int>> transitions;
    ofstream file(src);
    
    if(!file)
        return false;
    
    file<< a.getSymbols()<<endl;
    file<< a.getNbState()<<endl;
    
    temp = a.getEntries();
    file<< temp.size()<<" ";
    
    for(int i=0;i<temp.size();i++)
        file<<temp[i]->getName()<<" ";
    
    file<<endl;
    
    temp = a.getExits();
    file<< temp.size()<<" ";
    
    for(int i=0;i<temp.size();i++)
        file<<temp[i]->getName()<<" ";
    
    file<<endl;
    
    file<<a.getNbTransitions()<<endl;
    
    transitions = a.getAllTransitions();
    
    for(int i=0;i<transitions.size();i++)
        file<<transitions[i][0]<<(char)(transitions[i][1])<<transitions[i][2]<<endl;
    
    file.close();
    
    return true;
    
}