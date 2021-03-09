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

            long int decoded_instruction = bintodec(instruction);

            _memory.bank[imem] = decoded_instruction;
            imem++;
        }

    }

}

void Mips::init()
{
    pc = 0;

    _registers.bank[0] = 0; // $zero

    // INICIALIZA t0-t7 aleatorios
    for( int i=8; i < 16; i++ )
        _registers.bank[ i ] = rand()%10;

    // INICIALIZA s0-s7 aleatorios
    for( int i=16; i < 24; i++ )
        _registers.bank[ i ] = rand()%10;

    _registers.bank[29] = 256*4; // SP
    _registers.bank[28] = 0; // GP

    // INICIALIZA MEMORIA DE DADOS COM 0
    for( int i = 0; i < 256; i++ )
        _memory.bank[i] = -1;
}

Mips::~Mips()
{

}

void Mips::Run()
{
    for( int i=0; pc/4 < imem; i++ )
    {
        proccess();
    }
}

void Mips::proccess()
{
    cout << "=== INSTRUCTION ===" << endl;

    _cclock = 0;
    etapa01();
    cout << endl << "Ciclos de Clock: " << _cclock << endl;

    cout << "===================" << endl << endl;
}

void Mips::etapa01()
{
    pc += 4;
    cout << endl << "== ETAPA 01 ==" << endl;
    _cclock++;

    _control.MemRead = 1;
    _control.IRWrite = 1;
    _control.IorD = 0;
    _control.ALUSrcA = 0;
    _control.ALUSrcB = 1;
    _control.ALUOp = 0;
    _control.PCSource = 0;
    _control.PCWrite = 1;

    long int mem_address = ( _control.IorD == 0 ? (pc/4)-1:_registers.ALUOut );

    if( _control.IRWrite == 1 && _control.MemRead == 1 )
        _registers.IR = _memory.bank[ mem_address ];

    cout << "pc: " << pc << endl;
    cout << "mem_add: " << mem_address << endl;
    cout << "mem_data: " <<  _registers.IR << endl;
    cout << "mem_data_bin: " << dectobin( _registers.IR, 32 ) << endl;

    etapa02();
}
void Mips::etapa02()
{
    cout << endl << "== ETAPA 02 ==" << endl;
    _cclock++;
    /** 2° ETAPA */
    _control.ALUSrcA = 0;
    _control.ALUSrcB = 3;
    _control.ALUOp = 0;

    _decoded = decode( _registers.IR );
    
    _registers.A = _registers.bank[ _decoded.rs ];
    _registers.B = _registers.bank[ _decoded.rt ];

    cout << "carregado de " << _registers.helper[ _decoded.rs ] << ": " << _registers.A << endl;
    cout << "carregado de " << _registers.helper[ _decoded.rt ] << ": " << _registers.B << endl;

    // if( _control.PCSource == 2 )
        _registers.ALUOut = bintodec( dectobin( pc,8 ).substr( 0,4 ) + dectobin( _decoded.immediate, 26 ) + "00" );

    _control.GenerateSignals( _decoded.opcode );

    etapa03();
}
void Mips::etapa03()
{
    cout << endl << "== ETAPA 03 ==" << endl;
    _cclock++;

        string ExtendedBin = signedtobin( _decoded.immediate, 26 ); // EXTENDE O SINAL PARA 32 BITS
        char Signal_Backup = ExtendedBin[0]; // SALVA O BIT DE SINAL
        long int SignExtdShiftLeft = bintodec( ExtendedBin.append( 2,'0' ).substr(3).insert( 0,1,Signal_Backup ) ); // SHIFT LEFT 2 BITS

    if( _control.PCWrite == 1 && _control.PCSource == 2 )
    {
        pc = bintodec( dectobin( pc,8 ).substr(0,4) + signedtobin( SignExtdShiftLeft, 28 ) );
        cout << "Jumped to pc: " << pc << endl;
        return;
    }

    _alu.data1 = ( _control.ALUSrcA == 0 ? pc:_registers.A );
    _alu.data2 = ( _control.ALUSrcB == 0 ? _registers.B:
                   _control.ALUSrcB == 1 ? 4: 
                   _control.ALUSrcB == 2 ? _decoded.immediate: // SINAL 32 EXENDIDO
                   SignExtdShiftLeft // EXTENDED 2 SHIFT LEFT 
                 );

    cout << "ALU SrcA: " << _control.ALUSrcA << endl << "ALU SrcB: " << _control.ALUSrcB << endl;
    cout << "ALU Data1: " << _alu.data1 << endl << "ALU Data2: " << _alu.data2 << endl;

    _alu.ALUControl = ALUControl( _decoded.funct );
    
    long int Alu_Result = _alu.tick();

    cout << "ALU Result: " << Alu_Result << endl;

    if( _control.PCWriteCond == 1 )
    {
        if( ( _decoded.opcode == 4 && Alu_Result == 0 ) || ( _decoded.opcode == 5 && Alu_Result != 0 ) )
        {
            if( _control.PCSource == 0 )
                pc = Alu_Result;
            else if( _control.PCSource == 1 )
                pc = _registers.ALUOut;
            cout << "Branched to pc: " << pc << endl;
        }            
        return;
    }

    _registers.ALUOut = Alu_Result;

    etapa04();
}
void Mips::etapa04()
{
    cout << endl << "== ETAPA 04 ==" << endl;
    _cclock++;

    if( _control.MemWrite == 1 )
        if( _control.IorD == 1 )
        {
            _memory.bank[ _registers.ALUOut/4 ] = _registers.B;
            cout << "salvo em memoria: " << _registers.B << endl;
            return;
        }

    if( _control.MemRead == 1 )
        if( _control.IorD == 1 )
        {
            _registers.MDR = _memory.bank[ _registers.ALUOut/4 ];
            cout << "lido da memoria: " << _registers.MDR << endl;
            etapa05();
            return;
        }

    if( _control.RegWrite == 1 )
    {
        short int iReg = ( _control.RegDst == 0 ? _decoded.rt:_decoded.rd );
        if( _control.MemtoReg == 0 )
        {
            _registers.bank[ iReg ] = _registers.ALUOut;
            cout << "salvo em registrador " << _registers.helper[ iReg ] << ": " << _registers.ALUOut << endl;
        }
        else
        {
            _registers.bank[ iReg ] = _registers.MDR;
            cout << "salvo em registrador " << _registers.helper[ iReg ] << ": " << _registers.MDR << endl;
        }
        return;
    }    
}
void Mips::etapa05()
{
    _cclock++;
    cout << endl << "== ETAPA 05 ==" << endl;
    if( _control.RegWrite == 1 && _control.MemtoReg == 1 )
    {
        _registers.bank[ _decoded.rt ] = _registers.MDR;
        cout << "salvo em registrador " << _registers.helper[ _decoded.rt ] << ": " << _registers.MDR << endl;
    }
}

void Mips::Execute( string inst )
{
    size_t decoded = bintodec( inst );
    int pc_backup = pc;

    pc += 4;
    size_t backup = _memory.bank[ (pc/4)-1 ];
    _memory.bank[ (pc/4)-1 ] = decoded;
    pc -= 4;

    proccess();

    pc = pc_backup;
    _memory.bank[ (pc/4) ] = backup;

}

void Mips::Step()
{
    if( _memory.bank[ pc/4 ] == -1 )
    {
        cout << "Instrução vazia" << endl;
        return;
    }
    proccess();
}

Instruction Mips::decode(size_t instruction)
{
    string ibin = dectobin(instruction,32);

    int OpCode =  bintodec( ibin.substr( 0, 6 ) );

    Instruction i;

    i.opcode = OpCode;

    cout << "Opcode: " << OpCode << endl;

    i.rs = bintodec( ibin.substr( 6, 5 ) );
    i.rt = bintodec( ibin.substr( 11, 5 ) );
    i.rd = bintodec( ibin.substr( 16, 5 ) );
    i.shamt = bintodec( ibin.substr( 21, 5 ) );
    i.funct = bintodec( ibin.substr( 26 ) );

    i.immediate = bintosigned( ibin.substr( 16 ) );
    i.address = bintosigned( ibin.substr( 6 ) );

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
                    "addi "+helper[i.rt]+", "+helper[i.rs]+", "+to_string(i.immediate):
                        OpCode == 43 ?
                        "sw "+helper[i.rt]+", "+to_string(i.immediate)+"("+helper[i.rs]+")":
                            OpCode == 35 ?
                                "lw "+helper[i.rt]+", "+to_string(i.immediate)+"("+helper[i.rs]+")":
                                    OpCode == 4 ?
                                    "beq "+helper[i.rs]+", "+helper[i.rt]+", "+to_string(i.immediate):
                                        OpCode == 5 ?
                                        "bne "+helper[i.rs]+", "+helper[i.rt]+", "+to_string(i.immediate):
                                            OpCode == 2 ?
                                            "j "+to_string(i.address):
                                            "undefined"
                 )
            << endl;


    return i;
}

int Mips::ALUControl(int funct)
{
    int ALUControl;

    switch( _control.ALUOp )
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

    cout << "$zero = " << _registers.bank[0] << endl;
    
    cout << "$at = " << _registers.bank[1] << endl;
    
    cout << "$v0 = " << _registers.bank[2] << " ; ";
    cout << "$v1 = " << _registers.bank[3] << endl;
    
    cout << "$a0 = " << _registers.bank[4] << " ; ";
    cout << "$a1 = " << _registers.bank[5] << " ; ";
    cout << "$a2 = " << _registers.bank[6] << " ; ";
    cout << "$a3 = " << _registers.bank[7] << endl;

    for( int i = 8; i < 16; i++ )
        cout << "$t" << i-8 << " = " << _registers.bank[i] << ( i==15 ? "\n":" ; " );

    for( int i = 16; i < 24; i++ )
        cout << "$s" << i-16 << " = " << _registers.bank[i] << ( i==23 ? "\n":" ; " );

    cout << "$t8 = " << _registers.bank[24] << " ; ";
    cout << "$t9 = " << _registers.bank[25] << endl;

    cout << "$k0 = " << _registers.bank[26] << " ; ";
    cout << "$k1 = " << _registers.bank[27] << endl;

    cout << "$gp = " << _registers.bank[28] << endl;

    cout << "$sp = " << _registers.bank[29] << endl;

    cout << "$fp = " << _registers.bank[30] << endl;

    cout << "$ra = " << _registers.bank[31] << endl;

    cout << "=============================" << endl;

    cout << endl;
}
void Mips::PrintMem()
{
    cout << "========================== DATA Memory ==========================" << endl;

    for( int i=imem; i < 256; i++ )
    {
        cout << _memory.bank[i] << ( (i+1)%32 == 0 ? "\n":" " );
    }

    cout << "=================================================================" << endl;

    cout << endl;
}