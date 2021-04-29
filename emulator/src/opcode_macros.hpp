#ifndef EMU_OPCODE_MACROS_HPP
#define EMU_OPCODE_MACROS_HPP

#include "opcode_parse_helpers.hpp"


#define BSWAP16(v) __builtin_bswap16(v)

#define READ_2BYTE_PC_BIG_ENDIAN()                                \
  BSWAP16(((uint16_t*)m_state.program_memory)[m_state.regs.PC]);  \
  m_state.regs.PC+=2

#define POP_FROM_STACK()                                    \
  m_state.program_memory[GET_SP()--]

#define WRITE_STACK(val)                                    \
  m_state.program_memory[GET_SP()] = val;                   \
  INC_SP();

#define WRITE_STACK_2B(addr)                                      \
  *((uint16_t*)(m_state.program_memory + GET_SP())) = (uint16_t)addr; \
  GET_SP() += 2

#define JMP_FROM_STACK()          \
  --(GET_SP());                   \
  uint16_t newPC = SP_LOCATION(); \
  --(GET_SP());                   \
  newPC <<= 8;                    \
  newPC |= SP_LOCATION();         \
  m_state.regs.PC = newPC


#define JUMP_REL(offset) m_state.regs.PC += (int8_t)offset

#define JUMP_LOC(addr) m_state.regs.PC = addr
#define CALL_LOC(addr)              \
  WRITE_STACK_2B(m_state.regs.PC);  \
  JUMP_LOC(addr);

#define RESET_CARRY() *m_state.regs.PSW &= 0x7F
#define SET_CARRY()   *m_state.regs.PSW |= 0x80
#define CPL_CARRY()   *m_state.regs.PSW ^= 0x80

#define RESET_AUX_CARRY() *m_state.regs.PSW &= 0x10111111
#define SET_AUX_CARRY()   *m_state.regs.PSW |= 0x01000000

#define RESET_OV() *m_state.regs.PSW &= 0b11111011
#define SET_OV()   *m_state.regs.PSW |= 0b00000100

#define GET_CARRY_NO_SHIFT() (*m_state.regs.PSW & 0x80)
#define GET_CARRY() (*m_state.regs.PSW) >> 7

#define CARRY_CONDITIONAL(cond)                         \
  if (cond) { SET_CARRY(); } else { RESET_CARRY(); }

#define AUX_CARRY_CONDITIONAL(cond)                         \
  if (cond) { SET_AUX_CARRY(); } else { RESET_AUX_CARRY(); }

#define OV_CONDITIONAL(cond)                         \
  if (cond) { SET_OV(); } else { RESET_OV(); }

#define CONDITIONAL_REL_JUMP(cond)          \
  const int8_t offset = READ_BYTE_PC();     \
  if (cond) { JUMP_REL(offset); }

#define FETCH_BIT_ADRESSABLE(val)         \
  (m_state.program_memory + (val & 0xF8))

#define BIT_ADDRESSABLE_BITMASK(val) (0x01 << (val & 0x07))

#define CHECK_BIT_ADDRESSABLE_SET(val)    \
  ((*FETCH_BIT_ADRESSABLE(val)) & BIT_ADDRESSABLE_BITMASK(val))

#define CLR_BIT_ADDRESSABLE_RAW(val) \
  (*FETCH_BIT_ADRESSABLE(val)) &= (~BIT_ADDRESSABLE_BITMASK(val))

#define CLR_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  CLR_BIT_ADDRESSABLE_RAW(byte)

#define SET_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  CLR_BIT_ADDRESSABLE_RAW(byte)

#define CONDITIONAL_BIT_ADRESSABLE_MOV(cond)  \
  if(cond) { SET_BIT_ADDRESSABLE();  } else { CLR_BIT_ADDRESSABLE(); }

#define CPL_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  (*FETCH_BIT_ADRESSABLE(byte)) ^= (BIT_ADDRESSABLE_BITMASK(byte))


#endif