#ifndef EMU_TYPES_HPP
#define EMU_TYPES_HPP

#include<cinttypes>

// see page 38 of http://web.mit.edu/6.115/www/document/8051.pdf
#define A_OFFSET 0xE0
#define B_OFFSET 0xF0
#define PSW_OFFSET 0xD0
#define SP_OFFSET 0x81
#define DPTR_OFFSET 0x82
#define P0_OFFSET 0x80
#define P1_OFFSET 0x90
#define P2_OFFSET 0xA0
#define P3_OFFSET 0xB0


namespace emu
{
  typedef uint32_t fuint32_t;
  union uint16_union_t
  {
    uint16_t v16;
    uint8_t v8[2];
  };

  typedef struct Registers
  {
    uint16_t PC;
    uint16_t* DPTR;
    uint8_t* A;
    uint8_t* B;
    uint8_t* PSW;
    uint8_t* SP;
    uint8_t* P0;
    uint8_t* P1;
    uint8_t* P2;
    uint8_t* P3;
  } registers_t;

  typedef struct CPUState
  {
    void setRegisters()
    {
      regs.A = program_memory + A_OFFSET;
      regs.B = program_memory + B_OFFSET;
      regs.PSW = program_memory + PSW_OFFSET;
      regs.SP = program_memory + SP_OFFSET;
      regs.P0 = program_memory + P0_OFFSET;
      regs.P1 = program_memory + P1_OFFSET;
      regs.P2 = program_memory + P2_OFFSET;
      regs.P3 = program_memory + P3_OFFSET;
      regs.DPTR = (uint16_t*)(program_memory + DPTR_OFFSET);
    }
    void reset()
    {
      regs.PC = 0;
      *regs.A = 0;
      *regs.B = 0;
      *regs.PSW = 0;
      *regs.DPTR = 0;
      *regs.P0 = 0xFF;
      *regs.P1 = 0xFF;
      *regs.P2 = 0xFF;
      *regs.P3 = 0xFF;
      *regs.SP = 7;
    }


    uint8_t *program_memory;
    uint8_t *external_memory;
    registers_t regs;
  } state_t;

}

#endif