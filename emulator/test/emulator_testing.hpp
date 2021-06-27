#ifndef EMU_EMULATOR_TESTING_HPP
#define EMU_EMULATOR_TESTING_HPP

#include "test_conditions.hpp"
#include "../src/emulator.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <memory>

namespace emu
{
namespace testing
{
  
  typedef struct TestSetting
  {
    TestSetting(std::string f): filename(f) {}
    std::string filename;
    std::vector<std::unique_ptr<TestCondition>> m_conditions;
  } test_config_t;

  // break after amountCycles (rounded up to next instruction)
  void run_emulator_cycles(test_config_t& conf, fuint32_t amountCycles);

  // break at specific address or after at most maxAmountCycles (to prevent infinite runs)
  void run_emulator_breakpoint(test_config_t& conf, uint16_t breakAddress, fuint32_t maxAmountCycles);

}
}

#endif


