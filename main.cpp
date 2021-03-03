#include <iostream>
#include <fstream>
#include "lib.h"
#include <time.h>

using namespace std;

int main( int argc, char** argv )
{
    srand(time(NULL));

    string fname = "entradas/entrada_01.txt";
    ifstream file(fname);

    Mips simulador(file);

    simulador.Run();

    return 0;
}