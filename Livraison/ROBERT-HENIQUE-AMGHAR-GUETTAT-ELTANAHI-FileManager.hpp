//
//  FileManager.hpp
//  TAI
//

#ifndef FileManager_hpp
#define FileManager_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ROBERT-HENIQUE-AMGHAR-GUETTAT-ELTANAHI-Automaton.hpp"

using namespace std;

class FileManager {

public:
    FileManager();
    Automaton* load(string const src);
    bool save(Automaton &a, string const src);

};


#endif /* FileManager_hpp */
