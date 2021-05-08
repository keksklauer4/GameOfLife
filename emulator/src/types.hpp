#ifndef EMU_TYPES_HPP
#define EMU_TYPES_HPP

#include<cinttypes>

// see page 38 of http://web.mit.edu/6.115/www/document/8051.pdf
#define SFR_MEM_OFFSET 0x80
#define A_OFFSET 0xE0 - SFR_MEM_OFFSET
#define B_OFFSET 0xF0 - SFR_MEM_OFFSET
#define PSW_OFFSET 0xD0 - SFR_MEM_OFFSET
#define SP_OFFSET 0x81 - SFR_MEM_OFFSET
#define DPTR_OFFSET 0x82 - SFR_MEM_OFFSET
#define P0_OFFSET 0x80 - SFR_MEM_OFFSET
#define P1_OFFSET 0x90 - SFR_MEM_OFFSET
#define P2_OFFSET 0xA0 - SFR_MEM_OFFSET
#define P3_OFFSET 0xB0 - SFR_MEM_OFFSET
#define IP_OFFSET 0xB8 - SFR_MEM_OFFSET
#define IE_OFFSET 0xA8 - SFR_MEM_OFFSET
#define TMOD_OFFSET 0x89 - SFR_MEM_OFFSET
#define TCON_OFFSET 0x88 - SFR_MEM_OFFSET
#define TH0_OFFSET 0x8C - SFR_MEM_OFFSET
#define TL0_OFFSET 0x8A - SFR_MEM_OFFSET
#define TH1_OFFSET 0x8D - SFR_MEM_OFFSET
#define TL1_OFFSET 0x8B - SFR_MEM_OFFSET
#define SCON_OFFSET 0x98 - SFR_MEM_OFFSET
#define SBUF_OFFSET 0x99 - SFR_MEM_OFFSET
#define PCON_OFFSET 0x87 - SFR_MEM_OFFSET

#define CAST(v) static_cast<unsigned int>(v)

namespace emu
{
  typedef uint32_t fuint32_t;
  typedef union uint16_union_t
  {
    uint16_t v16;
    uint8_t v8[2];
  } uuint16_t;

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

    uint8_t* IP;
    uint8_t* IE;
    uint8_t* TMOD;
    uint8_t* TCON;
    uint8_t* TH0;
    uint8_t* TH1;
    uint8_t* TL0;
    uint8_t* TL1;
    uint8_t* SCON;
    uint8_t* SBUF;
    uint8_t* PCON;
  } registers_t;

  typedef struct CPUState
  {
    void setRegisters()
    {
      regs.A = sfr_memory + A_OFFSET;
      regs.B = sfr_memory + B_OFFSET;
      regs.PSW = sfr_memory + PSW_OFFSET;
      regs.SP = sfr_memory + SP_OFFSET;
      regs.P0 = sfr_memory + P0_OFFSET;
      regs.P1 = sfr_memory + P1_OFFSET;
      regs.P2 = sfr_memory + P2_OFFSET;
      regs.P3 = sfr_memory + P3_OFFSET;
      regs.DPTR = (uint16_t*)(sfr_memory + DPTR_OFFSET);
      regs.IP = sfr_memory + IP_OFFSET;
      regs.IE = sfr_memory + IE_OFFSET;
      regs.TMOD = sfr_memory + TMOD_OFFSET;
      regs.TCON = sfr_memory + TCON_OFFSET;
      regs.TH0 = sfr_memory + TH0_OFFSET;
      regs.TH1 = sfr_memory + TH1_OFFSET;
      regs.TL0 = sfr_memory + TL0_OFFSET;
      regs.TL1 = sfr_memory + TL1_OFFSET;
      regs.SCON = sfr_memory + SCON_OFFSET;
      regs.SBUF = sfr_memory + SBUF_OFFSET;
      regs.PCON = sfr_memory + PCON_OFFSET;
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
      *regs.IP = 0x00;
      *regs.IE = 0x00;
      *regs.TMOD = 0x00;
      *regs.TCON = 0x00;
      *regs.TH0 = 0x00;
      *regs.TH1 = 0x00;
      *regs.TL0 = 0x00;
      *regs.TL1 = 0x00;
      *regs.SCON = 0x00;
      *regs.SBUF = 0x00; // should be indeterminate but who cares
      *regs.PCON = 0x00;
    }


    uint8_t *program_memory;
    uint8_t *external_memory;
    uint8_t *internal_data;
    uint8_t *sfr_memory;
    registers_t regs;
    uint8_t last_opcode;
  } state_t;
}

#endif