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

int Automaton::getNbState(){
    if(entries.size()>0)
        return entries[0]->getSizePool();
    else
        return 0;
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

int Automaton::getNbTransitions(){
    
    int temp = 0;
    
    if(entries.size() == 0)
        return 0;
    
    vector<State*> pool = entries[0]->getPool();
    
    for(int i=0;i<pool.size();i++)
        temp += pool[i]->getNbTargets();

    return temp;
}

vector< vector<int> > Automaton::getAllTransitions(){
    return State::getAllTransitions();
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
    
    return true;
}

bool Automaton::isSynchronous(){
    return State::isSynchronous();
}

bool Automaton::sortDecrease(int a, int b){
    return b > a;
}

void Automaton::determize(){
    
    //On teste si la déterminisation est possible
    if(entries.size() == 0){
        cout<<"Impossible de determiniser"<<endl;
        return;
    }
    
    
    //On garde les états à supprimer
    vector<State*> oldPool = entries[0]->getPool();
    
    //Inialisation des variables temporaires
    vector<State*> manipulate = entries;
    vector< vector<int> > name;
    vector< vector< vector<int> > > transitions;
    vector< vector<int> > toProcess;
    vector<int> newExits, temp2;
    State *temp3;
    string temp;
    
    int j;
    
    for(int i=0;i<manipulate.size();i++){
        manipulate[i]->getTargetsAsync(temp2);
    }
    
    for(int i=0;i<temp2.size();i++){
        temp3 = entries[0]->getState(temp2[i]);
        if(find(manipulate.begin(), manipulate.end(), temp3) == manipulate.end())
            manipulate.push_back(temp3);
    }
    
    j=0;
    
    //Tant qu'on a des élément en attente
    while(manipulate.size() != 0){
    
        //On crée le nouvel élément et on liste toutes ses transitions (on enlève le cas où on aurait plusieurs fois le même état). On regarde aussi si l'état sera un état de sortie
        name.push_back(vector<int>());
        
        bool add = false;
        
        for(int i=0;i<manipulate.size();i++){
            name[j].push_back(manipulate[i]->getName());
            
            for(int k=0;k<exits.size();k++){
                if(manipulate[i]->getName() == exits[k]->getName() && !isInVector(newExits, i)){
                    newExits.push_back(j);
                    add = true;
                }
                
            }
           
        }
        determinizeUnique(name[j]);
        
        
        sort(name[j].begin(), name[j].end(), sortDecrease);
        
        
        //On ajoute les numéros de toutes ses transitions selon le symbole qui permet le passage (on enlève les transitions multiples au passage, par exemple deux transitions vers 3 avec le même symbole)
        transitions.push_back(vector< vector<int> >());
        
        for(int k=0;k<symbols;k++){
            
            transitions[j].push_back(vector<int>());
            
            for(int i=0;i<manipulate.size();i++){
                vector<int> temp2 = manipulate[i]->getTargets('a'+k);
                transitions[j][k].insert( transitions[j][k].end(), temp2.begin(), temp2.end() );
            }
            
            determinizeUnique(transitions[j][k]);
            
        }
        
        // On génère le nouveau nom de l'état, par exemple 0,1,3
        temp = determinizeGetName(name[j]);
        if(find(oldNameState.begin(), oldNameState.end(), temp) == oldNameState.end())
            oldNameState.push_back(temp);
    
        
        //On calcule le nom de chaque élément des transitions, si l'état n'existe pas on le mets en attente dans toProcess
        for(int k=0; k<transitions[j].size();k++){
            
            temp = determinizeGetName(transitions[j][k]);
            if(find(oldNameState.begin(), oldNameState.end(), temp) == oldNameState.end()){
                oldNameState.push_back(temp);
                toProcess.push_back(transitions[j][k]);
            }
        }
        
        //On pense à vider les transitions de l'élément sur lequel on travaillait, puis on charge les transitions de l'élément suivant
        manipulate.clear();
        
       if(toProcess.size()>0){
            for(int i=0;i<toProcess[0].size();i++){
                manipulate.push_back(entries[0]->getState(toProcess[0][i]));
            }
           toProcess.erase(toProcess.begin());
        }
        j++;
    }
    
    
    //On efface les entrées et sorties de même que les anciens états
    entries.clear();
    exits.clear();
    
    for(int i=0; i<oldPool.size();i++){
        delete oldPool[i];
    }
    
    oldPool.clear();
    
    
    //On ajoute les nouveaux états et les nouvelles transitions et on marque les états de sorties.
    for(int i=0;i<transitions.size();i++){
        if(i == 0)
            addEntry(0);
        
        for(int j=0;j<transitions[i].size();j++){
            addTransition(i, 'a'+j, determizeGetNewName(oldNameState, transitions[i][j]) );
        }
        
    }
    
    for(int i=0;i<newExits.size();i++)
        addExit(newExits[i]);
    
    
}

int Automaton::determizeGetNewName(vector<string> &a, vector<int> &b){
    string temp = determinizeGetName(b);
    
    for(int i=0;i<a.size();i++){
        if(a[i] == temp)
            return i;
    }
    
    return -1;
}

string Automaton::determinizeGetName(vector<int> &a){
    string name = "";
    
    for(int i=0;i<a.size();i++)
        name += (i==0)?patch::to_string(a[i]): (","+patch::to_string(a[i]));
    
    return name;
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

bool Automaton::isInVector(vector<int> &a, int b){
    for(int i=0;i<a.size();i++){
        if(a[i] == b)
            return true;
    }
    return false;
}

bool Automaton::isComplete()
{
    if(entries.size() == 0)
    {
        cout<<"Error"<<endl;
        return false;
    }
    
    vector<State*> oldPool = entries[0]->getPool();
    
    for(int i = 0;i<oldPool.size(); i++)
    {
        if(oldPool[i]->enoughTransitions(this->symbols).size() != 0)
            return false;
    }
    
    return true;
}

void Automaton::complete()
{
    if(entries.size() == 0)
    {
        cout<<"Error"<<endl;
        return;
    }
    //un automate est complet ssi il y a au moins une transition sortante pour chaque état
    //s'il n'y a pas de transition sortante, on crée un nouvelle état P "poubelle"
    vector<State*> oldPool = entries[0]->getPool();
    vector <char> temp;
    State* dustbin = NULL;
    
    for(int i = 0;i<oldPool.size(); i++)
    {
        temp = oldPool[i]->enoughTransitions(this->symbols);
        for(int j = 0; j<temp.size(); j++)
        {
            if(!dustbin){
                dustbin = new State();
                for(int k=0;k<this->symbols;k++)
                    dustbin->addTarget('a'+k, dustbin);
            }
            oldPool[i]->addTarget(temp[j], dustbin);
        }
        temp.clear();
    }
    
    return;
}

bool Automaton::isWordValid(string a){
    
    if(entries.size() == 0){
        cout<<"Error"<<endl;
        return false;
    }
    
    vector<State*> temp;
    
    for(int i=0;i<entries.size();i++){
        temp = entries[i]->validWord(a);
        for(int j=0;j<temp.size();j++){
            if(find(this->exits.begin(), this->exits.end(), temp[j]) != this->exits.end())
                return true;
        }
    }
    
    return false;
    
}

ostream &operator<<(ostream& os, const Automaton& a){
    return os << State::showAll();
}


