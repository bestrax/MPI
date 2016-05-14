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

Automaton* FileManager::load(string src){
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
    
    return a;
}