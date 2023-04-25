# made-up-arch
simple made-up cpu emulator 

## Current capabilities
- 16 64bit registers including program counter, stack pointer and link register
- status flags (carry, overflow, negative, zero)
- instructions (32b each):
    - mov (with imm variant)
    - ldr/str (1/2/4/8B; addressing: register base + imm offset)
    - push/pop (1/2/4/8B)
    - add/sub (with imm variants)
    - and/or/xor (with imm variants)
    - inv
    - shift/rotate (with imm variants)
    - branch (based on flags; pc relative and absolute; with link option) 
    - cmp (with imm variant)
    - tst
- rudimentary interrupts:
    - reset handler
    - invalid opcode
    - invalid address
    - timer0
    - timer1
- memory mapped i/o
- peripherals:
    - two 64bit timers

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

    writeCode(processor.state.memory, 0x0, code);

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
