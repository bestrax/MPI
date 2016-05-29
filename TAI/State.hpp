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
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

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
    void getTargetsAsync(vector<int> &name);
    void getTargetsAsync(State* a, vector<int> &name);
    int getNbTargets();
    State* getState(int name);
    static string showAll();
    static vector< vector<int> > getAllTransitions();
    static void sortTransitions();
    static bool isSynchronous();
    vector <State*> getPool();
    int getSizePool();
    vector <char> enoughTransitions(const int nbSymbols);
    vector<State*> validWord(string a);
    
    
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

// Afin de régler le problème des vieux GCC (par exemple ceux disponible à l'école...)
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


#endif /* State_hpp */
