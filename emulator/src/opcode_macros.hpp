#ifndef EMU_OPCODE_MACROS_HPP
#define EMU_OPCODE_MACROS_HPP

#include "opcode_parse_helpers.hpp"
#include "intrinsics.hpp"



#define READ_2BYTE_PC_BIG_ENDIAN()                           \
  BSWAP16(*((uint16_t*)(m_state.program_memory + PC_REG)));  \
  PC_REG+=2

#define POP_FROM_STACK()            \
  m_state.internal_data[DEC_SP()]

#define WRITE_STACK(val)                 \
  INC_SP();                              \
  m_state.internal_data[GET_SP()] = (val);

#define WRITE_STACK_2B(addr)  \
  WRITE_STACK(addr & 0xFF)    \
  WRITE_STACK(addr >> 8)

#define JMP_FROM_STACK()          \
  PC_REG = POP_FROM_STACK();      \
  PC_REG <<= 8;                   \
  PC_REG |= POP_FROM_STACK()


#define JUMP_REL(offset) PC_REG += (int8_t)offset

#define JUMP_LOC(addr) PC_REG = addr
#define AJMP_LOC(addr) PC_REG &= 0xF800; PC_REG |= addr

#define CALL_LOC(addr)              \
  WRITE_STACK_2B(m_state.regs.PC);  \
  JUMP_LOC(addr);

#define ACALL_LOC(addr)              \
  WRITE_STACK_2B(m_state.regs.PC);  \
  AJMP_LOC(addr);

#define RESET_CARRY() *m_state.regs.PSW &= 0x7F
#define SET_CARRY()   *m_state.regs.PSW |= 0x80
#define CPL_CARRY()   *m_state.regs.PSW ^= 0x80

#define RESET_AUX_CARRY() *m_state.regs.PSW &= 0b10111111
#define SET_AUX_CARRY()   *m_state.regs.PSW |= 0b01000000

#define RESET_OV() *m_state.regs.PSW &= 0b11111011
#define SET_OV()   *m_state.regs.PSW |= 0b00000100

#define STOP_IDLE_MODE() *m_state.regs.PCON &= 0b11111110

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
  (m_state.sfr_memory + (val & 0x78))

#define BIT_ADDRESSABLE_BITMASK(val) (0x01 << (val & 0x07))

#define CHECK_BIT_ADDRESSABLE_SET(val)    \
  ((*FETCH_BIT_ADRESSABLE(val)) & BIT_ADDRESSABLE_BITMASK(val))

#define CLR_BIT_ADDRESSABLE_RAW(val) \
  (*FETCH_BIT_ADRESSABLE(val)) &= (~BIT_ADDRESSABLE_BITMASK(val))

#define CLR_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  CLR_BIT_ADDRESSABLE_RAW(byte)

#define SET_BIT_ADDRESSABLE_RAW(val)                              \
  (*FETCH_BIT_ADRESSABLE(val)) |= (BIT_ADDRESSABLE_BITMASK(val))

#define SET_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  SET_BIT_ADDRESSABLE_RAW(byte)

#define CONDITIONAL_BIT_ADRESSABLE_MOV(cond)  \
  if(cond) { SET_BIT_ADDRESSABLE();  } else { CLR_BIT_ADDRESSABLE(); }

#define CPL_BIT_ADDRESSABLE()             \
  const uint8_t byte = READ_BYTE_PC();    \
  (*FETCH_BIT_ADRESSABLE(byte)) ^= (BIT_ADDRESSABLE_BITMASK(byte))


#endif