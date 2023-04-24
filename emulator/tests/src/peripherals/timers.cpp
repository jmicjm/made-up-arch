#include <gtest/gtest.h>
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include "emulator/memory_templates.h"
#include "emulator/memory_helpers.h"
#include "emulator/common.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

static std::pair<std::vector<uint8_t>, uint64_t> prepareTimerCpuMemory(uint64_t timer_value, uint64_t timer_reset_value, Timer_state::Config_word timer_config, uint64_t timer_address)
{
    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 0, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) * 7 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 1, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) * 8 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 2, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) * 9 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 3, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) * 10 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = 1, .rbase = 0, .off = 0 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = 2, .rbase = 0, .off = 8 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = 3, .rbase = 0, .off = 16 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -1 }));
    code.push_back(lowerHalf(timer_address));
    code.push_back(upperHalf(timer_address));
    code.push_back(lowerHalf(timer_value));
    code.push_back(upperHalf(timer_value));
    code.push_back(lowerHalf(timer_reset_value));
    code.push_back(upperHalf(timer_reset_value));
    code.push_back(lowerHalf(reinterpret_cast<Aliasable<uint32_t>&>(timer_config)));
    code.push_back(upperHalf(reinterpret_cast<Aliasable<uint32_t>&>(timer_config)));


    const auto initial_sp = 512;
    auto [memory, start_addr] = basicMemoryImage(512, initial_sp);
    writeCode(memory, start_addr, code);


    return { std::move(memory), start_addr + 6 * sizeof(Instruction_t) };
}

static void testTimerBackward(auto&& timer, uint64_t timer_address, bool auto_reload)
{
    const auto timer_value = 255;
    const auto timer_reset_value = timer_value;
    Timer_state::Config_word timer_config;
    timer_config.enable = true;
    timer_config.auto_reload = auto_reload;
    timer_config.mode = Timer_state::Mode::backward;

    auto [memory, counter_start_address] = prepareTimerCpuMemory(timer_value, timer_reset_value, timer_config, timer_address);

    Processor processor{ memory };

    auto cycles_to_overflow = timer_value;
    while (cycles_to_overflow > 0)
    {
        processor.executeNext();

        if (processor.state.registers[Processor_state::program_counter] > counter_start_address) cycles_to_overflow--;
    }

    EXPECT_EQ(timer(processor.state).state.config_word.overflow, false);
    EXPECT_EQ(timer(processor.state).state.value, 0);

    processor.executeNext();

    EXPECT_EQ(timer(processor.state).state.config_word.overflow, true);
    EXPECT_EQ(timer(processor.state).state.value, timer_reset_value);
    EXPECT_EQ(timer(processor.state).state.config_word.enable, auto_reload);
}

static void testTimerForward(auto&& timer, uint64_t timer_address, bool auto_reload)
{
    const auto timer_value = 0;
    const auto timer_reset_value = 255;
    Timer_state::Config_word timer_config;
    timer_config.enable = true;
    timer_config.auto_reload = auto_reload;
    timer_config.mode = Timer_state::Mode::forward;

    auto [memory, counter_start_address] = prepareTimerCpuMemory(timer_value, timer_reset_value, timer_config, timer_address);

    Processor processor{ memory };


    auto cycles_to_overflow = timer_reset_value - timer_value;
    while (cycles_to_overflow > 0)
    {
        processor.executeNext();

        if (processor.state.registers[Processor_state::program_counter] > counter_start_address) cycles_to_overflow--;
    }

    EXPECT_EQ(timer(processor.state).state.config_word.overflow, false);
    EXPECT_EQ(timer(processor.state).state.value, timer_reset_value);

    processor.executeNext();

    EXPECT_EQ(timer(processor.state).state.config_word.overflow, true);
    EXPECT_EQ(timer(processor.state).state.value, 0);
    EXPECT_EQ(timer(processor.state).state.config_word.enable, auto_reload);
}


TEST(timers, timer0_count_backward_no_auto_reload)
{
    testTimerBackward(
        [](Processor_state& ps) { return ps.peripherals.timer0; },
        timer0_address_range.begin,
        false
    );
}

TEST(timers, timer1_count_backward_no_auto_reload)
{
    testTimerBackward(
        [](Processor_state& ps) { return ps.peripherals.timer1; },
        timer1_address_range.begin,
        false
    );
}

TEST(timers, timer0_count_backward_auto_reload)
{
    testTimerBackward(
        [](Processor_state& ps) { return ps.peripherals.timer0; },
        timer0_address_range.begin,
        true
    );
}

TEST(timers, timer1_count_backward_auto_reload)
{
    testTimerBackward(
        [](Processor_state& ps) { return ps.peripherals.timer1; },
        timer1_address_range.begin,
        true
    );
}

TEST(timers, timer0_count_forward_no_auto_reload)
{
    testTimerForward(
        [](Processor_state& ps) { return ps.peripherals.timer0; },
        timer0_address_range.begin,
        false
    );
}

TEST(timers, timer1_count_forward_no_auto_reload)
{
    testTimerForward(
        [](Processor_state& ps) { return ps.peripherals.timer1; },
        timer1_address_range.begin,
        false
    );
}

TEST(timers, timer0_count_forward_auto_reload)
{
    testTimerForward(
        [](Processor_state& ps) { return ps.peripherals.timer0; },
        timer0_address_range.begin,
        true
    );
}

TEST(timers, timer1_count_forward_auto_reload)
{
    testTimerForward(
        [](Processor_state& ps) { return ps.peripherals.timer1; },
        timer1_address_range.begin,
        true
    );
}