#ifndef EMU_TYPES_HPP
#define EMU_TYPES_HPP

#include<cinttypes>


namespace emu
{

  typedef struct CPUState
  {
    uint8_t *program_memory;
    uint8_t *external_memory;
    registers_t regs;
  } state_t;

  typedef struct Registers
  {
    uint8_t *PSW;
  } registers_t;
}

#endif