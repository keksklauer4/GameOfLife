#ifndef EMU_EMULATOR_HPP
#define EMU_EMULATOR_HPP

#include <cstring>
#include <fstream>
#include <memory>

#include "types.hpp"
#include "opcode_handler.hpp"

#define ADDRESSABLE 0xFFFF + 1
#define MEM_SIZE ADDRESSABLE + 0xFF + 1
#define SFR_MEM_SIZE 0x80
#define INTERNAL_MEM_SIZE 0xFF + 1

namespace emu
{
  class OpcodeHandler;

  class Emulator
  {
    public:
      Emulator();
      ~Emulator();

      void step();
      void setExternalMemory(uint8_t* address);
      void triggerExternalInterrupt();
      void setup(const std::string& filename);

    private:
      void init();
      void handleInterrupts();
      void readFile(const std::string& filename);


    private:
      std::unique_ptr<OpcodeHandler> m_opHandler;
      state_t m_state;
  };
}



#endif