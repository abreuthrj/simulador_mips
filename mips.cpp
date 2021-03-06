#include "mips.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

Mips::Mips()
{
    imem = 0;
    init();
}

Mips::Mips(ifstream& file)
{   
    if( !file.is_open() )
    {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    init();

    string instruction;
    imem = 0;

    while( !file.eof() )
    {
        getline(file,instruction);

        if( instruction[0] != '#' )
        {
            cout << "Read: " << instruction << endl;

            int decoded_instruction = bintodec(instruction);

            instruction_memory[imem] = decoded_instruction;
            imem++;
        }

    }

}

void Mips::init()
{
    pc = 0;

    registers_bank[0] = 0; // $zero

    // INICIALIZA t0-t7 aleatorios
    for( int i=8; i < 16; i++ )
        registers_bank[ i ] = rand()%10;

    // INICIALIZA s0-s7 aleatorios
    for( int i=16; i < 24; i++ )
        registers_bank[ i ] = rand()%10;

    registers_bank[29] = 0; // SP
    registers_bank[28] = 0; // GP

    // INICIALIZA MEMORIA DE DADOS COM 0
    for( int i = 0; i < 256; i++ )
        data_memory[i] = 0;
}

Mips::~Mips()
{

}

void Mips::Run()
{
    PrintReg();
    for( int i=0; i < imem; i++ )
    {
        cout << "=== INSTRUCTION ===" << endl;
        proccess( instruction_memory[pc/4] );
        cout << "===================" << endl;
    }
}

void Mips::proccess( size_t inst )
{
    pc += 4;

    Instruction instruction = decode( inst );

    // GERA SINAIS BASEADO NO OPCODE
    control.GenerateSignals(instruction.opcode);
    // cout << "ALUOp: " << control.ALUOp << endl;

    // LÊ REGISTRADOR EM RS BITS->BYTES
    int s1 = registers_bank[ instruction.rs ];

    // LÊ REGISTRADOR EM RS BITS->BYTES
    int s2 = registers_bank[ instruction.rt ];

    int destiny;

    // CASO REGDST = 0 ESCREVE RT NO REGISTRADOR
    if( control.RegDst == 0 )
        destiny = instruction.rt;

    // CASO REGDST = 1 ESCREVE RD NO REGISTRADOR
    if( control.RegDst == 1 )
        destiny =  instruction.rd;

    // cout << "COntrol REgdst: " << control.RegDst << endl;

    // LEVA OS BITS DE ADDRESS PARA O EXTENSOR DE SINAL
    // DESNECESSÁRIO, JÁ QUE POR QUESTÃO DE OTIMIZAÇÃO
    // ADDRESS JÁ ESTÁ EM DECIMAL COM SINAL
    // string addressbin = signedtobin( instruction.address, 32 );
    // char signalbit = addressbin[0];
    // while( addressbin.size() < 31 )
    //     addressbin.insert( 0, 1, '0' );
    // addressbin.insert( 0, 1, signalbit );
    // int signalExtended = bintosigned( addressbin );
    int signalExtended = instruction.address;

    // LEVA CAMPO FUNCT PARA A ALU CONTROL
    alu.ALUControl = ALUControl( instruction.funct );

    // PASSA OS DADOS PARA A ALU E OBTEM O RESULTADO
    int ALUResult;
    if( control.ALUSrc == 0 ) 
        ALUResult = alu.Result( s1,s2 );

    // cout << "ALU SRC: " << control.ALUSrc << endl;
    // cout << "ALU Op: " << alu.ALUOp << endl;

    // cout << "ALUSrc: " << control.ALUSrc << endl;

    if( control.ALUSrc == 1 )
        ALUResult = alu.Result( s1,signalExtended );

    // SHIFT LEFT 2
    string shiftLeftbin = signedtobin( signalExtended, 32 );
    char bitsign = shiftLeftbin[0];
    shiftLeftbin = shiftLeftbin.substr( 3 );
    shiftLeftbin.append( 1,'0' );
    shiftLeftbin.append( 1,'0' );
    shiftLeftbin.insert( 0, 1, bitsign );

    long int shiftLeft = bintosigned( shiftLeftbin );

    // SOMA DO PC + 4 COM O SHIFT LEFT DA ALU
    // APENAS SE ZERO DA ALU
    if( control.Branch == 1 && ALUResult == 0 )
    {
        pc += shiftLeft;
        cout << "shl: " << shiftLeft/4 << endl << "pc: " << pc << endl;
    }

    if( control.MemWrite == 1 )
    {
        data_memory[ (ALUResult/4)-1 ] = s2;
        // cout << "S2: " << s2 << endl;
        // cout << "ALUResult: " << ALUResult << endl;
    }

    if( control.RegWrite == 1 )
    {
        if( control.MemtoReg == 1 && control.MemRead == 1 )
            registers_bank[ destiny ] = data_memory[ (ALUResult/4)-1 ];
        
        if( control.MemtoReg == 0 )
            registers_bank[ destiny ] = ALUResult;
    }
}

void Mips::Execute( string inst )
{
    size_t decoded = bintodec( inst );
    pc -= 4;
    proccess( decoded );
}

void Mips::Step()
{
    proccess( instruction_memory[pc/4] );
}

Instruction Mips::decode(size_t instruction)
{
    string ibin = dectobin(instruction,32);

    cout << "ibin: " << ibin << endl;

    int OpCode =  bintodec( ibin.substr( 0, 6 ) );

    Instruction i;

    i.opcode = OpCode;

    cout << "Opcode: " << OpCode << endl;

    i.rs = bintodec( ibin.substr( 6, 5 ) );
    i.rt = bintodec( ibin.substr( 11, 5 ) );
    i.rd = bintodec( ibin.substr( 16, 5 ) );
    i.shamt = bintodec( ibin.substr( 21, 5 ) );
    i.funct = bintodec( ibin.substr( 26 ) );

    // i.address = bintodec( ibin.substr( 6 ) );
    i.address = bintosigned( ibin.substr( 16 ) );

    if( OpCode == 0 )
        cout << ( i.funct == 32 ? 
                    "add "+helper[i.rd]+", "+helper[i.rs]+", "+helper[i.rt]:
                    i.funct == 34 ?
                        "sub "+helper[i.rd]+", "+helper[i.rs]+", "+helper[i.rt]:
                        i.funct == 36 ?
                            "and "+helper[i.rd]+", "+helper[i.rs]+", "+helper[i.rt]:
                            i.funct == 37 ?
                                "or "+helper[i.rd]+", "+helper[i.rs]+", "+helper[i.rt]:
                                i.funct == 42 ?
                                    "slt "+helper[i.rd]+", "+helper[i.rs]+", "+helper[i.rt]:
                                    i.funct == 0 ?
                                        "sll "+helper[i.rd]+", "+helper[i.rt]+", "+to_string(i.shamt):
                                        "undefined"
                 )
            << endl;
    else
        cout << ( OpCode == 8 ?
                    "addi "+helper[i.rt]+", "+helper[i.rs]+", "+to_string(i.address):
                        OpCode == 43 ?
                        "sw "+helper[i.rt]+", "+to_string(i.address)+"("+helper[i.rs]+")":
                            OpCode == 35 ?
                                "lw "+helper[i.rt]+", "+to_string(i.address)+"("+helper[i.rs]+")":
                                    OpCode == 4 ?
                                    "beq "+helper[i.rs]+", "+helper[i.rt]+", "+to_string(i.address):
                                    "undefined"
                 )
            << endl;


    return i;
}

int Mips::ALUControl(int funct)
{
    int ALUControl;

    switch( control.ALUOp )
    {
        case 0: // LOAD OU STORE WORD / IMMEDIATE
        case 8: // ADDI 
            ALUControl = bintodec("0010");
        break;
            
        case 1: // BRANCH EQUAL E BRANCH NOT EQUAL
            ALUControl = bintodec("0110");
        break;

        case 2: // INSTRUÇÃO R-TYPE
            switch( funct )
            {
                case 32: // ADD
                    ALUControl = bintodec("0010");
                break;

                case 34: // SUB
                    ALUControl = bintodec("0110");
                break;
                    
                case 36: // AND
                    ALUControl = bintodec("0000");
                break;

                case 37: // OR
                    ALUControl = bintodec("0001");
                break;

                case 42: // SLT
                    ALUControl = bintodec("0111");
                break;

                case 0: // SLL
                    ALUControl = bintodec("0100");
                break;

                default: break;
            }
        break;

        default: break;
    }
    return ALUControl;
}

void Mips::PrintReg()
{
    cout << "======= REGISTER BANK =======" << endl;

    cout << "$zero = " << registers_bank[0] << endl;
    
    cout << "$at = " << registers_bank[1] << endl;
    
    cout << "$v0 = " << registers_bank[2] << " ; ";
    cout << "$v1 = " << registers_bank[3] << endl;
    
    cout << "$a0 = " << registers_bank[4] << " ; ";
    cout << "$a1 = " << registers_bank[5] << " ; ";
    cout << "$a2 = " << registers_bank[6] << " ; ";
    cout << "$a3 = " << registers_bank[7] << endl;

    for( int i = 8; i < 16; i++ )
        cout << "$t" << i-8 << " = " << registers_bank[i] << ( i==15 ? "\n":" ; " );

    for( int i = 16; i < 24; i++ )
        cout << "$s" << i-16 << " = " << registers_bank[i] << ( i==23 ? "\n":" ; " );

    cout << "$t8 = " << registers_bank[24] << " ; ";
    cout << "$t9 = " << registers_bank[25] << endl;

    cout << "$k0 = " << registers_bank[26] << " ; ";
    cout << "$k1 = " << registers_bank[27] << endl;

    cout << "$gp = " << registers_bank[28] << endl;

    cout << "$sp = " << registers_bank[29] << endl;

    cout << "$fp = " << registers_bank[30] << endl;

    cout << "$ra = " << registers_bank[31] << endl;

    cout << "=============================" << endl;

    cout << endl;
}
void Mips::PrintMem()
{
    cout << "========================== DATA Memory ==========================" << endl;

    for( int i=0; i < 256; i++ )
    {
        cout << data_memory[i] << ( (i+1)%32 == 0 ? "\n":" " );
    }

    cout << "=================================================================" << endl;

    cout << endl;
}