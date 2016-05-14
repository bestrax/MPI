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

bool Automaton::sortDecrease(int a, int b){
    return b > a;
}

void Automaton::determize(){
    
    if(entries.size() == 0 || !isSynchronous()){
        cout<<"Impossible de determiniser"<<endl;
        return;
    }
    
    vector<State*> oldPool = entries[0]->getPool();
    
    
    vector<State*> manipulate = entries;
    vector<vector<int>> name;
    vector<vector<vector<int>>> transitions;
    vector<vector<int>> toProcess;
    vector<string> nameState;
    string temp;
    
    int j=0;
    
    cout<<"\nDeterminisation"<<endl;
    
    while(manipulate.size() != 0){
    
        name.push_back(vector<int>());
        
        for(int i=0;i<manipulate.size();i++){
            name[j].push_back(manipulate[i]->getName());
        }
        determinizeUnique(name[j]);
        
        sort(name[j].begin(), name[j].end(), sortDecrease);
        
        transitions.push_back(vector<vector<int>>());
        
        for(int k=0;k<symbols;k++){
            
            transitions[j].push_back(vector<int>());
            
            for(int i=0;i<manipulate.size();i++){
                vector<int> temp2 = manipulate[i]->getTargerts('a'+k);
                transitions[j][k].insert( transitions[j][k].end(), temp2.begin(), temp2.end() );
            }
            
            determinizeUnique(transitions[j][k]);
        }
        
        
        temp = determinizeGetName(name[j]);
        if(find(nameState.begin(), nameState.end(), temp) == nameState.end())
            nameState.push_back(temp);
        
        
        cout<<nameState[j] << " | ";
        
        for(int k=0; k<transitions[j].size();k++){
            cout<<(char)('a'+k)<< " : ";
            for(int i=0; i<transitions[j][k].size();i++)
                cout<<transitions[j][k][i] << " ";
            
            temp = determinizeGetName(transitions[j][k]);
            if(find(nameState.begin(), nameState.end(), temp) == nameState.end()){
                nameState.push_back(temp);
                toProcess.push_back(transitions[j][k]);
            }
        }
        
        cout<<endl;
        
        manipulate.clear();
        
       if(toProcess.size()>0){
            for(int i=0;i<toProcess[0].size();i++){
                manipulate.push_back(entries[0]->getState(toProcess[0][i]));
            }
           toProcess.erase(toProcess.begin());
        }
        j++;
    }

    
    //entry = new State(j);
    
    
}

string Automaton::determinizeGetName(vector<int> &a){
    string nameState = "";
    
    for(int i=0;i<a.size();i++)
        nameState += to_string(a[i]);
    
    return nameState;
}

void Automaton::determinizeUnique(vector<int> &a){
    set<int> b;
    
    for(vector<int>::iterator i=a.begin(); i!=a.end();){
        if(!b.insert(*i).second)
            i = a.erase(i);
        else
            i++;
    }
}

ostream &operator<<(ostream& os, const Automaton& a){
    return os << State::showAll();
}


