//
//  State.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "State.hpp"

vector< State* > State::pool = vector< State* > ();

/*
* function : Constructeur par défaut
*/
State::State(int name){
    transitions = vector<destination>();
    this->name = name;
    
    State::pool.push_back(this);
}

/*
* function : Destructeur par défaut
*/
State::~State(){    
    for(int i=0; i<State::pool.size();i++){
        if(State::pool[i] == this){
            State::pool.erase(State::pool.begin()+i);
            return;
        }
    }
}

/*
* function : On ajoute une transtion
*/
void State::addTarget(char symbol, State* target){
    
    bool exist = false;
    
    //On cherche la transition avec ce symbole et on y ajoute un nouvel état de destination
    for(int i=0;i<transitions.size();i++){
        if(transitions[i].symbol == symbol){
            transitions[i].targets.push_back(target);
            exist = true;
        }
    }
    
    //Si la transition n'existe pas on la crée et on y ajoute un état de destination
    if(!exist){
        destination a;
        
        a.symbol = symbol;
        a.targets.push_back(target);

        transitions.push_back(a);
    }
    
    
}

/*
* function : Getter, retourne le nom de l'état
*/
int State::getName(){
    return this->name;
}

/*
* function : Retourne toutes les nom des états cibles accessible depuis les transitions y 
             compris celles accessible en asynchrone
*/
vector<int> State::getTargets(char symbol){
    
    vector<int> name;
    for(int i=0;i<transitions.size();i++){
        
        for(int j=0;j<transitions[i].targets.size();j++){
            
            if(transitions[i].symbol == symbol && find(name.begin(), name.end(), transitions[i].targets[j]->name) == name.end()){
                name.push_back(transitions[i].targets[j]->name);
                getTargetsAsync(transitions[i].targets[j], name);
            }
            
        }
    }
    
    return name;
}

/*
* function : Retourne les noms des états cibles accessible seulement avec un epsilon (fonction récursive)
*/
void State::getTargetsAsync(vector<int> &name){
    
    for(int i=0;i<transitions.size();i++){
        
        for(int j=0;j<transitions[i].targets.size();j++){
            if(transitions[i].symbol == '*'){
                name.push_back(transitions[i].targets[j]->name);
                transitions[i].targets[j]->getTargetsAsync(name);
            }
        }
    }
    
}

/*
 * function : Retourne les noms des états cibles accessible seulement avec un epsilon (fonction récursive). Depuis
              un état défini
 */
void State::getTargetsAsync(State* a, vector<int> &name){
    
    for(int i=0;i<a->transitions.size();i++){
        
        for(int j=0;j<a->transitions[i].targets.size();j++){
            if(a->transitions[i].symbol == '*'){
                name.push_back(a->transitions[i].targets[j]->name);
                a->transitions[i].targets[j]->getTargetsAsync(name);
            }
        }
    }

}

/*
* function : Retourne le nombre de symbole ayant des transitions
*/
int State::getNbTargets(){
    
    int temp = 0;
    for(int i=0;i<transitions.size();i++)
        temp += transitions[i].targets.size();
    
    return temp;
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
        os +=  patch::to_string(State::pool[i]->name) + " | ";
        for(int j=0;j <State::pool[i]->transitions.size();j++){
            for(int k=0; k< State::pool[i]->transitions[j].targets.size();k++)
                os += string(1, State::pool[i]->transitions[j].symbol) + " : " + patch::to_string(State::pool[i]->transitions[j].targets[k]->name) + "  ";
        }
        
        os += "\n";
    }
    
    return os;
}

vector< vector<int> > State::getAllTransitions(){
    vector<vector<int>> a;
    int temp;
    
    for(int i=0; i<State::pool.size();i++){
        
        for(int j=0;j <State::pool[i]->transitions.size();j++){
            for(int k=0; k< State::pool[i]->transitions[j].targets.size();k++){
                a.push_back(vector<int>());
                temp = (int)a.size()-1;
                a[temp].push_back(State::pool[i]->name);
                a[temp].push_back(State::pool[i]->transitions[j].symbol);
                a[temp].push_back(State::pool[i]->transitions[j].targets[k]->name);
            }
        }
    }
    
    return a;
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

int State::getSizePool(){
    return (int)State::pool.size();
}

/*
* function : Retourne les symboles des transitions n'existant pas ou ne disposant pas d'états cibles
*/
vector <char> State::enoughTransitions(const int nbSymbols)
{
    vector <char> nameState;
    if(transitions.size()<nbSymbols){
        for(int i=0;i<nbSymbols;i++){
            
            bool exist = false;
            for(int j=0; j<transitions.size() && !exist ;j++){
                if(transitions[j].symbol == 'a'+i){
                    exist = true;
                    if(transitions[j].targets.size() == 0)
                        nameState.push_back(transitions[j].symbol);
                }
            }
            if(!exist)
                nameState.push_back('a'+i);
            
        }
    }
    return nameState;
}

/*
* function : Verifie si un mot est valide en récursif
*/
vector<State*> State::validWord(string a){
    
    vector<State*> temp, temp2;
    string b;
    
    //On sauvegarde notre chaine initiale mais aussi la chaine accessible depuis les prochains états
    b = a;
    b.erase(0,1);
    
    //Dans chaque transitions on envoie notre string raccourcit d'un caractère si le symbole correspond à notre premier
    //symbole du string ou si l'on a affaire à une transition epsilon
    for(int i=0;i<transitions.size();i++){
        
        if(a.size()>0 && transitions[i].symbol == a[0]){
            for(int j=0; j<transitions[i].targets.size();j++){
                temp2 = transitions[i].targets[j]->validWord(b);
                temp.insert(temp.end(), temp2.begin(), temp2.end());
                temp2.clear();
            }
            
        }
        else if(transitions[i].symbol == '*'){
            for(int j=0; j<transitions[i].targets.size();j++){
                temp2 = transitions[i].targets[j]->validWord(a);
                temp.insert(temp.end(), temp2.begin(), temp2.end());
                temp2.clear();
            }
            
        }
        
    }
    
    //Si notre string est vide et que l'on ai pas déjà des états finaux alors on ajoute l'état courant et
    //on retourne le table d'états finaux
    if(a.size() == 0 && temp.size() == 0){
        temp.push_back(this);
        return temp;
    }
    
    return temp;
    
}
