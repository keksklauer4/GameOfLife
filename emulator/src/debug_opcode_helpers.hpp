#ifndef DEBUG_OPCODE_HELPERS_HPP
#define DEBUG_OPCODE_HELPERS_HPP
#include <string>

#include "opcode_parse_helpers.hpp"

#define __DEBUG__ 1

#if __DEBUG__ == 1
#define DEBUG(content) content
#else
#define DEBUG(content)
#endif

#define OS_OBJ std::cout

#define DBG_PRINT_OPCODE()  \
  DEBUG(OS_OBJ << mnemonics[m_opcode]);

#define READ_BYTE_OFFSET(offset)  \
  (m_state.program_memory[(uint16_t)(PC_REG + offset)])

#define BIT_ADDRESS_OFFSET(offset) (READ_BYTE_OFFSET(offset) & 0xF8) << "." << (READ_BYTE_OFFSET(offset) & 0x07)
#define ADDRESS_OFFSET(offset) static_cast<int>(READ_BYTE_OFFSET(offset))
#define DBG_2B_IMM() (*(uint16_t*)(m_state.program_memory + PC_REG))
#define PC_REL_POS(offset) ((uint16_t)(PC_REG + offset + 1 + ((int)READ_BYTE_OFFSET(offset))))



#define DBG_P_AD() OS_OBJ << "0x" << DBG_2B_IMM() << std::endl;
#define DBG_P_DA() OS_OBJ << "0x" << READ_BYTE_OFFSET(0) << std::endl;
#define DBG_P_BA() OS_OBJ << "0x" << BIT_ADDRESS_OFFSET(offset) << std::endl;
#define DBG_P_BA_DA() OS_OBJ << "0x" << BIT_ADDRESS_OFFSET(offset)  << ", 0x" << READ_BYTE_OFFSET(1) << std::endl;
#define DBG_P_IMM() DEBUG(OS_OBJ <<  std::dec << "#" << CAST(READ_BYTE_OFFSET(0)) << std::endl;)
#define DBG_P_DA_IMM() OS_OBJ << "0x" << READ_BYTE_OFFSET(0) << ", #" << READ_BYTE_OFFSET(0) << std::endl;
#define DBG_P_IMM_CD() OS_OBJ << "#" << READ_BYTE_OFFSET(0) << ", 0x" << PC_REL_POS(1)


#define CAST(v) static_cast<unsigned int>(v)
#define PRINT_REG_DEBUG() DEBUG(std::cout << "Printing Register Debug: " \
  << "A: " << CAST(*A_REG) << "\t B: " << CAST(*B_REG) \
  << "\t R0:" << CAST(*GET_REG(0)) << "\t R1:" << CAST(*GET_REG(1)) << std::endl; \
  std::cout << "PC: 0x" << m_state.regs.PC << std::endl;)


static std::string mnemonics[] = {
  "NOP",    // 0x0
  "AJMP ",   // 0x0
  "LJMP ",    // 0x0
  "RR A",    // 0x0
  "INC A",     // 0x04
  "INC ",       // 0x05
  "INC @R0",   // 0x06
  "INC @R1",   // 0x07
  "INC R0",    // 0x08
  "INC R1",    // 0x09
  "INC R2",    // 0x0A
  "INC R3",    // 0x0B
  "INC R4",    // 0x0C
  "INC R5",    // 0x0D
  "INC R6",    // 0x0E
  "INC R7",    // 0x0F

  "JBC ",       // 0x10
  "ACALL ",     // 0x11
  "LCALL ",     // 0x12
  "RRC A",     // 0x13
  "DEC A",     // 0x14
  "DEC ",       // 0x15
  "DEC @R0",   // 0x16
  "DEC @R1",   // 0x17
  "DEC R0",    // 0x18
  "DEC R1",    // 0x19
  "DEC R2",    // 0x1A
  "DEC R3",    // 0x1B
  "DEC R4",    // 0x1C
  "DEC R5",    // 0x1D
  "DEC R6",    // 0x1E
  "DEC R7",    // 0x1F

  "JB ",       // 0x20
  "AJMP ",     // 0x21
  "RET",     // 0x22
  "RL A",     // 0x23
  "ADD A, ",     // 0x24
  "ADD A, ",       // 0x25
  "ADD A, @R0",   // 0x26
  "ADD A, @R1",   // 0x27
  "ADD A, R0",    // 0x28
  "ADD A, R1",    // 0x29
  "ADD A, R2",    // 0x2A
  "ADD A, R3",    // 0x2B
  "ADD A, R4",    // 0x2C
  "ADD A, R5",    // 0x2D
  "ADD A, R6",    // 0x2E
  "ADD A, R7",    // 0x2F

  "JNB ",       // 0x30
  "ACALL ",     // 0x31
  "RETI",     // 0x32
  "RLC A",     // 0x33
  "ADDC A, ",     // 0x34
  "ADDC A, ",       // 0x35
  "ADDC A, @R0",   // 0x36
  "ADDC A, @R1",   // 0x37
  "ADDC A, R0",    // 0x38
  "ADDC A, R1",    // 0x39
  "ADDC A, R2",    // 0x3A
  "ADDC A, R3",    // 0x3B
  "ADDC A, R4",    // 0x3C
  "ADDC A, R5",    // 0x3D
  "ADDC A, R6",    // 0x3E
  "ADDC A, R7",    // 0x3F

  "JC ",       // 0x40
  "AJMP ",     // 0x41
  "ORL ",     // 0x42
  "ORL ",     // 0x43
  "ORL A, ",     // 0x44
  "ORL A, ",       // 0x45
  "ORL A, @R0",   // 0x46
  "ORL A, @R1",   // 0x47
  "ORL A, R0",    // 0x48
  "ORL A, R1",    // 0x49
  "ORL A, R2",    // 0x4A
  "ORL A, R3",    // 0x4B
  "ORL A, R4",    // 0x4C
  "ORL A, R5",    // 0x4D
  "ORL A, R6",    // 0x4E
  "ORL A, R7",    // 0x4F

  "JNC ",       // 0x50
  "ACALL ",     // 0x51
  "ANL ",     // 0x52
  "ANL ",     // 0x53
  "ANL A, ",     // 0x54
  "ANL A, ",       // 0x55
  "ANL A, @R0",   // 0x56
  "ANL A, @R1",   // 0x57
  "ANL A, R0",    // 0x58
  "ANL A, R1",    // 0x59
  "ANL A, R2",    // 0x5A
  "ANL A, R3",    // 0x5B
  "ANL A, R4",    // 0x5C
  "ANL A, R5",    // 0x5D
  "ANL A, R6",    // 0x5E
  "ANL A, R7",    // 0x5F

  "JZ ",       // 0x60
  "AJMP ",     // 0x61
  "XRL ",     // 0x62
  "XRL ",     // 0x63
  "XRL A, ",     // 0x64
  "XRL A, ",       // 0x65
  "XRL A, @R0",   // 0x66
  "XRL A, @R1",   // 0x67
  "XRL A, R0",    // 0x68
  "XRL A, R1",    // 0x69
  "XRL A, R2",    // 0x6A
  "XRL A, R3",    // 0x6B
  "XRL A, R4",    // 0x6C
  "XRL A, R5",    // 0x6D
  "XRL A, R6",    // 0x6E
  "XRL A, R7",    // 0x6F

  "JNZ ",       // 0x70
  "ACALL ",     // 0x71
  "ORL C, ",     // 0x72
  "JMP @A+DPTR",     // 0x73
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
  "MOVC @A+PC",     // 0x83
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
  "MOVC @A+DPTR",     // 0x93
  "SUBB A, ",     // 0x94
  "SUBB A, ",       // 0x95
  "SUBB A, @R0",   // 0x96
  "SUBB A, @R1",   // 0x97
  "SUBB A, R0",    // 0x98
  "SUBB A, R1",    // 0x99
  "SUBB A, R2",    // 0x9A
  "SUBB A, R3",    // 0x9B
  "SUBB A, R4",    // 0x9C
  "SUBB A, R5",    // 0x9D
  "SUBB A, R6",    // 0x9E
  "SUBB A, R7",    // 0x9F

  "ORL C, /",       // 0xA0
  "AJMP ",     // 0xA1
  "MOV C, ",     // 0xA2
  "INC DPTR",     // 0xA3
  "MUL AB",     // 0xA4
  "<reserved>",       // 0xA5
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

  "ANL C, /",       // 0xB0
  "ACALL ",     // 0xB1
  "CPL ",     // 0xB2
  "CPL C",     // 0xB3
  "CJNE A, ",     // 0xB4
  "CJNE A, ",       // 0xB5
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

  "PUSH ",       // 0xC0
  "AJMP ",     // 0xC1
  "CLR ",     // 0xC2
  "CLR C",     // 0xC3
  "SWAP A",     // 0xC4
  "XCH A, ",       // 0xC5
  "XCH A, @R0",   // 0xC6
  "XCH A, @R1",   // 0xC7
  "XCH A, R0",    // 0xC8
  "XCH A, R1",    // 0xC9
  "XCH A, R2",    // 0xCA
  "XCH A, R3",    // 0xCB
  "XCH A, R4",    // 0xCC
  "XCH A, R5",    // 0xCD
  "XCH A, R6",    // 0xCE
  "XCH A, R7",    // 0xCF

  "POP ",       // 0xD0
  "ACALL ",     // 0xD1
  "SETB ",     // 0xD2
  "SETB C",     // 0xD3
  "DA A",     // 0xD4
  "DJNZ ",       // 0xD5
  "XCHD A, @R0, ",   // 0xD6
  "XCHD A, @R1, ",   // 0xD7
  "DJNZ R0, ",    // 0xD8
  "DJNZ R1, ",    // 0xD9
  "DJNZ R2, ",    // 0xDA
  "DJNZ R3, ",    // 0xDB
  "DJNZ R4, ",    // 0xDC
  "DJNZ R5, ",    // 0xDD
  "DJNZ R6, ",    // 0xDE
  "DJNZ R7, ",    // 0xDF

  "MOVX A, @DPTR",       // 0xE0
  "AJMP ",     // 0xE1
  "MOVX A, @R0",     // 0xE2
  "MOVX A, @R1",     // 0xE3
  "CLR A",     // 0xE4
  "MOV A, ",       // 0xE5
  "MOV A, @R0",   // 0xE6
  "MOV A, @R1",   // 0xE7
  "MOV A, R0",    // 0xE8
  "MOV A, R1",    // 0xE9
  "MOV A, R2",    // 0xEA
  "MOV A, R3",    // 0xEB
  "MOV A, R4",    // 0xEC
  "MOV A, R5",    // 0xED
  "MOV A, R6",    // 0xEE
  "MOV A, R7",    // 0xEF

  "MOVX @DPTR, A",       // 0xF0
  "ACALL ",     // 0xF1
  "MOVX @R0, A",     // 0xF2
  "MOVX @R1, A",     // 0xF3
  "CPL A",     // 0xF4
  "MOV ",       // 0xF5
  "MOV @R0, A",   // 0xF6
  "MOV @R1, A",   // 0xF7
  "MOV R0, A",    // 0xF8
  "MOV R1, A",    // 0xF9
  "MOV R2, A",    // 0xFA
  "MOV R3, A",    // 0xFB
  "MOV R4, A",    // 0xFC
  "MOV R5, A",    // 0xFD
  "MOV R6, A",    // 0xFE
  "MOV R7, A"    // 0xFF
};



#endif