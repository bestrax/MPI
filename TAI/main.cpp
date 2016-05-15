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
    
    cout<<"Bonjour Malik"<<endl;
    
    a = file.load("exemples/a2.txt");
    
    cout<<*a<<endl;
    
    cout<< "Is synchronous ? : "<<a->isSynchronous()<<endl<<endl;
    
    a->determize();
    
    cout<<endl<<endl;
    
    cout<<*a<<endl;
    
    file.save(*a, "exemples/output.txt");
    
    
    cout<<endl<<endl;
    
    return 0;
}
