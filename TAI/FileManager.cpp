//
//  FileManager.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "FileManager.hpp"


FileManager::FileManager(){

}

/*
* function : Fonction chargeant un automate depuis un fichier
*/
Automaton* FileManager::load(string const src){
    
    //On ouvre le fichier et on initalise un automate
    Automaton* a = new Automaton();
    ifstream file(src);
    int origin, destination, nbSymb, nbState, nbTrans, temp;
    char symbol;
    
    if(!file)
        return NULL;
    
    //On récupère le nombre de symboles, d'états et le nombre d'états d'origines
    file >> nbSymb;
    a->setSymbols(nbSymb);
    
    file >> nbState;
    
    file >> origin;
    
    //On ajoute les états d'origine
    for(int i=0;i<origin;i++){
        temp = -1;
        file >> temp;
        if(temp != -1)
            a->addEntry(temp);
    }
    
    //On charge le nombre d'états de sorties et on ajoute les états de sorties
    file >> destination;
    
    for(int i=0;i<destination;i++){
        temp = -1;
        file >> temp;
        if(temp != -1)
            a->addExit(temp);
    }
    
    
    //On charge le nombre transitions et on crée les transitions et les états nécessaires
    file >> nbTrans;
    
    for(int i=0;i<nbTrans;i++){
        file >> origin;
        file >> symbol;
        file >> destination;
        a->addTransition(origin, symbol, destination);
    }    
    
    
    //On ferme le fichier et on retourne l'automate
    file.close();
    
    return a;
}

/*
* function : Fonction qui s'occupe de sauvegarder un automate
*/
bool FileManager::save(Automaton &a, string const src){
    
    
    //On initialise nos variables et on ouvre notre fichier
    vector<State*> temp;
    vector<vector<int>> transitions;
    ofstream file(src);
    
    if(!file)
        return false;
    
    //On sauvegarde le nombre de symboles, d'états
    file<< a.getSymbols()<<endl;
    file<< a.getNbState()<<endl;
    
    
    //On écrit le nombre d'entrées et leurs numéros
    temp = a.getEntries();
    file<< temp.size()<<" ";
    
    for(int i=0;i<temp.size();i++)
        file<<temp[i]->getName()<<" ";
    
    file<<endl;
    
    //On écrit le nombre de sorties et leurs numéros
    temp = a.getExits();
    file<< temp.size()<<" ";
    
    for(int i=0;i<temp.size();i++)
        file<<temp[i]->getName()<<" ";
    
    file<<endl;
    
    //On écrit le nombre de transitions et les transitions en elles mêmes
    file<<a.getNbTransitions()<<endl;
    
    transitions = a.getAllTransitions();
    
    for(int i=0;i<transitions.size();i++)
        file<<transitions[i][0]<<(char)(transitions[i][1])<<transitions[i][2]<<endl;
    
    
    //On ferme le fichier
    file.close();
    
    return true;
    
}