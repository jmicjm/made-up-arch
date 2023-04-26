#include "emulator/processor.h"
#include "emulator/instructions.h"
#include "emulator/memory_templates.h"
#include <iostream>
#include <vector>


using namespace emulator;

int main()
{
    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = 0xFF }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = 0xFF }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 2, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Push_instruction{ .size = Data_type::octobyte, .rsrc = 2 }));


    const auto memory_size = 512;
    const auto initial_sp = 512;
    auto [memory, start_addr] = basicMemoryImage(memory_size, initial_sp);
    writeCode(memory, start_addr, code);


    Processor processor{ memory };


    processor.state.print();
    while(processor.state.registers[Processor_state::program_counter] < start_addr + code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
        std::cout << "\n";
        processor.state.print();
    }
}