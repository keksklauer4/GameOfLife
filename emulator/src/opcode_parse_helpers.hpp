#ifndef EMU_OPCODE_PARSE_MACROS_HPP
#define EMU_OPCODE_PARSE_MACROS_HPP

#define A_REG m_state.regs.A
#define B_REG m_state.regs.B
#define DPTR_REG m_state.regs.DPTR
#define PC_REG m_state.regs.PC
#define PC_MEM_LOC (m_state.program_memory + PC_REG)
#define INC_PC() m_state.regs.PC++
#define GET_SP() (*m_state.regs.SP)
#define INC_SP() GET_SP()++
#define DEC_SP() GET_SP()--
#define READ_ADDRESS_1B() ((uint8_t*)(m_state.program_memory + INC_PC()))
#define READ_BYTE_PC() m_state.program_memory[INC_PC()]
#define IMMEDIATE() READ_BYTE_PC()
#define READ_2BYTE_PC() (((uint16_t)READ_BYTE_PC())<<8) | READ_BYTE_PC()
#define SP_LOCATION() m_state.program_memory[GET_SP()]

#define PARSE_REG_BANK() ((*m_state.regs.PSW) & 0b00011000)
#define GET_REG(i) (m_state.program_memory + PARSE_REG_BANK() + i)

#define GET_REG_IND(i) (m_state.internal_data + *GET_REG(i))
#define GET_REG_IND_EXT(i) (m_state.external_memory + *GET_REG(i))
#define DPTR_EXTERNAL() (m_state.external_memory + *DPTR_REG)

constexpr uint16_t AJMP_BITS(uint16_t v)
{
  return v << 8;
}


#define RD_ADDRESS() uint8_t addr = READ_BYTE_PC();
#define GET_DATA_ADDRESS() (addr & 0x80 == 0 ? m_state.internal_data + addr : m_state.sfr_memory + (addr & 0x7F))

// There is one MOV data_address, data_address instruction which is why this dirty copy pasta is needed.
#define RD_ADDRESSES() RD_ADDRESS() uint8_t addr2 = READ_BYTE_PC();
#define GET_DATA_ADDRESS_SEC() (addr2 & 0x80 == 0 ? m_state.internal_data + addr2 : m_state.sfr_memory + (addr2 & 0x7F))

#endif