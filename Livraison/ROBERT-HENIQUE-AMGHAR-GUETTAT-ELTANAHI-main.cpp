//
//  main.cpp
//  TAI
//

#include <iostream>

#include "ROBERT-HENIQUE-AMGHAR-GUETTAT-ELTANAHI-FileManager.hpp"

using namespace std;

int main(int argc, const char * argv[]) {


    //On déclare nos variables nécessaires
    FileManager file;
    Automaton *a = NULL;
    string src = "";
    short quit = 0;

    do{

        do{

            cout<<"Entrez le nom du fichier en relatif par rapport a l'executable sans l'extension .txt : ";
            cin>>src;
            //On charge l'automate
            a = file.load(src+".txt");

            cout<<endl;

            if(!a)
                cout<<"Erreur lors de la lecture."<<endl<<endl;

        }while(!a);

        //On affiche l'automate et on procède aux vérifications et aux modifications
        cout<<"L'automate a la table de transition suivante : "<<endl<<endl<<*a<<endl;

        vector< int > tmp = a->isSynchronous();

        //Vérification si l'automate est synchrone
        if(tmp.size() == 0){
            cout<<"L'automate est synchrone"<<endl<<endl;

            tmp = a->isDeter();

            //Vérificaiton si l'automate est déterministe ou pas
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

                if(tmp[0] == -2){
                    cout<<"L'automate n'est pas déterministe car il possède plusieurs entrées"<<endl<<endl;
                    tmp.erase(tmp.begin());
                }
                cout<<"L'automate n'est pas déterministe car les etats suivants possedent plusieurs etats cibles pour une meme transition :"<<endl;
                for(int i=0;i<tmp.size();i++)
                    cout<<"- "<< tmp[i]<<endl;
                cout<<endl;
                a->determize();
                a->complete();

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


        //On affiche l'automate déterministe complet et sa table de correspondance
        cout<<"L'automate deterministe complet possede la table de transition suivante :"<<endl<<endl<<*a<<endl;
        cout<<"Les etats ont ete renomes comme suit durant la determinisation :\n"<<endl<<a->getTableOldNameState()<<endl<<endl;

        //On minimalise l'automate et on affiche sa table de correspondance
        a->minimalize();
        cout<<"L'automate deterministe complet minimaliste possede la table de transition suivante :"<<endl<<endl<<*a<<endl;
        cout<<"Les etats ont ete renomes comme suit durant la minimisation par rapport aux noms des etats de l'automate deterministe :\n"<<endl<<a->getTableOldNameStateMinimalize()<<endl<<endl;


        //On effectue la vérification de mots
        string strtest = "";
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

            cout<<endl<<"Voulez vous continuer a tester des mots (1: continuer, 0: quitter, 2: charger un autre automate) : ";
            cin>>quit;


        }while(quit == 1);

        //On pense à libérer la mémoire
        delete a;
        a = NULL;
        cout<<endl<<endl;

    }while(quit == 2);



    return 0;
}
