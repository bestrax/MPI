//
//  Automaton.hpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#ifndef Automaton_hpp
#define Automaton_hpp

#include <stdio.h>
#include <vector>
#include <iostream> 
#include "State.hpp"

using namespace std;


class Automaton {
    
public:
    Automaton(int symbols = 0);
    ~Automaton();
    void setSymbols(int symbols);
    int getSymbols();
    void addEntry(int name);
    void addExit(int name);
    vector< State* > getEntries();
    vector< State* > getExits();
    State* getState(int name);
    bool addTransition(int origin, char symbol, int destination);
    bool isSynchronous();
    
    friend ostream &operator<<(ostream& os, const Automaton& a);
    
    
    
protected:
    int symbols;
    vector< State* > entries;
    vector< State* > exits;
    
};

#endif /* Automaton_hpp */
