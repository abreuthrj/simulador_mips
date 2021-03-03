#include "lib.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

Mips::Mips()
{
    imem = 0;

    for( pc=0; pc < 32; pc++ )
        registers_bank[ pc ] = rand()%10;
}

Mips::Mips(ifstream& file)
{
    if( !file.is_open() )
    {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    string instruction;
    imem = 0;

    while( !file.eof() )
    {
        getline(file,instruction);

        if( instruction[0] != '#' )
        {

        cout << "Read: " << instruction << endl;

        int decoded_instruction = bintodec(instruction);

        // cout << "Decoded: " << decoded_instruction << endl;

        // cout << "Bin val: " << dectobin(decoded_instruction) << endl;

        memory[imem] = decoded_instruction;
        imem++;

        }

    }

    for( pc=0; pc < 32; pc++ )
        registers_bank[ pc ] = rand()%10;

}

Mips::~Mips()
{

}

void Mips::Run()
{
    ALU alu;
    for( int i=0; i < imem; i++ )
    {
        print();
        Instruction instruction = decode(memory[i]);

        // GERA SINAIS BASEADO NO OPCODE
        control.GenerateSignals(instruction.opcode);
        // cout << "ALUOp: " << control.ALUOp << endl;

        // LÊ REGISTRADOR EM RS BITS->BYTES
        int s1 = registers_bank[ instruction.rs ];

        // LÊ REGISTRADOR EM RS BITS->BYTES
        int s2 = registers_bank[ instruction.rt ];

        cout << "t1: " << s1 << endl;
        cout << "t2: " << s2 << endl;

        int destiny;
        // CASO REGDST = 0 ESCREVE RT NO REGISTRADOR
        if( control.RegDst == 0 )
            destiny = instruction.rt;

        // CASO REGDST = 1 ESCREVE RD NO REGISTRADOR
        if( control.RegDst == 1 )
            destiny =  instruction.rd;

        // cout << "COntrol REgdst: " << control.RegDst << endl;
        cout << "s0: " << registers_bank[ destiny ] << endl;

        // LEVA OS BITS DE ADDRESS PARA O EXTENSOR DE SINAL
        string addressbin = dectobin( instruction.address, 16 );
        char signalbit = addressbin[0];
        while( addressbin.size() < 32 )
            addressbin.insert( 0, 1, signalbit );
        int signalExtended = bintodec( addressbin );

        // LEVA CAMPO FUNCT PARA A ALU CONTROL
        alu.ALUControl = ALUControl( instruction.funct );

        // PASSA OS DADOS PARA A ALU E OBTEM O RESULTADO
        int ALUResult;
        if( control.ALUSrc == 0 ) 
            ALUResult = alu.Result( s1,s2 );

        // cout << "ALU SRC: " << control.ALUSrc << endl;
        // cout << "ALU Op: " << alu.ALUOp << endl;
        cout << "result = " << ALUResult << endl;

        // cout << "ALUSrc: " << control.ALUSrc << endl;

        if( control.ALUSrc == 1 )
            ALUResult = alu.Result( s1,signalExtended );

        // SHIFT LEFT 2
        string shiftLeftbin = dectobin( signalExtended );
        shiftLeftbin = shiftLeftbin.substr( 2 );
        shiftLeftbin.append( 1,'0' );
        shiftLeftbin.append( 1,'0' );

        int shiftLeft = bintodec( shiftLeftbin );

        // SOMA DO PC + 4 COM O SHIFT LEFT DA ALU
        // APENAS SE ZERO DA ALU
        if( control.Branch && ALUResult == 0 )
            i += shiftLeft/8;

        if( control.MemWrite == 1 )
            memory[ ALUResult/4 ] = s2;

        if( control.MemtoReg == 1 && control.MemRead == 1 )
            registers_bank[ destiny ] = memory[ ALUResult/4 ];
        
        if( control.MemtoReg == 0 )
            registers_bank[ destiny ] = ALUResult;

        print();

    }
}

int Mips::bintodec(string bin)
{
    int decimal = 0;

    for( int i=0; i < bin.length(); i++ )
    {
        if( bin[i]-'0' == 1 )
            decimal += pow( 2, bin.length()-i-1 );
    }

    return decimal;
}
int Mips::bintosigned(string bin)
{
    int decimal = 0;

    for( int i=1; i < bin.length(); i++ )
    {
        if( bin[i]-'0' == 1 )
            decimal += pow( 2, bin.length()-i-1 );
    }

    if( bin[0] == '1' )
        decimal *= -1;

    return decimal;
}

string Mips::dectobin(int dec)
{
    string bin;

    for( ; dec / 2 != 0; dec /= 2 )
        bin.insert( 0, 1, '0'+(dec%2) );
    bin.insert( 0, 1, '0'+(dec%2) );

    return bin;
}
string Mips::dectobin(int dec, int fillLeft)
{
    string bin;

    for( ; dec / 2 != 0; dec /= 2 )
        bin.insert( 0, 1, '0'+(dec%2) );
    bin.insert( 0, 1, '0'+(dec%2) );

    while( bin.length() < fillLeft )
        bin.insert( 0, 1, '0' );

    return bin;
}
string Mips::signedtobin(int dec, int fillLeft)
{
    string bin;

    for( ; dec / 2 != 0; dec /= 2 )
        bin.insert( 0, 1, '0'+(dec%2) );
    bin.insert( 0, 1, '0'+(dec%2) );

    while( bin.length() < fillLeft-1 )
        bin.insert( 0, 1, '0' );

    if( dec < 0 )
        bin.insert( 0, 1, '1' );
    else
        bin.insert( 0, 1, '0' );

    return bin;
}

Instruction Mips::decode(int instruction)
{
    string ibin = dectobin(instruction,32);

    // cout << ibin.substr( 0, 6 ) << endl;
    // cout << ibin.substr( 6, 5 ) << endl;
    // cout << ibin.substr( 11, 5 ) << endl;
    // cout << ibin.substr( 16, 5 ) << endl;
    // cout << ibin.substr( 21, 5 ) << endl;
    // cout << ibin.substr( 26 ) << endl;

    int OpCode =  bintodec( ibin.substr( 0, 6 ) );

    Instruction i;

    i.opcode = OpCode;

    cout << "Opcode: " << OpCode << endl;

    i.rs = bintodec( ibin.substr( 6, 5 ) );
    i.rt = bintodec( ibin.substr( 11, 5 ) );
    i.rd = bintodec( ibin.substr( 16, 5 ) );
    i.shamt = bintodec( ibin.substr( 21, 5 ) );
    i.funct = bintodec( ibin.substr( 26 ) );

    i.address = bintodec( ibin.substr( 6 ) );
    i.immediate = bintodec( ibin.substr( 16 ) );

    if( OpCode == 0 )
        cout << ( i.funct == 0 ? 
                    "add":
                    i.funct == 2 ?
                        "sub":
                        i.funct == 4 ?
                            "and":
                            i.funct == 5 ?
                                "or":
                                i.funct == 10 ?
                                    "slt":
                                    "undefined"
                 )
            << " " << i.rd << ", " << i.rs << ", " << i.rt << ";" << endl;


    return i;

    // switch( OpCode )
    // {
    //     case 0:
    //         i.type = 'r';
    //         i.rs = bintodec( ibin.substr( 6, 5 ) );
    //         i.rt = bintodec( ibin.substr( 11, 5 ) );
    //         i.rd = bintodec( ibin.substr( 16, 5 ) );
    //         i.shamt = bintodec( ibin.substr( 21, 5 ) );
    //         i.funct = bintodec( ibin.substr( 26 ) );
    //     break;

    //     case 4:
    //         i.type = 'j';
    //         i.address = bintodec( ibin.substr( 6 ) );
    //     break;

    //     case 35:
    //     case 43:
    //         i.type = 'i';
    //         i.rs = bintodec( ibin.substr( 6, 5 ) );
    //         i.rt = bintodec( ibin.substr( 11, 5 ) );
    //         i.immediate = bintodec( ibin.substr( 16 ) );
    //     break;

    //     default: 
    //         cout << "OpCode desconhecido: " << OpCode << endl;
    //         exit(0);
    //     break;
    // }

    // cout << "Instrução do tipo: " << (char)('A'+(i.type-'a')) << endl;

}

int Mips::ALUControl(int funct)
{
    int ALUControl;

    funct = bintodec ( dectobin( funct, 6 ).substr( 2 ) ); // PEGA APENAS OS 4 ultimos BITS

    switch( control.ALUOp )
    {
        case 0: // LOAD OU STORE WORD
            ALUControl = bintodec("0010");
        break;
            
        case 1: // BRANCH EQUAL E BRANCH NOT EQUAL
            ALUControl = bintodec("0110");
        break;

        case 2: // INSTRUÇÃO R-TYPE
            switch( funct )
            {
                case 0: // ADD
                    ALUControl = bintodec("0010");
                break;

                case 2: // SUB
                    ALUControl = bintodec("0110");
                break;
                    
                case 4: // AND
                    ALUControl = bintodec("0000");
                break;

                case 5: // OR
                    ALUControl = bintodec("0001");
                break;

                case 10: // SLT
                    ALUControl = bintodec("0111");
                break;

                default: break;
            }
        break;

        default: break;
    }
    return ALUControl;
}

void Mips::print()
{
    cout << "======= REGISTER BANK =======" << endl;

    for( int i=0; i < 32; i++ )
    {
        cout << registers_bank[i] << " " << ( (i+1)%4 == 0 ? "\n":"" );
    }

    cout << endl;
}