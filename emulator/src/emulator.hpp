#ifndef EMU_EMULATOR_HPP
#define EMU_EMULATOR_HPP

#include <cstring>
#include <fstream>
#include <memory>
#include <functional>
#include <bitset>

#include "config.hpp"
#include "types.hpp"
#include "opcode_handler.hpp"

#define ADDRESSABLE 0xFFFF + 1
#define MEM_SIZE ADDRESSABLE + 0xFF + 1
#define SFR_MEM_SIZE 0x80
#define INTERNAL_MEM_SIZE 0xFF + 1

namespace emu
{
  class OpcodeHandler;
  enum TimerMode {

  };

  class Emulator;
  typedef void (Emulator::*timer_jp_table_entry_t)(uint8_t*, uint8_t);

  class Emulator
  {
    public:
      Emulator(std::function<void(state_t&)> stepCallback);
      ~Emulator();

      void step();
      void setExternalMemory(uint8_t* address);
      void triggerExternalInterrupt0();
      void triggerExternalInterrupt1();
      void setup(const std::string& filename);

    private:
      void init();
      void handleInterrupts();
      void readFile(const std::string& filename);
      inline void handleTimer(uint8_t* address);

      void timer_mode_00(uint8_t *timer, uint8_t mask);
      void timer_mode_01(uint8_t *timer, uint8_t mask);
      void timer_mode_10(uint8_t *timer, uint8_t mask);
      void timer_mode_11(uint8_t *timer, uint8_t mask);

    private:
      std::function<void(state_t&)> m_stepCallback;
      timer_jp_table_entry_t m_jpTable[4];
      std::unique_ptr<OpcodeHandler> m_opHandler;
      state_t m_state;
  };
}



#endif