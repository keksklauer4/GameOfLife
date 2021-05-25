#ifndef DEBUG_OPCODE_HELPERS_HPP
#define DEBUG_OPCODE_HELPERS_HPP
#include <string>
#include <map>
#include <cinttypes>

#include "config.hpp"
#include "opcode_parse_helpers.hpp"
#include "types.hpp"

#if __DEBUG__ == 1
#define DEBUG(...) __VA_ARGS__
#else
#define DEBUG(...)
#endif

#define OS_OBJ std::cout

#define DBG_PRINT_OPCODE()  \
  DEBUG(OS_OBJ << mnemonics[m_opcode]);

#define READ_BYTE_OFFSET(offset)  \
  (m_state.program_memory[(uint16_t)(PC_REG + offset)])
#define READ_SIGNED_OFFSET(offset)  \
  (*((int8_t*)(m_state.program_memory + (uint16_t)(PC_REG + offset))))

#define BIT_ADDRESS_OFFSET(offset) (READ_BYTE_OFFSET(offset) & 0xF8) << "." << (READ_BYTE_OFFSET(offset) & 0x07)
#define ADDRESS_OFFSET(offset) static_cast<int>(READ_BYTE_OFFSET(offset))
#define DBG_2B_IMM() (*(uint16_t*)(m_state.program_memory + PC_REG))
#define PC_REL_POS(offset) ((uint16_t)(PC_REG + offset + 1 + ((int)READ_BYTE_OFFSET(offset))))

#define HEX() "0x" << std::hex
#define DECI() std::dec

#define DBG_BIT_ADDRESSABLE(offset)                                     \
  auto v = READ_BYTE_OFFSET(offset);                                    \
  auto it = bit_addressable_map.find(v & 0xF8);                         \
  if (it == bit_addressable_map.end()){ OS_OBJ << HEX() << CAST(v&0xF8); }  \
  else { OS_OBJ << it->second;}                                         \
  OS_OBJ << "." << (v&0x7);

#define DBG_IMMEDIATE(offset)                     \
  OS_OBJ << "#" << DECI() << CAST(READ_BYTE_OFFSET(offset));

#define DBG_REL_CODE(offset)                                    \
  OS_OBJ << HEX() << CAST(PC_REG + 1 + READ_SIGNED_OFFSET(offset));

#define DBG_COMMA() OS_OBJ << ", ";

#define DBG_DATA_ADDR(offset)                   \
  OS_OBJ << HEX() << CAST(READ_BYTE_OFFSET(offset));

#define DBG_CODE_ADDR(offset)                   \
  OS_OBJ << HEX() << CAST(PC_REG + offset + 1 + READ_SIGNED_OFFSET(offset));

#define DBG_CODE_ADDR_ABS(bitmask)                   \
  DEBUG(OS_OBJ << HEX() << CAST(bitmask | READ_BYTE_OFFSET(0));)

#define DBG_P_REL() DEBUG(DBG_REL_CODE(0))
#define DBG_P_BA() DEBUG(DBG_BIT_ADDRESSABLE(0) OS_OBJ << std::endl;)
#define DBG_P_BA_REL() DEBUG(DBG_BIT_ADDRESSABLE(0) DBG_COMMA() DBG_REL_CODE(1) OS_OBJ << std::endl;)
#define DBG_P_IMM() DEBUG(DBG_IMMEDIATE(0) OS_OBJ << std::endl;)
#define DBG_P_IMM_REL() DEBUG(DBG_IMMEDIATE(0) DBG_COMMA() DBG_REL_CODE(1) OS_OBJ << std::endl;)
#define DBG_AJMP() DEBUG(OS_OBJ << HEX() << CAST((AJMP_BITS(m_opcode >> 5)) | ((uint16_t)READ_BYTE_OFFSET(0)) | ((PC_REG + 1) & 0xF800)) << std::endl;)
#define DBG_ACALL() DBG_AJMP()
#define DBG_2B_ADDR() DEBUG(OS_OBJ << HEX() << CAST(BSWAP16(DBG_2B_IMM())) << std::endl;)
#define DBG_P_IMM_2B() DEBUG(OS_OBJ << HEX() << ((((uint16_t)READ_BYTE_OFFSET(0)) << 8) | READ_BYTE_OFFSET(1)) << std::endl;)
#define DBG_SINGLE_DATA_ADDR() DEBUG(OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(0)] << std::endl;)
#define DBG_DATA_ADDR_OP(operand) DEBUG(OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(0)]; DBG_COMMA() OS_OBJ << #operand << std::endl;)
#define DBG_DATA_ADDR_IMM() DEBUG(OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(0)]; DBG_COMMA() DBG_IMMEDIATE(1) OS_OBJ << std::endl;)
#define DBG_8_BIT_REG_BIN(NAME, ptr) DEBUG(OS_OBJ << #NAME << ": \t0b" << std::bitset<8>{*(ptr)} << std::endl;)
#define DBG_2_DATA_ADDR() DEBUG(OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(1)]; DBG_COMMA() OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(0)] << std::endl;)
#define DBG_P_CODE_ADDR() DEBUG(DBG_CODE_ADDR(0) OS_OBJ << std::endl;)
#define DBG_P_DATA_ADDR_CODE_ADDR() DEBUG(OS_OBJ << data_address_dbg[READ_BYTE_OFFSET(0)]; DBG_COMMA() DBG_CODE_ADDR(1) OS_OBJ << std::endl;)

#define PRINT_REG_DEBUG() DEBUG(                                                             \
  OS_OBJ << "Printing Register Debug: "                                                      \
    << "A: " << CAST(*A_REG) << "\t B: " << CAST(*B_REG) << std::endl;                       \
  for (fuint32_t registerBank = 0; registerBank < 4; registerBank++)                         \
  {                                                                                          \
    OS_OBJ << "Bank " << registerBank << ": \t";                                             \
    for(fuint32_t reg = 0; reg < 8; ++reg) {                                                 \
      std::cout << "R" << reg << ": "                                                        \
        << std::hex << CAST(*(m_state.internal_data + reg + (registerBank << 3))) << "\t";   \
    }                                                                                        \
    OS_OBJ << std::endl;                                                                     \
  }                                                                                          \
  for (fuint32_t port = 0; port < 4; ++port)                                                 \
  {                                                                                          \
    OS_OBJ << "Port " << port << ": 0x" << CAST(*(m_state.regs.P0 + (port << 4))) << "\t";     \
  }                                                                                          \
  OS_OBJ << std::endl;                                                                       \
  OS_OBJ << "SP: \t" << CAST(*m_state.regs.SP) << std::endl;                                     \
  DBG_8_BIT_REG_BIN(PSW, m_state.regs.PSW)                                                   \
  DBG_8_BIT_REG_BIN(IP, m_state.regs.IP)                                                     \
  DBG_8_BIT_REG_BIN(IE, m_state.regs.IE)                                                     \
  DBG_8_BIT_REG_BIN(TCON, m_state.regs.TCON)                                                 \
  DBG_8_BIT_REG_BIN(TMOD, m_state.regs.TMOD)                                                 \
  DBG_8_BIT_REG_BIN(PCON, m_state.regs.PCON)                                                 \
  DBG_8_BIT_REG_BIN(SCON, m_state.regs.SCON)                                                 \
  DBG_8_BIT_REG_BIN(SBUF, m_state.regs.SBUF)                                                 \
  OS_OBJ << "DPTR: 0x" << std::hex << CAST(*m_state.regs.DPTR) << std::endl;                 \
  OS_OBJ << "PC: 0x" << (PC_REG - 1) << std::endl;)


static const std::map<uint8_t, std::string> bit_addressable_map{
  { 0x80, "P0" },
  { 0x88, "TCON" },
  { 0x90, "P1" },
  { 0x98, "SCON" },
  { 0xA0, "P2" },
  { 0xA8, "IE" },
  { 0xB0, "P3" },
  { 0xB8, "IP" },
  { 0xC8, "T2CON" },
  { 0xD0, "PSW" },
  { 0xE0, "ACC" },
  { 0xF0, "B" }
};

static std::string mnemonics[] = {
  "NOP\n",        // 0x00
  "AJMP ",      // 0x01
  "LJMP ",      // 0x02
  "RR A\n",       // 0x03
  "INC A\n",      // 0x04
  "INC ",       // 0x05
  "INC @R0\n",   // 0x06
  "INC @R1\n",   // 0x07
  "INC R0\n",    // 0x08
  "INC R1\n",    // 0x09
  "INC R2\n",    // 0x0A
  "INC R3\n",    // 0x0B
  "INC R4\n",    // 0x0C
  "INC R5\n",    // 0x0D
  "INC R6\n",    // 0x0E
  "INC R7\n",    // 0x0F

  "JBC ",       // 0x10
  "ACALL ",     // 0x11
  "LCALL ",     // 0x12
  "RRC A\n",     // 0x13
  "DEC A\n",     // 0x14
  "DEC ",       // 0x15
  "DEC @R0\n",   // 0x16
  "DEC @R1\n",   // 0x17
  "DEC R0\n",    // 0x18
  "DEC R1\n",    // 0x19
  "DEC R2\n",    // 0x1A
  "DEC R3\n",    // 0x1B
  "DEC R4\n",    // 0x1C
  "DEC R5\n",    // 0x1D
  "DEC R6\n",    // 0x1E
  "DEC R7\n",    // 0x1F

  "JB ",       // 0x20
  "AJMP ",     // 0x21
  "RET\n",     // 0x22
  "RL A\n",     // 0x23
  "ADD A, ",     // 0x24
  "ADD A, ",       // 0x25
  "ADD A, @R0\n",   // 0x26
  "ADD A, @R1\n",   // 0x27
  "ADD A, R0\n",    // 0x28
  "ADD A, R1\n",    // 0x29
  "ADD A, R2\n",    // 0x2A
  "ADD A, R3\n",    // 0x2B
  "ADD A, R4\n",    // 0x2C
  "ADD A, R5\n",    // 0x2D
  "ADD A, R6\n",    // 0x2E
  "ADD A, R7\n",    // 0x2F

  "JNB ",       // 0x30
  "ACALL ",     // 0x31
  "RETI\n",     // 0x32
  "RLC A\n",     // 0x33
  "ADDC A, ",     // 0x34
  "ADDC A, ",       // 0x35
  "ADDC A, @R0\n",   // 0x36
  "ADDC A, @R1\n",   // 0x37
  "ADDC A, R0\n",    // 0x38
  "ADDC A, R1\n",    // 0x39
  "ADDC A, R2\n",    // 0x3A
  "ADDC A, R3\n",    // 0x3B
  "ADDC A, R4\n",    // 0x3C
  "ADDC A, R5\n",    // 0x3D
  "ADDC A, R6\n",    // 0x3E
  "ADDC A, R7\n",    // 0x3F

  "JC ",       // 0x40
  "AJMP ",     // 0x41
  "ORL ",     // 0x42
  "ORL ",     // 0x43
  "ORL A, ",     // 0x44
  "ORL A, ",       // 0x45
  "ORL A, @R0\n",   // 0x46
  "ORL A, @R1\n",   // 0x47
  "ORL A, R0\n",    // 0x48
  "ORL A, R1\n",    // 0x49
  "ORL A, R2\n",    // 0x4A
  "ORL A, R3\n",    // 0x4B
  "ORL A, R4\n",    // 0x4C
  "ORL A, R5\n",    // 0x4D
  "ORL A, R6\n",    // 0x4E
  "ORL A, R7\n",    // 0x4F

  "JNC ",       // 0x50
  "ACALL ",     // 0x51
  "ANL ",     // 0x52
  "ANL ",     // 0x53
  "ANL A, ",     // 0x54
  "ANL A, ",       // 0x55
  "ANL A, @R0\n",   // 0x56
  "ANL A, @R1\n",   // 0x57
  "ANL A, R0\n",    // 0x58
  "ANL A, R1\n",    // 0x59
  "ANL A, R2\n",    // 0x5A
  "ANL A, R3\n",    // 0x5B
  "ANL A, R4\n",    // 0x5C
  "ANL A, R5\n",    // 0x5D
  "ANL A, R6\n",    // 0x5E
  "ANL A, R7\n",    // 0x5F

  "JZ ",       // 0x60
  "AJMP ",     // 0x61
  "XRL ",     // 0x62
  "XRL ",     // 0x63
  "XRL A, ",     // 0x64
  "XRL A, ",       // 0x65
  "XRL A, @R0\n",   // 0x66
  "XRL A, @R1\n",   // 0x67
  "XRL A, R0\n",    // 0x68
  "XRL A, R1\n",    // 0x69
  "XRL A, R2\n",    // 0x6A
  "XRL A, R3\n",    // 0x6B
  "XRL A, R4\n",    // 0x6C
  "XRL A, R5\n",    // 0x6D
  "XRL A, R6\n",    // 0x6E
  "XRL A, R7\n",    // 0x6F

  "JNZ ",       // 0x70
  "ACALL ",     // 0x71
  "ORL C, ",     // 0x72
  "JMP @A+DPTR\n",     // 0x73
  "MOV A, ",     // 0x74
  "MOV ",       // 0x75
  "MOV @R0, ",   // 0x76
  "MOV @R1, ",   // 0x77
  "MOV R0, ",    // 0x78
  "MOV R1, ",    // 0x79
  "MOV R2, ",    // 0x7A
  "MOV R3, ",    // 0x7B
  "MOV R4, ",    // 0x7C
  "MOV R5, ",    // 0x7D
  "MOV R6, ",    // 0x7E
  "MOV R7, ",    // 0x7F

  "SJMP ",       // 0x80
  "AJMP ",     // 0x81
  "ANL C, ",     // 0x82
  "MOVC A, @A+PC\n",     // 0x83
  "DIV AB",     // 0x84
  "MOV ",       // 0x85
  "MOV ",   // 0x86
  "MOV ",   // 0x87
  "MOV ",    // 0x88
  "MOV ",    // 0x89
  "MOV ",    // 0x8A
  "MOV ",    // 0x8B
  "MOV ",    // 0x8C
  "MOV ",    // 0x8D
  "MOV ",    // 0x8E
  "MOV ",    // 0x8F

  "MOV DPTR, ",       // 0x90
  "ACALL ",     // 0x91
  "MOV ",     // 0x92
  "MOVC A, @A+DPTR\n",     // 0x93
  "SUBB A, ",     // 0x94
  "SUBB A, ",       // 0x95
  "SUBB A, @R0\n",   // 0x96
  "SUBB A, @R1\n",   // 0x97
  "SUBB A, R0\n",    // 0x98
  "SUBB A, R1\n",    // 0x99
  "SUBB A, R2\n",    // 0x9A
  "SUBB A, R3\n",    // 0x9B
  "SUBB A, R4\n",    // 0x9C
  "SUBB A, R5\n",    // 0x9D
  "SUBB A, R6\n",    // 0x9E
  "SUBB A, R7\n",    // 0x9F

  "ORL C, /",       // 0xA0
  "AJMP ",       // 0xA1
  "MOV C, ",     // 0xA2
  "INC DPTR\n",    // 0xA3
  "MUL AB\n",      // 0xA4
  "<reserved>\n",  // 0xA5
  "MOV @R0, ",   // 0xA6
  "MOV @R1, ",   // 0xA7
  "MOV R0, ",    // 0xA8
  "MOV R1, ",    // 0xA9
  "MOV R2, ",    // 0xAA
  "MOV R3, ",    // 0xAB
  "MOV R4, ",    // 0xAC
  "MOV R5, ",    // 0xAD
  "MOV R6, ",    // 0xAE
  "MOV R7, ",    // 0xAF

  "ANL C, /",     // 0xB0
  "ACALL ",       // 0xB1
  "CPL ",         // 0xB2
  "CPL C\n",        // 0xB3
  "CJNE A, ",     // 0xB4
  "CJNE A, ",     // 0xB5
  "CJNE @R0, ",   // 0xB6
  "CJNE @R1, ",   // 0xB7
  "CJNE R0, ",    // 0xB8
  "CJNE R1, ",    // 0xB9
  "CJNE R2, ",    // 0xBA
  "CJNE R3, ",    // 0xBB
  "CJNE R4, ",    // 0xBC
  "CJNE R5, ",    // 0xBD
  "CJNE R6, ",    // 0xBE
  "CJNE R7, ",    // 0xBF

  "PUSH ",        // 0xC0
  "AJMP ",        // 0xC1
  "CLR ",         // 0xC2
  "CLR C\n",        // 0xC3
  "SWAP A\n",       // 0xC4
  "XCH A, ",      // 0xC5
  "XCH A, @R0\n",   // 0xC6
  "XCH A, @R1\n",   // 0xC7
  "XCH A, R0\n",    // 0xC8
  "XCH A, R1\n",    // 0xC9
  "XCH A, R2\n",    // 0xCA
  "XCH A, R3\n",    // 0xCB
  "XCH A, R4\n",    // 0xCC
  "XCH A, R5\n",    // 0xCD
  "XCH A, R6\n",    // 0xCE
  "XCH A, R7\n",    // 0xCF

  "POP ",         // 0xD0
  "ACALL ",       // 0xD1
  "SETB ",        // 0xD2
  "SETB C\n",       // 0xD3
  "DA A\n",         // 0xD4
  "DJNZ ",        // 0xD5
  "XCHD A, @R0, ",// 0xD6
  "XCHD A, @R1, ",// 0xD7
  "DJNZ R0, ",    // 0xD8
  "DJNZ R1, ",    // 0xD9
  "DJNZ R2, ",    // 0xDA
  "DJNZ R3, ",    // 0xDB
  "DJNZ R4, ",    // 0xDC
  "DJNZ R5, ",    // 0xDD
  "DJNZ R6, ",    // 0xDE
  "DJNZ R7, ",    // 0xDF

  "MOVX A, @DPTR\n",       // 0xE0
  "AJMP ",     // 0xE1
  "MOVX A, @R0\n",     // 0xE2
  "MOVX A, @R1\n",     // 0xE3
  "CLR A\n",     // 0xE4
  "MOV A, ",       // 0xE5
  "MOV A, @R0\n",   // 0xE6
  "MOV A, @R1\n",   // 0xE7
  "MOV A, R0\n",    // 0xE8
  "MOV A, R1\n",    // 0xE9
  "MOV A, R2\n",    // 0xEA
  "MOV A, R3\n",    // 0xEB
  "MOV A, R4\n",    // 0xEC
  "MOV A, R5\n",    // 0xED
  "MOV A, R6\n",    // 0xEE
  "MOV A, R7\n",    // 0xEF

  "MOVX @DPTR, A\n",       // 0xF0
  "ACALL ",     // 0xF1
  "MOVX @R0, A\n",     // 0xF2
  "MOVX @R1, A\n",     // 0xF3
  "CPL A\n",     // 0xF4
  "MOV ",       // 0xF5
  "MOV @R0, A\n",   // 0xF6
  "MOV @R1, A\n",   // 0xF7
  "MOV R0, A\n",    // 0xF8
  "MOV R1, A\n",    // 0xF9
  "MOV R2, A\n",    // 0xFA
  "MOV R3, A\n",    // 0xFB
  "MOV R4, A\n",    // 0xFC
  "MOV R5, A\n",    // 0xFD
  "MOV R6, A\n",    // 0xFE
  "MOV R7, A\n"    // 0xFF
};

static const uint8_t cycles[] = {
  1,  // 0x00
  2,  // 0x01
  2,  // 0x02
  1,  // 0x03
  1,  // 0x04
  1,  // 0x05
  1,  // 0x06
  1,  // 0x07
  1,  // 0x08
  1,  // 0x09
  1,  // 0x0A
  1,  // 0x0B
  1,  // 0x0C
  1,  // 0x0D
  1,  // 0x0E
  1,  // 0x0F

  2,  // 0x10
  2,  // 0x11
  2,  // 0x12
  1,  // 0x13
  1,  // 0x14
  1,  // 0x15
  1,  // 0x16
  1,  // 0x17
  1,  // 0x18
  1,  // 0x19
  1,  // 0x1A
  1,  // 0x1B
  1,  // 0x1C
  1,  // 0x1D
  1,  // 0x1E
  1,  // 0x1F

  2,  // 0x20
  2,  // 0x21
  2,  // 0x22
  1,  // 0x23
  1,  // 0x24
  1,  // 0x25
  1,  // 0x26
  1,  // 0x27
  1,  // 0x28
  1,  // 0x29
  1,  // 0x2A
  1,  // 0x2B
  1,  // 0x2C
  1,  // 0x2D
  1,  // 0x2E
  1,  // 0x2F

  2,  // 0x30
  2,  // 0x31
  2,  // 0x32
  1,  // 0x33
  1,  // 0x34
  1,  // 0x35
  1,  // 0x36
  1,  // 0x37
  1,  // 0x38
  1,  // 0x39
  1,  // 0x3A
  1,  // 0x3B
  1,  // 0x3C
  1,  // 0x3D
  1,  // 0x3E
  1,  // 0x3F

  2,  // 0x40
  2,  // 0x41
  1,  // 0x42
  2,  // 0x43
  1,  // 0x44
  1,  // 0x45
  1,  // 0x46
  1,  // 0x47
  1,  // 0x48
  1,  // 0x49
  1,  // 0x4A
  1,  // 0x4B
  1,  // 0x4C
  1,  // 0x4D
  1,  // 0x4E
  1,  // 0x4F

  2,  // 0x50
  2,  // 0x51
  1,  // 0x52
  2,  // 0x53
  1,  // 0x54
  1,  // 0x55
  1,  // 0x56
  1,  // 0x57
  1,  // 0x58
  1,  // 0x59
  1,  // 0x5A
  1,  // 0x5B
  1,  // 0x5C
  1,  // 0x5D
  1,  // 0x5E
  1,  // 0x5F

  2,  // 0x60
  2,  // 0x61
  1,  // 0x62
  2,  // 0x63
  1,  // 0x64
  1,  // 0x65
  1,  // 0x66
  1,  // 0x67
  1,  // 0x68
  1,  // 0x69
  1,  // 0x6A
  1,  // 0x6B
  1,  // 0x6C
  1,  // 0x6D
  1,  // 0x6E
  1,  // 0x6F

  2,  // 0x70
  2,  // 0x71
  2,  // 0x72
  2,  // 0x73
  1,  // 0x74
  2,  // 0x75
  1,  // 0x76
  1,  // 0x77
  1,  // 0x78
  1,  // 0x79
  1,  // 0x7A
  1,  // 0x7B
  1,  // 0x7C
  1,  // 0x7D
  1,  // 0x7E
  1,  // 0x7F

  2,  // 0x80
  2,  // 0x81
  2,  // 0x82
  2,  // 0x83
  4,  // 0x84
  2,  // 0x85
  2,  // 0x86
  2,  // 0x87
  2,  // 0x88
  2,  // 0x89
  2,  // 0x8A
  2,  // 0x8B
  2,  // 0x8C
  2,  // 0x8D
  2,  // 0x8E
  2,  // 0x8F

  2,  // 0x90
  2,  // 0x91
  2,  // 0x92
  2,  // 0x93
  1,  // 0x94
  1,  // 0x95
  1,  // 0x96
  1,  // 0x97
  1,  // 0x98
  1,  // 0x99
  1,  // 0x9A
  1,  // 0x9B
  1,  // 0x9C
  1,  // 0x9D
  1,  // 0x9E
  1,  // 0x9F

  2,  // 0xA0
  2,  // 0xA1
  1,  // 0xA2
  2,  // 0xA3
  4,  // 0xA4
  0,  // 0xA5
  2,  // 0xA6
  2,  // 0xA7
  2,  // 0xA8
  2,  // 0xA9
  2,  // 0xAA
  2,  // 0xAB
  2,  // 0xAC
  2,  // 0xAD
  2,  // 0xAE
  2,  // 0xAF

  2,  // 0xB0
  2,  // 0xB1
  1,  // 0xB2
  1,  // 0xB3
  2,  // 0xB4
  2,  // 0xB5
  2,  // 0xB6
  2,  // 0xB7
  2,  // 0xB8
  2,  // 0xB9
  2,  // 0xBA
  2,  // 0xBB
  2,  // 0xBC
  2,  // 0xBD
  2,  // 0xBE
  2,  // 0xBF

  1,  // 0xC0
  2,  // 0xC1
  1,  // 0xC2
  1,  // 0xC3
  1,  // 0xC4
  1,  // 0xC5
  1,  // 0xC6
  1,  // 0xC7
  1,  // 0xC8
  1,  // 0xC9
  1,  // 0xCA
  1,  // 0xCB
  1,  // 0xCC
  1,  // 0xCD
  1,  // 0xCE
  1,  // 0xCF

  2,  // 0xD0
  2,  // 0xD1
  1,  // 0xD2
  1,  // 0xD3
  1,  // 0xD4
  2,  // 0xD5
  1,  // 0xD6
  1,  // 0xD7
  2,  // 0xD8
  2,  // 0xD9
  2,  // 0xDA
  2,  // 0xDB
  2,  // 0xDC
  2,  // 0xDD
  2,  // 0xDE
  2,  // 0xDF

  2,  // 0xE0
  2,  // 0xE1
  2,  // 0xE2
  2,  // 0xE3
  1,  // 0xE4
  1,  // 0xE5
  1,  // 0xE6
  1,  // 0xE7
  1,  // 0xE8
  1,  // 0xE9
  1,  // 0xEA
  1,  // 0xEB
  1,  // 0xEC
  1,  // 0xED
  1,  // 0xEE
  1,  // 0xEF

  2,  // 0xF0
  2,  // 0xF1
  2,  // 0xF2
  2,  // 0xF3
  1,  // 0xF4
  1,  // 0xF5
  1,  // 0xF6
  1,  // 0xF7
  1,  // 0xF8
  1,  // 0xF9
  1,  // 0xFA
  1,  // 0xFB
  1,  // 0xFC
  1,  // 0xFD
  1,  // 0xFE
  1   // 0xFF
};

DEBUG(static const char* data_address_dbg[]{
  "0x00", "0x01", "0x02", "0x03", "0x04", "0x05", "0x06", "0x07",
  "0x08", "0x09", "0x0a", "0x0b", "0x0c", "0x0d", "0x0e", "0x0f",
  "0x10", "0x11", "0x12", "0x13", "0x14", "0x15", "0x16", "0x17",
  "0x18", "0x19", "0x1a", "0x1b", "0x1c", "0x1d", "0x1e", "0x1f",
  "0x20", "0x21", "0x22", "0x23", "0x24", "0x25", "0x26", "0x27",
  "0x28", "0x29", "0x2a", "0x2b", "0x2c", "0x2d", "0x2e", "0x2f",
  "0x30", "0x31", "0x32", "0x33", "0x34", "0x35", "0x36", "0x37",
  "0x38", "0x39", "0x3a", "0x3b", "0x3c", "0x3d", "0x3e", "0x3f",
  "0x40", "0x41", "0x42", "0x43", "0x44", "0x45", "0x46", "0x47",
  "0x48", "0x49", "0x4a", "0x4b", "0x4c", "0x4d", "0x4e", "0x4f",
  "0x50", "0x51", "0x52", "0x53", "0x54", "0x55", "0x56", "0x57",
  "0x58", "0x59", "0x5a", "0x5b", "0x5c", "0x5d", "0x5e", "0x5f",
  "0x60", "0x61", "0x62", "0x63", "0x64", "0x65", "0x66", "0x67",
  "0x68", "0x69", "0x6a", "0x6b", "0x6c", "0x6d", "0x6e", "0x6f",
  "0x70", "0x71", "0x72", "0x73", "0x74", "0x75", "0x76", "0x77",
  "0x78", "0x79", "0x7a", "0x7b", "0x7c", "0x7d", "0x7e", "0x7f",
  "P0",   "SP",   "DPL",  "DPH",  "0x84", "0x85", "0x86", "PCON",
  "TCON", "TMOD", "TL0",  "TL1",  "TH0",  "TH1",  "0x8e", "0x8f",
  "P1",   "0x91", "0x92", "0x93", "0x94", "0x95", "0x96", "0x97",
  "SCON", "SBUF", "0x9a", "0x9b", "0x9c", "0x9d", "0x9e", "0x9f",
  "P2",   "0xa1", "0xa2", "0xa3", "0xa4", "0xa5", "0xa6", "0xa7",
  "IE",   "0xa9", "0xaa", "0xab", "0xac", "0xad", "0xae", "0xaf",
  "P3",   "0xb1", "0xb2", "0xb3", "0xb4", "0xb5", "0xb6", "0xb7",
  "IP",   "0xb9", "0xba", "0xbb", "0xbc", "0xbd", "0xbe", "0xbf",
  "0xc0", "0xc1", "0xc2", "0xc3", "0xc4", "0xc5", "0xc6", "0xc7",
  "0xc8", "0xc9", "0xca", "0xcb", "0xcc", "0xcd", "0xce", "0xcf",
  "PSW",  "0xd1", "0xd2", "0xd3", "0xd4", "0xd5", "0xd6", "0xd7",
  "0xd8", "0xd9", "0xda", "0xdb", "0xdc", "0xdd", "0xde", "0xdf",
  "ACC",  "0xe1", "0xe2", "0xe3", "0xe4", "0xe5", "0xe6", "0xe7",
  "0xe8", "0xe9", "0xea", "0xeb", "0xec", "0xed", "0xee", "0xef",
  "B",    "0xf1", "0xf2", "0xf3", "0xf4", "0xf5", "0xf6", "0xf7",
  "0xf8", "0xf9", "0xfa", "0xfb", "0xfc", "0xfd", "0xfe", "0xff"
};)

#endif