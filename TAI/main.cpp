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
    
    cout<< "Is synchronous ? : "<<a->isSynchronous()<<endl<<endl;
    
    a->determize();
    
    cout<<endl<<endl;
    
    
    cout<< "Is complete ? :"<<a->isComplete()<<endl<<endl;
    a->complete();
    
    cout<<*a<<endl;
    
    cout<< "Is complete ? :"<<a->isComplete()<<endl<<endl;
    
    cout<<"Reconnu ? : "<<a->isWordValid("a")<<endl;
    
    file.save(*a, "exemples/output.txt");
    
    
    
    
    cout<<endl<<endl;
    
    a->minimalize();
    
    cout<<endl<<endl;
    
    cout<<*a<<endl;
    
    cout<<"Reconnu ? : "<<a->isWordValid("aba")<<endl;

    
    return 0;
}
