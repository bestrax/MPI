//
//  Automaton.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "Automaton.hpp"


Automaton::Automaton(int symbols){
    entries = vector< State* >();
    exits = vector< State* >();
    this->symbols = symbols;
}

Automaton::~Automaton(){
    for(int i=0;i<entries.size();i++)
        delete entries[i];
}

void Automaton::setSymbols(int symbols){
    this->symbols = symbols;
}

int Automaton::getSymbols(){
    return symbols;
}

void Automaton::addEntry(int name){
    State* init = getState(name);
    if(!init)
        init = new State(name);
    entries.push_back(init);
}

void Automaton::addExit(int name){
    State* init = getState(name);
    if(!init)
        init = new State(name);
    exits.push_back(init);
}

vector< State* > Automaton::getEntries(){
    return entries;
}

vector< State* > Automaton::getExits(){
    return exits;
}

State* Automaton::getState(int name){
    State *a = NULL;
    for(int i=0;i<entries.size();i++){
        a = entries[i]->getState(name);
        if(a != NULL)
            return a;
    }
    
    return NULL;
}

bool Automaton::addTransition(int origin, char symbol, int destination){
    State* init = NULL, *dest = NULL;
    
    init = getState(origin);
    dest = getState(destination);
    
    if(!init)
        return false;
    
    if(!dest)
        dest = new State(destination);
    
    init->addTarget(symbol, dest);
    
    //cout<< State::showAll();
    
    return true;
}

bool Automaton::isSynchronous(){
    return State::isSynchronous();
}

ostream &operator<<(ostream& os, const Automaton& a){
    return os << State::showAll();
}


