#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <iostream>

using namespace std;

struct UC
{
    int PCWriteCond = 0;
    int PCWrite = 0;
    int IorD = 0;
    int MemRead = 0;
    int MemWrite = 0;
    int MemtoReg = 0;
    int IRWrite = 1;
    int PCSource = 0;
    int ALUOp = 0;
    int ALUSrcB = 0;
    int ALUSrcA = 0;
    int RegWrite = 0;
    int RegDst = 0;
    // int Branch;
    // int Jump;

    void GenerateSignals(int OpCode);
    void Print();
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
    long int data1;
    long int data2;
    
    long int tick();
};

struct Memory
{
    size_t address;
    size_t write_data;
    size_t mem_data;

    Instruction instruction;

    long int bank[256];
};

struct Registers
{
    // Instruction IR;
    long int IR,MDR,A,B,ALUOut;
    long int bank[32];

    short int zero=0;short int at=1;short int v0=2;short int v1=3;short int a0=4;short int a1=5;short int a2=6;short int a3=7;short int t0=8;short int t1=9;short int t2=10;short int t3=11;short int t4=12;short int t5=13;short int t6=14;short int t7=15;short int s0=16;short int s1=17;short int s2=18;short int s3=19;short int s4=20;short int s5=21;short int s6=22;short int s7=23;short int t8=24;short int t9=25;short int k0=26;short int k1=27;short int gp=28;short int sp=29;short int fp=30;short int ra=31;
    
    string helper[32] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};
};



size_t bintodec(string bin);
long int bintosigned(string bin);
string dectobin(size_t dec);
string dectobin(size_t dec, int fillLeft);
string signedtobin(long int dec, int fillLeft);

#endif