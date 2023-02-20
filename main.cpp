#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <iostream>
#include <vector>


using namespace emulator;

int main()
{
    const auto begin_addr = 0x100;
    const auto len = 16;
    const auto value = 0xFF;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = begin_addr }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = len }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 2, .imm = value }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 3, .imm = 1 }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Comparison_instruction{ .opcode = Opcode::cmp, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::equal, .offset = 3 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::byte, .rsrc = 2, .rbase = 0, .off = 0 }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 0, .r1 = 0, .r2 = 3 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -5 }));

    Processor processor(512);


    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }

    processor.state.print();
    while(processor.state.registers[Processor_state::program_counter] < code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
        std::cout << "\n";
        processor.state.print();
    }
}