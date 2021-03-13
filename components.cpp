#include "components.h"
#include <math.h>

long int ALU::tick()
{
    switch( ALUControl )
    {
        case 0:
            return ( data1 && data2 );
        break;
        
        case 1:
            return ( data1 || data2 );
        break;

        case 2:
            return data1+data2;
        break;

        case 6:
            return data1-data2;
        break;

        case 7:
            return ( data1 < data2 ? 1:0 );
        break;

        case 4:
            return bintodec( dectobin(data2,32).substr(4).append( 4,'0' ) );
        break;

        default: break;
    }
    return 0;
}
void UC::GenerateSignals(int OpCode)
{
    switch( OpCode )
    {
        case 0: // R-Type
            PCWriteCond = 0;
            PCWrite = 0;
            IorD = 0;
            MemRead = 0;
            MemWrite = 0;
            MemtoReg = 0;
            IRWrite = 1;
            PCSource = 0;
            ALUOp = 2;
            ALUSrcB = 0;
            ALUSrcA = 1;
            RegWrite = 1;
            RegDst = 1;
            cout << "R-Type" << endl;
        break;

        case 8: // addi
            PCWriteCond = 0;
            PCWrite = 0;
            IorD = 0;
            MemRead = 0;
            MemWrite = 0;
            MemtoReg = 0;
            IRWrite = 1;
            PCSource = 2;
            ALUOp = 0;
            ALUSrcB = 2;
            ALUSrcA = 1;
            RegWrite = 1;
            RegDst = 0;
            cout << "Immediate-Type" << endl;
        break;

        case 35: // lw
            PCWriteCond = 0;
            PCWrite = 0;
            IorD = 1;
            MemRead = 1;
            MemWrite = 0;
            MemtoReg = 1;
            IRWrite = 1;
            PCSource = 2;
            ALUOp = 0;
            ALUSrcB = 2;
            ALUSrcA = 1;
            RegWrite = 1;
            RegDst = 1;
            cout << "Immediate-Type ( LW )" << endl;
        break;

        case 43: // sw
            PCWriteCond = 0;
            PCWrite = 0;
            IorD = 1;
            MemRead = 0;
            MemWrite = 1;
            MemtoReg = 0;
            IRWrite = 1;
            PCSource = 2;
            ALUOp = 0;
            ALUSrcB = 2;
            ALUSrcA = 1;
            RegWrite = 0;
            RegDst = 0;
            cout << "Immediate-Type ( SW )" << endl;
        break;

        case 5: // bne
        case 4: // beq
            PCWriteCond = 1;
            PCWrite = 0;
            IorD = 0;
            MemRead = 0;
            MemWrite = 0;
            MemtoReg = 0;
            IRWrite = 1;
            PCSource = 1;
            ALUOp = 1;
            ALUSrcB = 0;
            ALUSrcA = 1;
            RegWrite = 0;
            RegDst = 0;
            cout << "Branch-Type" << endl;
        break;

        case 3: // JAL
        case 2:
            PCWriteCond = 0;
            PCWrite = 1;
            IorD = 0;
            MemRead = 0;
            MemWrite = 0;
            MemtoReg = 0;
            IRWrite = 1;
            PCSource = 2;
            ALUOp = 1;
            ALUSrcB = 0;
            ALUSrcA = 1;
            RegWrite = 0;
            RegDst = 0;
            cout << "Jump-Type" << endl;
        break;

        default: break;
    }
}
void UC::Print()
{
    cout << "PCWriteCond: " << PCWriteCond << endl;
    cout << "PCWrite: " << PCWrite << endl;
    cout << "IorD: " << IorD << endl;
    cout << "MemRead: " << MemRead << endl;
    cout << "MemWrite: " << MemWrite << endl;
    cout << "MemtoReg: " << MemtoReg << endl;
    cout << "IRWrite: " << IRWrite << endl;
    cout << "PCSource: " << PCSource << endl;
    cout << "ALUOp: " << ALUOp << endl;
    cout << "ALUSrcB: " << ALUSrcB << endl;
    cout << "ALUSrcA: " << ALUSrcA << endl;
    cout << "RegWrite: " << RegWrite << endl;
    cout << "RegDst: " << RegDst << endl;
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

    for( ; dec / 2 != 0 && bin.length() < fillLeft-1; dec /= 2 )
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