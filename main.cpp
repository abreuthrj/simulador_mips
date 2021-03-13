#include <iostream>
#include <fstream>
#include "mips.h"
#include <time.h>
#include "interface.h"

using namespace std;

int main( int argc, char** argv )
{
    srand(time(NULL));

    string fname = "entrada.txt";
    if( argc > 1 )
    {
        if( ((string)(argv[1])).compare("genbin") == 0 )
        {
            string a[4];
            a[0] = argv[2];
            a[1] = argv[3];
            if( argc > 4 )
            {
                a[2] = argv[4];
                a[3] = argv[5];
            }
            cout << interpretator( a ) << endl;
            return 0;
        }
        fname = argv[1];
    }

    ifstream file(fname);

    if( !file.is_open() )
    {
        cout << "Arquivo não encontrado (arquivo padrão: entrada.txt) || mips <arquivo>" << endl;
        return -1;
    }

    Mips simulador(file);

    interface( simulador );

    // simulador.Run();

    // simulador.PrintReg();
    // simulador.PrintMem();

    return 0;
}