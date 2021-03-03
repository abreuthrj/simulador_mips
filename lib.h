#include <iostream>
#include <fstream>
#include <vector>
#include "components.h"

class Mips
{
    private:
        int memory[32];
        // std::vector<int> memory;
        int registers_bank[32];

        int imem;
        int pc;
        UC control;

        Instruction decode(int instruction);

        // void add();
        // void sub();
        // void and();
        // void or();
        // void slt();
        // void sll();
        // void addi();
        // void lw();
        // void sw();
        // void beq();
        // void bne();
        // void j();
        // void jr();
        // void jal();

        // ESSENCIAIS
        int bintodec(std::string bin);
        int bintosigned(std::string bin);
        std::string dectobin(int dec);
        std::string dectobin(int dec, int fillLeft);
        std::string signedtobin(int dec, int fillLeft);

        // COMPONENTES
        int ALUControl(int funct);

    public:
        Mips();
        ~Mips();

        Mips(std::ifstream& file);

        void Run();

        void print();
};