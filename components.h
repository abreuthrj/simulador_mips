#include <iostream>

using namespace std;

struct UC
{
    int PCWriteCond;
    int PCWrite;
    int IorD;
    int MemRead;
    int MemWrite;
    int MemtoReg;
    int IRWrite;
    int PCSource;
    int ALUOp;
    int ALUSrcB;
    int ALUSrcA;
    int RegWrite;
    int RegDst;
    int ALUSrc;
    int Branch;

    void GenerateSignals(int OpCode)
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
    };
};

struct Instruction
{
    char type;
    int opcode;
    int rs;
    int rt;
    int rd;
    int shamt;
    int funct;
    int immediate;
    int address;
};

struct ALU
{
    int ALUControl;

    int Result(int data01, int data02)
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

            default: break;
        }
        cout << "NONE" << endl;
        return 0;
    }
};