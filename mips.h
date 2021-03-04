#ifndef MIPS_H
#define MIPS_H

#include <iostream>
#include <fstream>
#include <vector>
#include "components.h"

class Mips
{
    private:
        int data_memory[256];
        size_t instruction_memory[256];
        int registers_bank[32];
        // std::vector<int> memory;

        int imem;
        int pc;

        UC control;
        ALU alu;

        Instruction decode(size_t instruction);
        void step( size_t instruction );
        
        void init();

        string helper[32] = {
            "$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"
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
};

#endif