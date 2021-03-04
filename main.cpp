#include <iostream>
#include <fstream>
#include "mips.h"
#include <time.h>
#include "interface.h"

using namespace std;

int main( int argc, char** argv )
{
    srand(time(NULL));

    string fname = "entradas/entrada_01.txt";
    ifstream file(fname);

    Mips simulador(file);

    interface( simulador );

    // simulador.Run();

    // simulador.PrintReg();
    // simulador.PrintMem();

    return 0;
}