#include "opcode_handler.hpp"
#include "opcode_macros.hpp"

using namespace emu;

void OpcodeHandler::NOP(){}

void OpcodeHandler::ACALL(uint16_t upper_bits)
{
  DBG_CODE_ADDR_ABS(upper_bits)
  upper_bits |= READ_BYTE_PC();
  ACALL_LOC(upper_bits);
}

#define MASKED_ADDITION(mask) (a_reg & mask) + (value & mask)
void OpcodeHandler::ADD(uint8_t value)
{
  const fuint32_t a_reg = *A_REG;

  // handle aux carry flag
  fuint32_t res = MASKED_ADDITION(0x0F);
  AUX_CARRY_CONDITIONAL((res & 0xF0) != 0)

  // handle carry flag
  res = a_reg + value;
  bool carry = (res & 0xFF00) != 0;
  CARRY_CONDITIONAL(carry)
  *A_REG = res & 0xFF;

  // handle overflow flag
  res = MASKED_ADDITION(0x7F);
  carry ^= ((res & 0x80) != 0);
  OV_CONDITIONAL(carry)
}

void OpcodeHandler::ADDC(uint8_t value)
{
  if (GET_CARRY_NO_SHIFT() == 0) { return ADD(value); }
  const fuint32_t a_reg = *A_REG;

  // handle aux carry flag
  fuint32_t res = MASKED_ADDITION(0x0F) + 1;
  AUX_CARRY_CONDITIONAL((res & 0xF0) != 0)

  // handle carry flag
  res = a_reg + value + 1;
  bool carry = (res & 0xFF00) != 0;
  CARRY_CONDITIONAL(carry)
  *A_REG = res & 0xFF;

  // handle overflow flag
  res = MASKED_ADDITION(0x7F) + 1;
  carry ^= ((res & 0x80) != 0);
  OV_CONDITIONAL(carry)
}

void OpcodeHandler::AJMP(uint16_t upper_bits)
{
  upper_bits |= READ_BYTE_PC();
  AJMP_LOC(upper_bits);
}

void OpcodeHandler::ANL_BYTE(uint8_t* dest, uint8_t value)
{
  *dest &= value;
}
void OpcodeHandler::ANL_C(bool cpl)
{
  fuint32_t bit = READ_BYTE_PC();
  bit = CHECK_BIT_ADDRESSABLE_SET(bit);
  CARRY_CONDITIONAL(GET_CARRY_NO_SHIFT() & (cpl ^ (bit != 0)))
}

void OpcodeHandler::CJNE(uint8_t value, uint8_t comp)
{
  int8_t offset = (int8_t)READ_BYTE_PC();
  if (value != comp) { JUMP_REL(offset); }
  CARRY_CONDITIONAL(value < comp)
}

void OpcodeHandler::CLR_A()
{
  *A_REG = 0;
}

void OpcodeHandler::CLR_bit()
{
  DBG_P_BA()
  CLR_BIT_ADDRESSABLE();
}

void OpcodeHandler::CLR_C()
{
  RESET_CARRY();
}

void OpcodeHandler::CPL_A()
{
  *A_REG ^= 0xFF;
}
void OpcodeHandler::CPL_bit()
{
  DBG_P_BA()
  CPL_BIT_ADDRESSABLE();
}

void OpcodeHandler::CPL_C()
{
  CPL_C();
}

void OpcodeHandler::DA()
{
  // TODO
}

void OpcodeHandler::DEC(uint8_t* address)
{
  (*address)--;
}

void OpcodeHandler::DIV()
{
  RESET_CARRY();
  if (*B_REG == 0)
  {
    SET_OV();
  }
  else
  {
    RESET_OV();
    uint8_t div_res = *A_REG / *B_REG;
    *B_REG = *A_REG % *B_REG;
    *A_REG = div_res;
  }
}

void OpcodeHandler::DJNZ(uint8_t* address)
{
  (*address)--;
  CONDITIONAL_REL_JUMP((*address) != 0)
}

void OpcodeHandler::INC(uint8_t* address)
{
  (*address)++;
}

void OpcodeHandler::INC_DPTR()
{
  (*DPTR_REG)++;
}

void OpcodeHandler::JB()
{
  DBG_P_BA_REL()
  const fuint32_t bit = READ_BYTE_PC();
  CONDITIONAL_REL_JUMP(CHECK_BIT_ADDRESSABLE_SET(bit) != 0)
}
void OpcodeHandler::JBC()
{
  DBG_P_BA_REL()
  const fuint32_t bit = READ_BYTE_PC();
  const int8_t offset = (int8_t)READ_BYTE_PC();
  if(CHECK_BIT_ADDRESSABLE_SET(bit) != 0)
  { // cannot use CONDITIONAL_REL_JUMP() because of conditional CLR...
    CLR_BIT_ADDRESSABLE_RAW(bit);
    JUMP_REL(offset);
  }
}

void OpcodeHandler::JC()
{
  DBG_P_BA_REL()
  CONDITIONAL_REL_JUMP(GET_CARRY_NO_SHIFT() != 0)
}

void OpcodeHandler::JMP_A_DPTR()
{
  JUMP_LOC(*DPTR_REG + *A_REG);
}

void OpcodeHandler::JNB()
{
  DBG_P_BA_REL()
  const fuint32_t bit = READ_BYTE_PC();
  CONDITIONAL_REL_JUMP(CHECK_BIT_ADDRESSABLE_SET(bit) == 0)
}

void OpcodeHandler::JNC()
{
  DBG_P_CODE_ADDR()
  CONDITIONAL_REL_JUMP(GET_CARRY_NO_SHIFT() == 0)
}

void OpcodeHandler::JNZ()
{
  DBG_P_REL()
  CONDITIONAL_REL_JUMP(*A_REG != 0)
}

void OpcodeHandler::JZ()
{
  DBG_P_REL()
  CONDITIONAL_REL_JUMP(*A_REG == 0)
}

void OpcodeHandler::LCALL()
{
  DBG_2B_ADDR()
  auto addr = READ_2BYTE_PC_BIG_ENDIAN();
  CALL_LOC(addr)
}

void OpcodeHandler::LJMP()
{
  DBG_2B_ADDR()
  uint16_t newPC = READ_2BYTE_PC_BIG_ENDIAN();
  PC_REG = newPC;
}

void OpcodeHandler::MOV(uint8_t* dest, uint8_t val)
{
  *dest = val;
}

void OpcodeHandler::MOV_to_C()
{
  DBG_P_BA()
  const fuint32_t byte = READ_BYTE_PC();
  CARRY_CONDITIONAL(CHECK_BIT_ADDRESSABLE_SET(byte) != 0)
}

void OpcodeHandler::MOV_from_C()
{
  CONDITIONAL_BIT_ADRESSABLE_MOV(GET_CARRY_NO_SHIFT() != 0)
}

void OpcodeHandler::MOV_DPTR()
{
  DBG_P_IMM_2B()
  *m_state.regs.DPTR = READ_2BYTE_PC_BIG_ENDIAN();
}

void OpcodeHandler::MOVC_IND_16(uint16_t* half_ptr)
{
  *A_REG = *(m_state.program_memory + (*half_ptr + *A_REG));
}

void OpcodeHandler::MOVX(uint8_t* dest, uint8_t* src)
{
  *dest = *src;
}

void OpcodeHandler::MUL()
{
  RESET_CARRY();
  uint16_union_t res;
  res.v16 = (*A_REG) * (*B_REG);
  *A_REG = res.v8[0];
  *B_REG = res.v8[1];
  OV_CONDITIONAL(res.v8[1] != 0)
}

void OpcodeHandler::ORL_BYTE(uint8_t* dest, uint8_t data)
{
  *dest |= data;
}

void OpcodeHandler::ORL_C(bool cpl)
{
  const uint8_t byte = READ_BYTE_PC();
  CARRY_CONDITIONAL((GET_CARRY_NO_SHIFT() != 0) | (cpl ^ (CHECK_BIT_ADDRESSABLE_SET(byte) != 0)))
}

void OpcodeHandler::POP()
{
  DBG_SINGLE_DATA_ADDR()
  RD_ADDRESS()
  *GET_DATA_ADDRESS() = POP_FROM_STACK();
}

void OpcodeHandler::PUSH()
{
  DBG_SINGLE_DATA_ADDR()
  RD_ADDRESS()
  WRITE_STACK(*GET_DATA_ADDRESS());
}

void OpcodeHandler::RET()
{
  JMP_FROM_STACK();
}

void OpcodeHandler::RETI()
{
  RET();
  // TODO
}

void OpcodeHandler::RL()
{
  *A_REG = ((*A_REG << 1) | (*A_REG >> 7));
}

void OpcodeHandler::RLC()
{
  uint8_t c = GET_CARRY();
  CARRY_CONDITIONAL(((*A_REG) & 0x80) != 0)
  *A_REG = ((*A_REG << 1) | c);
}

void OpcodeHandler::RR()
{
  *A_REG = (*A_REG >> 1) | (*A_REG << 7);
}

void OpcodeHandler::RRC()
{
  uint8_t c = GET_CARRY();
  CARRY_CONDITIONAL(((*A_REG) & 0x01))
  *A_REG = ((*A_REG >> 1) | (c << 7));
}

void OpcodeHandler::SETB_C()
{
  SET_CARRY();
}

void OpcodeHandler::SETB_bit()
{
  DBG_P_BA()
  SET_BIT_ADDRESSABLE();
}

void OpcodeHandler::SJMP()
{
  DBG_P_REL()
  int8_t offset = (int8_t)READ_BYTE_PC();
  JUMP_REL(offset);
}

void OpcodeHandler::SUBB(uint16_t val)
{
  val += GET_CARRY();

  // for auxilary carry flag checking
  uint16_t res = ((*A_REG) & 0x0F) - (val & 0xF);
  AUX_CARRY_CONDITIONAL((res & 0xFFF0) != 0)

  // for overflow checking
  res = ((*A_REG) & 0x7F) - (val & 0x7F);
  bool overflow = (res & 0xFF80) != 0;

  // actual calculation (carry and overflow)
  res = (*A_REG) - val;
  *A_REG = (res & 0xFF);
  bool carry = (res & 0xFF00) != 0;
  CARRY_CONDITIONAL(carry)
  if (carry ^ overflow){ SET_OV(); }
}

void OpcodeHandler::SWAP()
{
  const uint8_t a = *A_REG;
  *A_REG = (a<<4) | (a>>4);
}

void OpcodeHandler::XCH(uint8_t* address)
{
  uint8_t v = *address;
  *address = *A_REG;
  *A_REG = v;
}

void OpcodeHandler::XCHD(uint8_t* address)
{
  auto lower_nibble = *address & 0x0F;
  *address = ((*address) & 0xF0) | ((*A_REG) & 0x0F);
  *A_REG = (*A_REG & 0xF0) | lower_nibble;
}

void OpcodeHandler::XRL(uint8_t* dest, uint8_t data)
{
  *dest ^= data;
}