#ifndef INTERFACE_H
#define INTERFACE_H

#include "mips.h"
#include <iostream>

using namespace std;

void interface( Mips& mips );
string itobin( string i );
string rtobin( string r );
string interpretator( string* );

#endif