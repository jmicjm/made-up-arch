#pragma once
#include "../instruction.h"
#include <vector>
#include <string>
#include <type_traits>


namespace emulator
{
    template<Instruction_t>
    class Register_instruction_describer;

    template<Instruction_t>
    class Register_instruction_describer_generic;

    namespace debugging
    {
        std::string instructionToString(Instruction_t instruction);

        class InstructionDescribers
        {
            static inline std::vector<std::string(*)(Instruction_t)> describers = std::vector<std::string(*)(Instruction_t)>(1 << opcode_size, [](Instruction_t) -> std::string { return "invalid instruction"; });

            template<Instruction_t>
            friend class ::emulator::Register_instruction_describer;
            template<Instruction_t>
            friend class ::emulator::Register_instruction_describer_generic;
            friend std::string instructionToString(Instruction_t instruction);
        };


        template<typename T>
        concept Imm_opt_instruction = requires(T t)
        {
            t.imm_arg;
        };

        template<typename T>
        concept Rdst_rsrc_instruction = requires(T t)
        {
            t.rdst;
            t.rsrc;
        };

        template<typename T>
        concept Rdst_imm_instruction = requires(T t)
        {
            t.rdst;
            t.imm;
        };

        template<typename T>
        concept R1_r2_instruction = requires(T t)
        {
            t.r1;
            t.r2;
        };

        template<typename T>
        concept R1_imm_instruction = requires(T t)
        {
            t.r1;
            t.imm;
        };

        template<typename T>
        concept Data_transfer_instruction = requires(T t)
        {
            t.size;
            t.rbase;
            t.off;
        };

        template<typename T>
        concept Stack_transfer_instruction = requires(T t)
        {
            t.size;
            requires !Data_transfer_instruction<T>;
        };


        template<Rdst_rsrc_instruction T, Rdst_imm_instruction Y>
        requires Imm_opt_instruction<T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            std::string desc = std::string{ name } + " r" + std::to_string(i.rdst) + " ";
            if (!i.imm_arg) desc += "r" + std::to_string(i.rsrc);
            else desc += std::to_string(instruction_cast<Y>(i).imm);
            return desc;
        }

        template<Rdst_rsrc_instruction T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            return std::string{ name } + " r" + std::to_string(i.rdst) + " r" + std::to_string(i.rsrc);
        }

        template<R1_r2_instruction T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            auto rdst = [&]
            {
                if constexpr (requires(T t) { t.rdst; }) return " r" + std::to_string(i.rdst);
                else return "";
            };
            return std::string{ name } + rdst() + " r" + std::to_string(i.r1) + " r" + std::to_string(i.r2);
        }

        template<R1_r2_instruction T, R1_imm_instruction Y>
        requires Imm_opt_instruction<T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            auto rdst = [&]
            {
                if constexpr (requires(T t) { t.rdst; }) return " r" + std::to_string(i.rdst);
                else return "";
            };
            std::string desc = std::string{ name } + rdst() + " r" + std::to_string(i.r1) + " ";
            if (!i.imm_arg) desc += "r" + std::to_string(i.r2);
            else desc += std::to_string(instruction_cast<Y>(i).imm);

            return desc;
        }

        template<Data_transfer_instruction T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            auto sd = [&]
            {
                if constexpr (requires(T t) { t.rsrc; }) return i.rsrc;
                else return i.rdst;
            };
            return std::string{ name } + std::to_string(1 << i.size) + " [r" + std::to_string(i.rbase) + " + " + std::to_string(i.off) + "] r" + std::to_string(sd());
        }

        template<Stack_transfer_instruction T>
        std::string describeInstructionImpl(T i, std::string_view name)
        {
            auto sd = [&]
            {
                if constexpr (requires(T t) { t.rsrc; }) return i.rsrc;
                else return i.rdst;
            };
            return std::string{ name } + std::to_string(1 << i.size) + " r" + std::to_string(sd());
        }

        template<typename main_instr_t, typename opt_instr_t = void>
        std::string describeInstruction(Instruction_t instruction, std::string_view name)
        {
            if constexpr (std::is_same_v<opt_instr_t, void>) return describeInstructionImpl<main_instr_t>(instruction_cast<main_instr_t>(instruction), name);
            else return describeInstructionImpl<main_instr_t, opt_instr_t>(instruction_cast<main_instr_t>(instruction), name);
        }
    }
}

#define REGISTER_INSTRUCTION_DESCRIBER(opcode, to_string)                                   \
template<>                                                                                  \
class Register_instruction_describer<opcode>                                                \
{                                                                                           \
    struct Register                                                                         \
    {                                                                                       \
        Register()                                                                          \
        {                                                                                   \
            debugging::InstructionDescribers::describers[opcode] = to_string;               \
        }                                                                                   \
    };                                                                                      \
    inline static auto dummy = Register{};                                                  \
};

#define REGISTER_INSTRUCTION_DESCRIBER_GENERIC(opcode, name, type, type_opt)                \
template<>                                                                                  \
class Register_instruction_describer<opcode>                                                \
{                                                                                           \
    struct Register                                                                         \
    {                                                                                       \
        Register()                                                                          \
        {                                                                                   \
            debugging::InstructionDescribers::describers[opcode] = [](Instruction_t i) {    \
                return debugging::describeInstruction<type, type_opt>(i, name);             \
            };                                                                              \
        }                                                                                   \
    };                                                                                      \
    inline static auto dummy = Register{};                                                  \
};
