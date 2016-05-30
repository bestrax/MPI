//
//  main.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include <iostream>

#include "FileManager.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    FileManager file;
    Automaton *a = NULL;
    
    a = file.load("exemples/a1.txt");
    
    if(!a){
        cout<<"Erreur lors de la lecture."<<endl<<endl;
        exit(0);
    }
    
    cout<<"L'automate a la table de transition suivante : "<<endl<<endl<<*a<<endl;
    
    vector< int > tmp = a->isSynchronous();
    
    if(tmp.size() == 0){
        cout<<"L'automate est synchrone"<<endl<<endl;
        
        tmp = a->isDeter();
        
        if(tmp.size() == 0){
            
            cout<<"L'automate est deterministe"<<endl<<endl;
            tmp = a->isComplete();
            if(tmp.size() == 0){
                cout<<"L'automate est complet"<<endl<<endl;
            }else{
                cout<<"L'automate n'est pas complet car les etats suivants possedent au moins une transition manquante :"<<endl;
                for(int i=0;i<tmp.size();i++)
                    cout<<"- "<< tmp[i]<<endl;
                cout<<endl;
                
                a->complete();
            }
            
        }else{
            
            cout<<"L'automate n'est pas déterministe car les etats suivants possedent plusieurs etats cibles pour une meme transition :"<<endl;
            for(int i=0;i<tmp.size();i++)
                cout<<"- "<< tmp[i]<<endl;
            cout<<endl;
            a->determize();
            
        }
    }
    else{
        cout<<"L'automate est asynchrone car les etats suivants possedent au moins une transition epsilon : "<<endl;
        for(int i=0;i<tmp.size();i++)
            cout<<"- "<< tmp[i]<<endl;
        
        cout<<endl;
        a->determize();
        a->complete();
    }
    
    
    cout<<"L'automate deterministe complet possede la table de transition suivante :"<<endl<<endl<<*a<<endl;
    cout<<"Les etats ont ete renomes comme suit durant la determinisation :\n"<<endl<<a->getTableOldNameState()<<endl<<endl;
    
    a->minimalize();
   

    cout<<"L'automate deterministe complet minimaliste possede la table de transition suivante :"<<endl<<endl<<*a<<endl;
    
    cout<<"Les etats ont ete renomes comme suit durant la minimisation par rapport aux noms des etats de l'automate deterministe :\n"<<endl<<a->getTableOldNameStateMinimalize()<<endl<<endl;
    
    string strtest = "";
    short quit = 0;
    char e = 1;
    
    do{
    
        strtest = "";
        cout<<endl<<"Entrez le mot a tester : ";
#ifdef __MINGW32__
        fflush(stdin);
#endif
#ifdef __APPLE__
        fpurge(stdin);
#endif
        e = 1;
        strtest = "";
        while(e != '\n' && e != '\0' && e != '\xff') {
            e = getchar();
            if(e != '\n' && e != '\0' && e != '\xff')
                strtest += e;
        }        
        
        if(a->isWordValid(strtest))
            cout<<endl<<"Le mot est accepte";
        else
            cout<<endl<<"Le mot n'est pas accepte";
        
        cout<<endl<<"Voulez vous continuer a tester des mots (1: continuer, 0: quitter) : ";
        cin>>quit;
        
        
    }while(quit == 1);

   /*
    
    cout<<"Reconnu ? : "<<a->isWordValid("a")<<endl;
    
    file.save(*a, "exemples/output.txt");
    
    
    
    
    cout<<endl<<endl;
    
    a->minimalize();
    
    cout<<endl<<endl;
    
    cout<<*a<<endl;
    
    cout<<"Reconnu ? : "<<a->isWordValid("aba")<<endl;*/

    
    return 0;
}
