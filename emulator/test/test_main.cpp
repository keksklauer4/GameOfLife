#include "emulator_testing.hpp"
#include "test_data.hpp"

#include <gtest/gtest.h>

using namespace emu::testing;

static auto test_data = create_test_conditions();

TEST(T, Example)
{
  run_emulator_breakpoint(test_data.at(0), BREAK_ADDRESS, 1000);
}

TEST(U, Example)
{
  run_emulator_breakpoint(test_data.at(1), BREAK_ADDRESS, 1000);
}

TEST(V, Example)
{
  run_emulator_breakpoint(test_data.at(2), BREAK_ADDRESS, 1000);
}