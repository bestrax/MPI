//
//  State.hpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct destination;

typedef struct destination destination;

class State {
    
public:
    State(int name = -1);
    ~State();
    void addTarget(char symbol, State* target);
    int getName();
    vector<int> getTargets(char symbol);
    int getNbTargets();
    State* getState(int name);
    static string showAll();
    static vector<vector<int>> getAllTransitions();
    static void sortTransitions();
    static bool isSynchronous();
    vector <State*> getPool();
    int getSizePool();
    
    
    
private:
    int name;
    vector< destination > transitions;
    static vector< State* > pool;
    
    static bool sort(destination &a, destination &b);
    
};

struct destination {
    vector< State* > targets;
    char symbol;
};



#endif /* State_hpp */
