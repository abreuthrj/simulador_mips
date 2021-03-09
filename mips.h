#ifndef MIPS_H
#define MIPS_H

#include <iostream>
#include <fstream>
#include <vector>
#include "components.h"

class Mips
{
    private:
        // int data_memory[256];
        // size_t instruction_memory[256];
        // int registers_bank[32];
        // size_t memory[256];
        // std::vector<int> memory;

        int imem;
        int pc;

        UC _control;
        ALU _alu;
        Memory _memory;
        Registers _registers;
        Instruction _decoded;
        int _cclock;

        Instruction decode(size_t instruction);
        void proccess();
        void etapa01();
        void etapa02();
        void etapa03();
        void etapa04();
        void etapa05();
        
        void init();

        string helper[32] = {
            "$zero", // 0
            "$at", // 1
            "$v0", // 2
            "$v1", // 3
            "$a0", // 4
            "$a1", // 5
            "$a2", // 6
            "$a3", // 7
            "$t0", // 8
            "$t1", // 9
            "$t2", // 10
            "$t3", // 11
            "$t4", // 12
            "$t5", // 13
            "$t6", // 14
            "$t7", // 15
            "$s0", // 16
            "$s1", // 17
            "$s2", // 18
            "$s3", // 19
            "$s4", // 20
            "$s5", // 21
            "$s6", // 22
            "$s7", // 23
            "$t8", // 24
            "$t9", // 25
            "$k0", // 26
            "$k1", // 27
            "$gp", // 28
            "$sp", // 29
            "$fp", // 30
            "$ra" // 31
            };

        // COMPONENTES
        int ALUControl(int funct);

    public:
        Mips();
        ~Mips();

        Mips(std::ifstream& file);

        void Run();

        void PrintReg();
        void PrintMem();

        void Execute( string inst );
        void Step();
};

#endif