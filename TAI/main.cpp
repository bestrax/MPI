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
    
    a = file.load("exemples/a5.txt");
    
    cout<<*a<<endl;
    
    if(a->isSynchronous()){
        cout<<"L'automate est synchrone"<<endl<<endl;
        
        //TODO : Détecter si l'automate est déterministe ou pas
        
        if(0){
            
            cout<<"L'automate est déterministe"<<endl<<endl;
            if(a->isComplete()){
                cout<<"L'automate est complet"<<endl<<endl;
            }else{
                cout<<"L'automate n'est pas complet"<<endl<<endl;
                a->complete();
            }
            
        }else{
            
            cout<<"L'automate n'est pas déterministe"<<endl<<endl;
            a->determize();
            
        }
    }
    else
        cout<<"L'automate est asynchrone"<<endl<<endl;
    
   

   /* cout<<*a<<endl;
    
    
    cout<<"Reconnu ? : "<<a->isWordValid("a")<<endl;
    
    file.save(*a, "exemples/output.txt");
    
    
    
    
    cout<<endl<<endl;
    
    a->minimalize();
    
    cout<<endl<<endl;
    
    cout<<*a<<endl;
    
    cout<<"Reconnu ? : "<<a->isWordValid("aba")<<endl;*/

    
    return 0;
}
