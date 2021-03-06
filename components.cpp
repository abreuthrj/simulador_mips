#include "components.h"
#include <math.h>

int ALU::Result(int data01, int data02)
{
    switch( ALUControl )
    {
        case 0:
            return ( data01 && data02 );
        break;
        
        case 1:
            return ( data01 || data02 );
        break;

        case 2:
            return data01+data02;
        break;

        case 6:
            return data01-data02;
        break;

        case 7:
            return ( data01 < data02 ? 1:0 );
        break;

        case 4:
            return bintodec( dectobin(data02,32).substr(4).append( 4,'0' ) );
        break;

        default: break;
    }
    // cout << "NONE" << endl;
    return 0;
}
void UC::GenerateSignals(int OpCode)
{
    switch( OpCode )
    {
        case 0: // R-Type
            RegDst = 1;
            ALUSrc = 0;
            MemtoReg = 0;
            RegWrite = 1;
            MemRead = 0;
            MemWrite = 0;
            Branch = 0;
            ALUOp = 2;
        break;

        case 8: // addi
            RegDst = 0;
            ALUSrc = 1;
            MemtoReg = 0;
            RegWrite = 1;
            MemRead = 0;
            MemWrite = 0;
            Branch = 0;
            ALUOp = 0;
        break;

        case 35: // lw
            RegDst = 0;
            ALUSrc = 1;
            MemtoReg = 1;
            RegWrite = 1;
            MemRead = 1;
            MemWrite = 0;
            Branch = 0;
            ALUOp = 0;
        break;

        case 43: // sw
            ALUSrc = 1;
            RegWrite = 0;
            MemRead = 0;
            MemWrite = 1;
            Branch = 0;
            ALUOp = 0;
        break;

        case 4: // beq
            ALUSrc = 0;
            RegWrite = 0;
            MemRead = 0;
            MemWrite = 0;
            Branch = 1;
            ALUOp = 1;
        break;

        default: break;
    }
}

size_t bintodec(string bin)
{
    size_t decimal = 0;

    for( int i=0; i < bin.length(); i++ )
    {
        if( bin[i]-'0' == 1 )
            decimal += pow( 2, bin.length()-i-1 );
    }

    return decimal;
}
long int bintosigned(string bin)
{
    long int decimal = 0;

    for( int i=1; i < bin.length(); i++ )
    {
        if( bin[i]-'0' == 1 )
            decimal += pow( 2, bin.length()-i-1 );
    }

    if( bin[0] == '1' )
        decimal *= -1;

    return decimal;
}

string dectobin(size_t dec)
{
    string bin;

    for( ; dec / 2 != 0; dec /= 2 )
        bin.insert( 0, 1, '0'+(dec%2) );
    bin.insert( 0, 1, '0'+(dec%2) );

    return bin;
}
string dectobin(size_t dec, int fillLeft)
{
    string bin;

    for( ; dec / 2 != 0; dec /= 2 )
        bin.insert( 0, 1, '0'+(dec%2) );
    bin.insert( 0, 1, '0'+(dec%2) );

    while( bin.length() < fillLeft )
        bin.insert( 0, 1, '0' );

    return bin;
}
string signedtobin(long int dec, int fillLeft)
{
    string bin;
    long int copy = dec;

    for( ; copy / 2 != 0; copy /= 2 )
        bin.insert( 0, 1, '0'+(abs(copy%2)) );
    bin.insert( 0, 1, '0'+(abs(copy%2)) );

    while( bin.length() < fillLeft-1 )
        bin.insert( 0, 1, '0' );

    if( dec < 0 )
        bin.insert( 0, 1, '1' );
    else
        bin.insert( 0, 1, '0' );

    return bin;
}