#pragma once
#include "memory_templates.h"


namespace emulator
{
    std::pair<std::vector<uint8_t>, uint64_t> basicMemoryImage(size_t size, uint64_t initial_sp)
    {
        std::vector<uint8_t> memory(size);

        const auto reset_handler = basicResetHandler(initial_sp);
        const auto failure_handler = nopHandler();

        Interrupt_vector& int_vec = reinterpret_cast<Interrupt_vector&>(memory[0]);
        int_vec.reset = sizeof(Interrupt_vector) + 4 * failure_handler.size() * sizeof(Instruction_t);
        int_vec.invalid_opcode = sizeof(Interrupt_vector);
        int_vec.invalid_address = sizeof(Interrupt_vector) + failure_handler.size() * sizeof(Instruction_t);
        int_vec.timer0 = sizeof(Interrupt_vector) + 2 * failure_handler.size() * sizeof(Instruction_t);
        int_vec.timer1 = sizeof(Interrupt_vector) + 3 * failure_handler.size() * sizeof(Instruction_t);


        writeCode(memory, int_vec.invalid_opcode, failure_handler);
        writeCode(memory, int_vec.invalid_address, failure_handler);
        writeCode(memory, int_vec.timer0, failure_handler);
        writeCode(memory, int_vec.timer1, failure_handler);
        writeCode(memory, int_vec.reset, reset_handler);


        const auto start_address = int_vec.reset + reset_handler.size() * sizeof(Instruction_t);

        return { std::move(memory), start_address };
    }

    std::vector<Instruction_t> basicResetHandler(uint64_t initial_sp)
    {
        std::vector<Instruction_t> reset_handler;
        reset_handler.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = Processor_state::stack_pointer, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) }));
        reset_handler.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = 2 }));
        reset_handler.push_back(lowerHalf(initial_sp));
        reset_handler.push_back(upperHalf(initial_sp));

        return reset_handler;
    }

    std::vector<Instruction_t> nopHandler()
    {
        std::vector<Instruction_t> handler;
        handler.push_back(toInstruction(Branch_absolute_instruction{ .link = 0, .condition = Branch_condition::always, .dst = Processor_state::link_register }));

        return handler;
    }
}