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
#include <algorithm>
#include <set>
#include <string>
#include "State.hpp"

using namespace std;

typedef struct element element;


class Automaton {

public:
    Automaton(int symbols = 0);
    ~Automaton();
    void setSymbols(int symbols);
    int getSymbols();
    int getNbState();
    void addEntry(int name);
    void addExit(int name);
    vector< State* > getEntries();
    vector< State* > getExits();
    State* getState(int name);
    int getNbTransitions();
    vector< vector<int> > getAllTransitions();
    bool addTransition(int origin, char symbol, int destination);
    bool isSynchronous();
    static bool sortDecrease(int a, int b);
    void determize();
    bool isComplete();
    void complete();
    bool isWordValid(string a);
    void minimalize();
    
    
    friend ostream &operator<<(ostream& os, const Automaton& a);



private:
    int symbols;
    vector< State* > entries;
    vector< State* > exits;
    vector<string> oldNameState;

    int determizeGetNewName(vector<string> &a, vector<int> &b);
    string determinizeGetName(vector<int> &a);
    void determinizeUnique(vector<int> &a);
    bool isInVector(vector<int> &a, int b);
    
    void minimalizeCompute(element *el, element *current);
    element* findInElements(element *el, State *a);
    void getTable(element *el, element *current, vector< vector< vector<int> > > &table,  vector < vector< int > > &corres);
    void showTable(vector< vector< vector<int> > > &table);
    void sortTable(vector< vector< vector<int> > > &table, vector < vector< int > > &corres);



};

struct element{
    vector< element* > els;
    vector< State* > states;
};


#endif /* Automaton_hpp */
