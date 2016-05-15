//
//  FileManager.hpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#ifndef FileManager_hpp
#define FileManager_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Automaton.hpp"

using namespace std;

class FileManager {
    
public:
    FileManager();
    Automaton* load(string const src);
    bool save(Automaton &a, string const src);
    
};


#endif /* FileManager_hpp */
