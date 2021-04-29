#ifndef EMU_EMULATOR_HPP
#define EMU_EMULATOR_HPP

#include <cstring>
#include <fstream>
#include <memory>

#include "types.hpp"
#include "opcode_handler.hpp"

#define MEM_SIZE 0xFFFF + 1

namespace emu
{
  class OpcodeHandler;

  class Emulator
  {
    public:
      Emulator(const std::string& filename);
      ~Emulator();

      void step();
      void setExternalMemory(uint8_t* address);
      void triggerExternalInterrupt();


    private:
      void init();
      void handleInterrupts();
      void readFile();


    private:
      std::unique_ptr<OpcodeHandler> m_opHandler;
      state_t m_state;

  };
}



#endif