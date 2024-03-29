//
//  Automaton.hpp
//  TAI
//

#ifndef Automaton_hpp
#define Automaton_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include "ROBERT-HENIQUE-AMGHAR-GUETTAT-ELTANAHI-State.hpp"

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
    vector< int > isSynchronous();
    static bool sortDecrease(int a, int b);
    vector< int > isDeter();
    void determize();
    vector< int > isComplete();
    void complete();
    bool isWordValid(string a);
    void minimalize();
    short typeState(State *a) const;
    string getTableOldNameState() const;
    string getTableOldNameStateMinimalize() const;
    string showAll() const;

    friend ostream &operator<<(ostream& os, const Automaton& a);



private:
    int symbols;
    vector< State* > entries;
    vector< State* > exits;
    vector<string> oldNameState;
    vector<string> oldNameStateMinimalize;
    vector< State* > pool;

    int determizeGetNewName(vector<string> &a, vector<int> &b);
    string determinizeGetName(vector<int> &a);
    void determinizeUnique(vector<int> &a);
    bool isInVector(vector<int> &a, int b);

    void showMinimalize(element &el);
    int minimalizeCompute(element *el, element *current);
    element* findInElements(element *el, State *a);
    void deleteMinimalizeTree(element *el);
    void getTable(element *el, element *current, vector< vector< vector<int> > > &table,  vector < vector< int > > &corres);
    void showTable(vector< vector< vector<int> > > &table);
    void showTable(vector< vector< vector<int> > > &table, vector < vector< int > > &corres);
    void sortTable(vector< vector< vector<int> > > &table, vector < vector< int > > &corres);



};

//Structure servant de noeud pour l'arbre de minimisation
struct element{
    vector< element* > els;
    vector< State* > states;
};


#endif /* Automaton_hpp */
