# made-up-arch
simple made-up cpu emulator 

## Example
Following test case from project source shows writing code that calculates popcount to memory and its execution.

```cpp
TEST(simple_sequences_tests, popcount)
{
    const auto val = 0xF000'0F00'00F0'000F;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = 0 }));
    code.push_back(toInstruction(Comparison_imm_instruction{ .opcode = Opcode::cmp, .r1 = 0, .imm = 0 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::equal, .offset = 4 }));
    code.push_back(toInstruction(Shift_imm_instruction{ .type = Shift_type::logical_left, .rdst = 0, .r1 = 0, .imm = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::not_carry, .offset = 1 }));
    code.push_back(toInstruction(Arithmetic_imm_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 1, .imm = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -6 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }

    processor.state.registers[0] = val;


    while (processor.state.registers[Processor_state::program_counter] < code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[1], std::popcount(val));
}
```

## Planned
- assembly
