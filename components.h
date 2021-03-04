#ifndef COMPONENTS_H
#define COMPONENTS_H

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

    void GenerateSignals(int OpCode);
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
    int Result(int data01, int data02);
};


size_t bintodec(string bin);
long int bintosigned(string bin);
string dectobin(size_t dec);
string dectobin(size_t dec, int fillLeft);
string signedtobin(long int dec, int fillLeft);

#endif