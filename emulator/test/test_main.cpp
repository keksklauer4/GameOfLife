#include "emulator_testing.hpp"
#include "test_data.hpp"

#include <gtest/gtest.h>

using namespace emu::testing;

static auto test_data = create_test_conditions();

TEST(ADD, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(0), BREAK_ADDRESS, 1000);
}

TEST(SUBB, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(1), BREAK_ADDRESS, 1000);
}

TEST(PUSH, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(2), BREAK_ADDRESS, 1000);
}

TEST(MUL, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(4), BREAK_ADDRESS, 1000);
}

TEST(DIV, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(3), BREAK_ADDRESS, 1000);
}

TEST(DJNZ, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(5), BREAK_ADDRESS, 300);
}

TEST(STACK, InstructionTest)
{
  run_emulator_breakpoint(test_data.at(6), BREAK_ADDRESS, 300);
}