//
//  Automaton.cpp
//  TAI
//
//  Created by ROBERT Bastien on 14/05/2016.
//  Copyright © 2016 ROBERT Bastien. All rights reserved.
//

#include "Automaton.hpp"

/*
* function : Constructeur par défaut
*/
Automaton::Automaton(int symbols){
    entries = vector< State* >();
    exits = vector< State* >();
    this->symbols = symbols;
}

/*
* function : Destructeur par défaut
*/
Automaton::~Automaton(){
    for(int i=0;i<pool.size();i++)
        delete pool[i];
}

/*
* function : setter
*/
void Automaton::setSymbols(int symbols){
    this->symbols = symbols;
}

/*
* function : getter
*/
int Automaton::getSymbols(){
    return symbols;
}

/*
* function : getter, retourne le nombre d'états de l'automate
*/
int Automaton::getNbState(){
    return (int)pool.size();
}

/*
* function : Ajoute une entrée, si l'élement n'existe pas alors on le crée
*/
void Automaton::addEntry(int name){
    State* init = getState(name);
    if(!init){
        init = new State(name);
        pool.push_back(init);
    }
    entries.push_back(init);
}

/*
* function : Ajoute une sortie, si l'élement n'existe pas alors on le crée
*/
void Automaton::addExit(int name){
    State* init = getState(name);
    if(!init){
        init = new State(name);
        pool.push_back(init);
    }
    exits.push_back(init);
}

/*
* function : getter, retourne toutes les entrées
*/
vector< State* > Automaton::getEntries(){
    return entries;
}

/*
* function : getter, retourne toutes les sorties
*/
vector< State* > Automaton::getExits(){
    return exits;
}

/*
* function : Retourne un élément particulier depuis le pool
*/
State* Automaton::getState(int name){
    
    for(int i=0; i<pool.size();i++){
        if(pool[i]->getName() == name)
            return pool[i];
    }
    
    return NULL;
}

/*
* function : Retourne le nombre de transitions de l'automate
*/
int Automaton::getNbTransitions(){
    
    int temp = 0;
    
    if(entries.size() == 0)
        return 0;
    
    for(int i=0;i<pool.size();i++)
        temp += pool[i]->getNbTargets();

    return temp;
}

/*
* function : Retourne un tableau à 2 dimensions avec toutes les transitions
*/
vector< vector<int> > Automaton::getAllTransitions(){
    vector< vector< int > > a;
    vector < int > tmp;
    int temp;
    
    for(int i=0; i<pool.size();i++){
        
        for(int j=0;j <pool[i]->getNbTransitions();j++){
            tmp = pool[i]->getTargetsNotAsync('a'+j);
            for(int k=0; k< tmp.size();k++){
                a.push_back(vector<int>());
                temp = (int)a.size()-1;
                a[temp].push_back(pool[i]->getName());
                a[temp].push_back('a'+j);
                a[temp].push_back(tmp[j]);
            }
        }
    }
    
    return a;
}

/*
* function : Ajoute une transition et un état si besoin est
*/
bool Automaton::addTransition(int origin, char symbol, int destination){
    State* init = NULL, *dest = NULL;
    
    init = getState(origin);
    
    if(!init){
        init = new State(origin);
        pool.push_back(init);
    }
    
    dest = getState(destination);
    
    if(!dest){
        dest = new State(destination);
        pool.push_back(dest);
    }
    
    init->addTarget(symbol, dest);
    
    return true;
}

/*
* function : On teste si l'automate est synchrone
*/
vector< int > Automaton::isSynchronous(){
    vector< int > tmp;
    for(int i=0;i<pool.size();i++){
        if(pool[i]->hasAsync())
            tmp.push_back(pool[i]->getName());
    }
    
    return tmp;
}

/*
* function : Fonction nous permettant de trier par ordre décroissant un vector
*/
bool Automaton::sortDecrease(int a, int b){
    return b > a;
}

/*
* function : Teste si l'automate est déterministe
*/
vector< int > Automaton::isDeter(){
    vector< int > tmp;
    for(int i=0;i<pool.size();i++){
        if(!pool[i]->isDeter())
            tmp.push_back(pool[i]->getName());
    }
    return tmp;
}

/*
* function : Cette fonction déterminise un automate, elle s'occupe de le modifier et de sauvegarde les noms des anciens états
*/
void Automaton::determize(){
    
    //On teste si la déterminisation est possible
    if(entries.size() == 0){
        cout<<"Impossible de determiniser"<<endl;
        return;
    }
    
    //Inialisation des variables temporaires
    vector<State*> manipulate = entries;
    vector< vector<int> > name;
    vector< vector< vector<int> > > transitions;
    vector< vector<int> > toProcess;
    vector<int> newExits, temp2;
    State *temp3;
    string temp;
    
    int j;
    
    //On met en attente les entrées de l'automate et les états accessible depuis les entrées avec des transitions epsilon
    for(int i=0;i<manipulate.size();i++){
        manipulate[i]->getTargetsAsync(temp2);
    }
    
    for(int i=0;i<temp2.size();i++){
        temp3 = getState(temp2[i]);
        if(find(manipulate.begin(), manipulate.end(), temp3) == manipulate.end())
            manipulate.push_back(temp3);
    }
    
    j=0;
    
    //Tant qu'on a des élément en attente
    while(manipulate.size() != 0){
    
        //On crée le nouvel élément et on liste tous les états en attente pour faire son nom (on enlève le cas où on aurait
        //plusieurs fois le même état). On regarde aussi si l'état sera un état de sortie.
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
        if(find(oldNameState.begin(), oldNameState.end(), temp) == oldNameState.end() && temp.size() > 0)
            oldNameState.push_back(temp);
    
        
        //On calcule le nom de chaque élément des transitions, si l'état n'existe pas on le mets en attente dans toProcess
        for(int k=0; k<transitions[j].size();k++){
            
            temp = determinizeGetName(transitions[j][k]);
            if(find(oldNameState.begin(), oldNameState.end(), temp) == oldNameState.end() && temp.size() > 0){
                oldNameState.push_back(temp);
                toProcess.push_back(transitions[j][k]);
            }
        }
        
        //On pense à vider les transitions de l'élément sur lequel on travaillait, puis on charge les transitions de l'élément suivant
        manipulate.clear();
        
       if(toProcess.size()>0){
            for(int i=0;i<toProcess[0].size();i++){
                manipulate.push_back(getState(toProcess[0][i]));
            }
           toProcess.erase(toProcess.begin());
        }
        j++;
    }
    
    
    //On efface les entrées et sorties de même que les anciens états
    entries.clear();
    exits.clear();
    
    for(int i=0; i<pool.size();i++){
        delete pool[i];
    }
    
    pool.clear();
    
    
    //On ajoute les nouveaux états et les nouvelles transitions et on marque les états de sorties.
    for(int i=0;i<transitions.size();i++){
        if(i == 0)
            addEntry(0);
        
        for(int j=0;j<transitions[i].size();j++){
            if(transitions[i][j].size() > 0)
                addTransition(i, 'a'+j, determizeGetNewName(oldNameState, transitions[i][j]) );
        }
        
    }
    
    //On pense à remettre les sorties de l'automate
    for(int i=0;i<newExits.size();i++)
        addExit(newExits[i]);
    
    
}

/*
* function : Retourne le nom du nouvel état selon les états déjà créée
*/
int Automaton::determizeGetNewName(vector<string> &a, vector<int> &b){
    string temp = determinizeGetName(b);
    
    for(int i=0;i<a.size();i++){
        if(a[i] == temp)
            return i;
    }
    
    return -1;
}

/*
* function : Retourne le nom de l'état en concaténant les noms des états
*/
string Automaton::determinizeGetName(vector<int> &a){
    string name = "";
    
    std::sort (a.begin(), a.end());
    
    for(int i=0;i<a.size();i++)
        name += (i==0)?patch::to_string(a[i]): (","+patch::to_string(a[i]));
    
    return name;
}

/*
* function : Efface les doublons dans le tableau d'entier
*/
void Automaton::determinizeUnique(vector<int> &a){
    set<int> b;
    
    for(vector<int>::iterator i=a.begin(); i!=a.end();){
        if(!b.insert(*i).second)
            i = a.erase(i);
        else
            i++;
    }
}

/*
* function : Vérifie si l'élément b se trouve dans le vector a
*/
bool Automaton::isInVector(vector<int> &a, int b){
    for(int i=0;i<a.size();i++){
        if(a[i] == b)
            return true;
    }
    return false;
}

/*
* function : Vérifie si l'automate est complet
*/
vector< int > Automaton::isComplete()
{
    if(entries.size() == 0)
    {
        cout<<"L'automate ne possede pas d'entree"<<endl;
        return vector< int >();
    }
    
    vector< int > tmp;
    
    //On vérifie que chaque état dispose de toutes ses transitions
    for(int i = 0;i<pool.size(); i++)
    {
        if(pool[i]->enoughTransitions(this->symbols).size() != 0)
            tmp.push_back(pool[i]->getName());
    }
    
    return tmp;
}

/*
* function : Fonction qui complète un automate
*/
void Automaton::complete()
{
    if(entries.size() == 0)
    {
        cout<<"L'automate ne possede pas d'entree"<<endl;
        return;
    }
    
    //un automate est complet ssi il y a au moins une transition sortante pour chaque état
    //s'il n'y a pas de transition sortante, on crée un nouvelle état P "poubelle"
    vector <char> temp;
    State* dustbin = NULL;
    
    //On parcourt chaque état pour vérifier s'il possede une transition vide, si c'est le cas on fait pointer la transition vers
    //la poubelle
    for(int i = 0;i<pool.size(); i++)
    {
        temp = pool[i]->enoughTransitions(this->symbols);
        for(int j = 0; j<temp.size(); j++)
        {
            if(!dustbin){
                int max=-1;
                for(int k=0;k<pool.size();k++){
                    if(pool[k]->getName() >= max)
                        max = pool[k]->getName()+1;
                }
                dustbin = new State(max);
                pool.push_back(dustbin);
                for(int k=0;k<this->symbols;k++)
                    dustbin->addTarget('a'+k, dustbin);
            }
            pool[i]->addTarget(temp[j], dustbin);
        }
        temp.clear();
    }
    
    return;
}

/*
* function : Fonction qui vérifie si un mot est accepté par l'automate
*/
bool Automaton::isWordValid(string a){
    
    if(entries.size() == 0){
        cout<<"L'automate ne possede pas d'entree"<<endl;
        return false;
    }
    
    vector<State*> temp;
    
    //On lance sur toutes entrées l'analyse pour voir si le mot est accepté,
    //On vérifie que les états où on s'est arrêté car le mot était lu en entier sont des états de sorties
    for(int i=0;i<entries.size();i++){
        temp = entries[i]->validWord(a);
        for(int j=0;j<temp.size();j++){
            if(find(this->exits.begin(), this->exits.end(), temp[j]) != this->exits.end())
                return true;
        }
    }
    
    return false;
    
}

/*
* function : Fonction qui minimalise un automate
*/
void Automaton::minimalize(){
    
    if(this->entries.size() < 1){
        cout<<"L'automate n'a pas d'entree."<<endl;
        exit(0);
    }
    
    //On définit nos variables temporaires
    element *el = new element;
    string tmp;
    vector< vector< vector< int > > > table;
    vector < vector< int > > corres;
    vector< int > newExits;
    vector< int > newEntries;
    
    //On divise nos états en deux parties sous forme d'un arbre :
    //Les états terminaux et les états non terminaux
    for(int i=0;i<pool.size();i++){
        if(find(this->exits.begin(), this->exits.end(), pool[i]) == this->exits.end()){
            if(el->els.size() == 0)
                el->els.push_back(new element);
            
            el->els[0]->states.push_back(pool[i]);
        }
        else{
            if(el->els.size() == 0){
                el->els.push_back(new element);
                el->els.push_back(new element);
            }else if(el->els.size() == 1){
                el->els.push_back(new element);
            }
            el->els[1]->states.push_back(pool[i]);
        }
    }
    
    //On lance la minimisation
    minimalizeCompute(el, el);
    
    //On crée la table de transitions et la table avec les anciens noms (table de correspondance) à l'aide de
    //l'arbre créée lors de la minimisation
    getTable(el, el, table, corres);
    sortTable(table, corres);
    
    //On sauvegarde les états d'entrés pour les trouver dans les nouveaux états
    for(int i=0;i<entries.size();i++){
        for(int j=0;j<corres.size();j++){
            if(find(corres[j].begin(), corres[j].end(), entries[i]->getName()) != corres[j].end())
                newEntries.push_back(j);
        }
    }
    
    //On sauvegarde les états de sorties pour les trouver dans les nouveaux états
    for(int i=0;i<exits.size();i++){
        for(int j=0;j<corres.size();j++){
            if(find(corres[j].begin(), corres[j].end(), exits[i]->getName()) != corres[j].end())
                newExits.push_back(j);
        }
    }
    
    //On efface les entrées et sorties de même que les anciens états
    entries.clear();
    exits.clear();
    
    for(int i=0; i<pool.size();i++){
        delete pool[i];
    }
    
    pool.clear();
    
    //On ajoute les nouveaux états et les nouvelles transitions et on marque les états de sorties et d'entrés
    for(int i=0;i<table.size();i++){
        tmp = "";
        
        for(int j=0;j<corres[i].size();j++)
            tmp += patch::to_string(corres[i][j]) + (j==corres[i].size()-1?"":", ");
        
        oldNameStateMinimalize.push_back(tmp);
        
        for(int j=0;j<table[i].size();j++){
            addTransition(i, 'a'+j, table[i][j][0] );
        }
        
    }
    
    for(int i=0;i<newEntries.size();i++)
        addEntry(newEntries[i]);
    
    for(int i=0;i<newExits.size();i++)
        addExit(newExits[i]);
    
    deleteMinimalizeTree(el);
}


/*
* function : Affiche l'automate depuis l'arbre de minimalisation
*/
void Automaton::showMinimalize(element &el){
    
    vector< vector< vector< int > > > table;
    vector < vector< int > > corres;
    
    cout<<"Etape de minimisation : "<<endl<<endl;
    
    //On crée la table de transitions et la table avec les anciens noms (table de correspondance) à l'aide de
    //l'arbre créée lors de la minimisation
    getTable(&el, &el, table, corres);
    sortTable(table, corres);
    showTable(table, corres);
    
    cout<<endl<<endl<<endl;
}

/*
* function : Fonction récursive qui crée un arbre pour minimaliser un automate
*/
void Automaton::minimalizeCompute(element *el, element *current){
    
    //Si on est sur une feuille on quitte
    if(current->states.size() == 1)
        return;
    
    //Si on est sur un noeud alors on descend en récursif sur nos éléments enfants
    if(current->states.size() == 0){
        for(int i=0;i<current->els.size();i++){
            minimalizeCompute(el, current->els[i]);
        }
        return;
    }
    
    //On définit nos variables temporaires
    vector< element* > dest;
    bool found;
    vector< State* > tmp;
    element * tmp2;
    int nberase;
    
    //Tant que l'on traite des éléments on continue
    do{
        
        //On vide nos vector temporaires
        tmp.clear();
        dest.clear();
        
        //On enregistre les transitions partant de notre première transition pour chaque symbole
        for(int i=0;i<this->symbols;i++)
            dest.push_back( findInElements(el, getState( current->states[0]->getTargets('a'+i)[0])) );
        
        //On ajoute la première transition pour la déplacer quoi qu'il arrive
        tmp.push_back(current->states[0]);
        
        
        //On parcourt toutes nos autres transitions et on regarde si nos transitions sortantes
        //sont les mêmes et dans le même ordre que nos transitions sortantes de notre première transition
        for(int i=1;i<current->states.size();i++){
            found = true;
            for(int j=0;j<this->symbols;j++){
                if(findInElements(el, getState( current->states[i]->getTargets('a'+j)[0])) != dest[j])
                    found = false;
            }
            
            if(found)
                tmp.push_back(current->states[i]);

        }
        
        //Si on a une transition à déplacer on lefface et on l'ajoute en élément enfant de notre noeud courant
        if((tmp.size() != current->states.size() || tmp.size() == 1 ) && current->states.size() > 0){
            tmp2 = new element;
            nberase = 0;
            current->els.push_back(tmp2);
            for(int i=0; i<tmp.size();i++){
                
                tmp2->states.push_back(tmp[i]);
                nberase = 0;
                for(int j=0; j<current->states.size();j++){
                    if(current->states[j] == tmp[i]){
                        current->states.erase(current->states.begin()+j-nberase);
                        nberase++;
                    }
                }
            }
        }
        
    }while((tmp.size() != current->states.size() || tmp.size() == 1 ) && current->states.size() > 0);
    
    //On affiche la partition actuelle
    showMinimalize((*el));
    
    //Si on on a des éléments enfants mais aussi des états alors on met les
    //états dans un élément enfant
    
    if(current->els.size() > 0 && current->states.size() > 0){
        tmp2 = new element;
        current->els.push_back(tmp2);
        for(int i=0; i<current->states.size();i++){
            
            tmp2->states.push_back(current->states[i]);
            nberase = 0;
            for(int j=0; j<current->states.size();j++){
                if(current->states[j] == tmp[i]){
                    current->states.erase(current->states.begin()+j-nberase);
                    nberase++;
                }
            }
        }
    }
    
    //On lance notre minimalisation en récursif pour nos éléments enfants
    for(int i=0;i<current->els.size();i++)
        minimalizeCompute(el, current->els[i]);
    
}

/*
* function : Fonction qui recherche dans un arbre un état en particulier
*/
element* Automaton::findInElements(element *el, State *a){
    
    for(int i=0;i<el->states.size();i++){
        if(el->states[i] == a)
            return el;
    }
    
    element * tmp = NULL;
    
    for(int i=0;i<el->els.size();i++){
        tmp = findInElements(el->els[i], a);
        if(tmp != NULL)
            return tmp;
    }
    
    return NULL;
}

/*
 * function : Fonction qui détruit l'arbre de minimisation
 */
void Automaton::deleteMinimalizeTree(element *el){
    
    for(int i=0;i<el->els.size();i++){
        deleteMinimalizeTree(el->els[i]);
    }
    
    delete el;
}

/*
* function : Retourne la table de transition et la table de correspondance depuis l'arbre de minimisation (fonction récursive)
*/
void Automaton::getTable(element *el, element *current, vector< vector< vector<int> > > &table,  vector < vector< int > > &corres){
    
    //Si on est sur noeud avec des enfants alors on continue sur les enfants
    if(current->states.size() == 0){
        for(int i=0;i<current->els.size();i++){
            getTable(el, current->els[i], table, corres);
        }
        return;
    }
    
    //On rajoute une ligne à notre table de transition et de correspondance
    table.push_back(vector< vector<int> >());
    corres.push_back(vector< int >());
    
    //On rajoute le nom de tous nos état pour la table de correspondance
    for(int i=0;i<current->states.size();i++)
        corres[corres.size()-1].push_back(current->states[i]->getName());
    
    //On rajoute nos transitions à la table de transition
    for(int j=0;j<this->symbols;j++){
        
        table[table.size()-1].push_back(vector<int>());
        
        table[table.size()-1][j].push_back( findInElements( el, getState( current->states[0]->getTargets('a'+j)[0]) )->states[0]->getName() );
    }
    
}


/*
* function : Affiche la table de transitions
*/
void Automaton::showTable(vector< vector< vector<int> > > &table){
    for(int i=0;i<table.size();i++){
        
        cout<<i<<" | ";
        
        for(int j=0;j<table[i].size();j++){
            
            cout<<(char)('a'+j)<<" : ";
            
            for(int k=0;k< table[i][j].size();k++)
                cout<<table[i][j][k]<<" ";
            
        }
        cout <<"\n";
    }
}

/*
 * function : Affiche la table de transitions à l'aide de la table de correspondance
 */
void Automaton::showTable(vector< vector< vector<int> > > &table, vector < vector< int > > &corres){
    
    for(int i=0;i<table.size();i++){
        
        cout<<corres[i][0];
        for(int l=1;l<corres[i].size();l++)
            cout<<", "<<corres[i][l];
        cout<<" | ";
        
        for(int j=0;j<table[i].size();j++){
            
            cout<<(char)('a'+j)<<" : ";
            
            for(int k=0;k< table[i][j].size();k++)
                cout<<table[i][j][k]<<" ";
            
        }
        cout <<"\n";
    }
}

/*
* function : Remplace la liste de toutes les transitions par le nouveau nom de l'état compris dans la table de correspondance
*/
void Automaton::sortTable(vector< vector< vector<int> > > &table, vector < vector< int > > &corres){
    
    for(int i=0;i<table.size();i++){
        
        for(int j=0;j<table[i].size();j++){
            
            for(int k=0;k<table[i][j].size();k++){
                
                for(int n=0;n<corres.size();n++){
                    if(table[i][j][k] == corres[n][0]){
                        table[i][j][k] = n;
                        n = (int) corres.size();
                    }
                }
                
            }
                
            
        }
        
    }
    
}

/*
* function : Retourne 0 si ce n'est ni une sortie ni  une entrée, 1 si c'est une entrée et 2 si c'est une sortie
             et 3 si c'est une entrée et une sortie
*/
short Automaton::typeState(State *a) const{
    short tmp = 0;
    
    for(int i=0;i<entries.size();i++){
        if(entries[i] == a)
            tmp = 1;
    }
    
    for(int i=0;i<exits.size();i++){
        if(exits[i] == a){
            if(tmp == 1)
                return 3;
            else
                return 2;
        }
    }
    
    return tmp;
}

/*
* function : Affiche la table de correspondance entre l'automate non deterministe
             et l'automate deterministe
*/
string Automaton::getTableOldNameState() const{
    string os;
    
    for(int i=0;i<oldNameState.size();i++)
        os += patch::to_string(i) + " : " + oldNameState[i] + "\n";
    
    return os;
}

/*
 * function : Affiche la table de correspondance entre l'automate deterministe
              et l'automate minimal
 */
string Automaton::getTableOldNameStateMinimalize() const{
    string os;
    
    for(int i=0;i<oldNameStateMinimalize.size();i++)
        os += patch::to_string(i) + " : " + oldNameStateMinimalize[i] + "\n";
    
    return os;
}

/*
* function : Génère un string représentant la table de transition
*/
string Automaton::showAll() const{
    
    string os;
    vector< int > tmp;
    
    for(int i=0; i<pool.size();i++){
        pool[i]->sortTransitions();
        
        switch (typeState(pool[i])) {
            case 1:
                os +=  patch::to_string(pool[i]->getName()) + "(E)   | ";
                break;
                
            case 2:
                os +=  patch::to_string(pool[i]->getName()) + "(S)   | ";
                break;
            
            case 3:
                os +=  patch::to_string(pool[i]->getName()) + "(E/S) | ";
                break;
                
            default:
                os +=  patch::to_string(pool[i]->getName()) + "      | ";
                break;
        }
        
        for(int j=0;j <pool[i]->getNbTransitions()-(int)pool[i]->hasAsync();j++){
            tmp = pool[i]->getTargetsNotAsync('a'+j);
            for(int k=0; k< tmp.size();k++)
                os += string(1, 'a'+j) + " : " + patch::to_string(tmp[k]) + "  ";
            
            tmp.clear();
        }
        
        if(pool[i]->hasAsync()){
            tmp = pool[i]->getTargetsNotAsync('*');
            for(int k=0; k< tmp.size();k++)
                os += string(1, '*') + " : " + patch::to_string(tmp[k]) + "  ";
            
            tmp.clear();
        }
        
        os += "\n";
    }
    
    return os;
}

/*
* function : Surcharge de l'opérateur de sortie
*/
ostream &operator<<(ostream& os, const Automaton& a){
    return os << a.showAll();
}


