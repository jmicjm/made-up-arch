#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <iostream>
#include <vector>

using namespace emulator;

int main()
{
    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Mov_instruction{ .rdst = 7, .rsrc = 8 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 6, .imm = 0x7CBA987654321 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::byte, .rdst = 10, .rbase = 15, .off = 8 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 11, .rbase = 15, .off = 464 }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .r1 = 11, .r2 = 6 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 13, .imm = 504 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = 11, .rbase = 13, .off = 0 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = 0xF0F0F0 }));
    code.push_back(toInstruction(Logical_inv_instruction{ .r = 0 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = 0xF0F0F0 }));
    code.push_back(toInstruction(Logical_instruction{ .opcode = Opcode::orl, .r1 = 0, .r2 = 1 }));
    code.push_back(uint64_t{0xDDDDDDDDCCCCBBAA});


    Processor processor(512);

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i*8]) = code[i];
    }

    reinterpret_cast<Instruction_t&>(processor.state.memory[504]) = uint64_t{ 0xDDDDDDDDCCCCBBAA };

    processor.state.registers[0] = 254;
    processor.state.registers[1] = 2;
    processor.state.registers[8] = 0xAA;


    processor.state.print();
    for (auto i = 0; i < code.size(); i++)
    {
        processor.executeNext();
        std::cout << "\n";
        processor.state.print();
    }
}