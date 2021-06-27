#include "emulator_testing.hpp"
#include "test_conditions.hpp"

using namespace emu;
using namespace emu::testing;

void emu::testing::run_emulator_cycles(test_config_t &conf, fuint32_t amountCycles)
{
  bool run = true;
  test_config_t *config = &conf;
  auto lambda =
      std::function<void(state_t &)>([&run, config, amountCycles](state_t &s)
      {
        if (s.cycles_passed >= amountCycles)
        {
          run = false;
          for (auto &condition : config->m_conditions)
          {
            condition->execute(s);
          }
        }
      });

  Emulator emu(lambda);
  emu.setup(conf.filename);

  while (run)
  {
    emu.step();
  }
}

// break at specific address or after at most maxAmountCycles (to prevent
// infinite runs)
void emu::testing::run_emulator_breakpoint(test_config_t &conf, uint16_t breakAddress,
                             fuint32_t maxAmountCycles)
{
  bool run = true;
  test_config_t *config = &conf;
  auto lambda = std::function<void(state_t &)>(
      [&run, config, maxAmountCycles, &breakAddress](state_t &s) 
      {
        if (s.cycles_passed >= maxAmountCycles || s.regs.PC == breakAddress) 
        {
          run = false;
          for (auto &condition : config->m_conditions)
          {
            condition->execute(s);
          }
        }
      });

  Emulator emu(lambda);
  emu.setup(conf.filename);

  while (run)
  {
    emu.step();
  }
}