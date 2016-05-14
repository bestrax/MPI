//
//  State.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "State.hpp"

vector< State* > State::pool = vector< State* > ();

State::State(int name){
    transitions = vector<destination>();
    this->name = name;
    
    State::pool.push_back(this);
}

State::~State(){    
    for(int i=0; i<State::pool.size();i++){
        if(State::pool[i] == this)
            State::pool.erase(State::pool.begin()+i);
    }
}

void State::addTarget(char symbol, State* target){
    
    bool exist = false;
    
    for(int i=0;i<transitions.size();i++){
        if(transitions[i].symbol == symbol){
            transitions[i].targets.push_back(target);
            exist = true;
        }
    }
    
    if(!exist){
        destination a;
        
        a.symbol = symbol;
        a.targets.push_back(target);

        transitions.push_back(a);
    }
    
    
}

int State::getName(){
    return this->name;
}

vector<int> State::getTargerts(char symbol){
    
    vector<int> name;
    for(int i=0;i<transitions.size();i++){
        if(transitions[i].symbol == symbol){
            for(int j=0;j<transitions[i].targets.size();j++)
                name.push_back(transitions[i].targets[j]->name);
        }
    }
    
    return name;
}

State* State::getState(int name){
    
    for(int i=0; i<State::pool.size();i++){
        if(State::pool[i]->name == name)
            return State::pool[i];
    }
    
    return NULL;
}

string State::showAll(){
    
    string os;
    
    State::sortTransitions();
    
    for(int i=0; i<State::pool.size();i++){
        os +=  to_string(State::pool[i]->name) + " | ";
        for(int j=0;j <State::pool[i]->transitions.size();j++){
            for(int k=0; k< State::pool[i]->transitions[j].targets.size();k++)
                os += string(1, State::pool[i]->transitions[j].symbol) + " : " + to_string(State::pool[i]->transitions[j].targets[k]->name) + "  ";
        }
        
        os += "\n";
    }
    
    return os;
}

bool State::sort(destination &a, destination &b) {
    return a.symbol < b.symbol;
}

void State::sortTransitions(){
    for(int i=0; i<State::pool.size();i++)
        std::sort (State::pool[i]->transitions.begin(), State::pool[i]->transitions.end(), sort);
}

bool State::isSynchronous(){
    for(int i=0;i<State::pool.size();i++){
        for(int j=0;j<State::pool[i]->transitions.size();j++){
            if(State::pool[i]->transitions[j].symbol == '*')
                return false;
        }
    }
    
    return true;
}

vector <State*> State::getPool(){
    return State::pool;
}