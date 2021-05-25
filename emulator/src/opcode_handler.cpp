#include "opcode_handler.hpp"
#include "opcode_macros.hpp"
#include "intrinsics.hpp"

#define HANDLE_PARITY_BIT() if ((POPCOUNT(*A_REG) & 0x01) != 0){ *m_state.regs.PSW |= 0x01; } else { *m_state.regs.PSW &= 0xFE; }


using namespace emu;

void OpcodeHandler::execOpcode()
{
  m_opcode = READ_BYTE_PC();
  DEBUG(std::cout << "Opcode to execute: " << std::hex <<
    static_cast<unsigned int>(m_opcode) << std::endl;)
  HANDLE_PARITY_BIT()
  PRINT_REG_DEBUG()
  DBG_PRINT_OPCODE()
  (*this.*(m_jpTable[m_opcode]))();
  m_state.last_opcode = cycles[m_opcode];
  m_state.cycles_passed += m_state.last_opcode;
  DEBUG(std::cout << std::endl;)
}

void OpcodeHandler::jumpInterrupt(uint16_t address)
{
  DEBUG(std::cout << "Interrupt at 0x" << std::hex << address << std::endl;)
  CALL_LOC(address);
  STOP_IDLE_MODE(); // interrupts reset idle mode bit
}

void OpcodeHandler::RESERVED_OPCODE()
{
  THROW_EXCEPTION(throw std::runtime_error("Next opcode is <reserved> which should not happen!");)
}

void OpcodeHandler::init()
{
  m_jpTable[0x00] = &OpcodeHandler::NOP;
  m_jpTable[0x01] = &OpcodeHandler::AJMP_01;
  m_jpTable[0x02] = &OpcodeHandler::LJMP;
  m_jpTable[0x03] = &OpcodeHandler::RR;
  m_jpTable[0x04] = &OpcodeHandler::INC_A_04;
  m_jpTable[0x05] = &OpcodeHandler::INC_data_addr_05;
  m_jpTable[0x06] = &OpcodeHandler::INC_at_R0_06;
  m_jpTable[0x07] = &OpcodeHandler::INC_at_R1_07;
  m_jpTable[0x08] = &OpcodeHandler::INC_R0_08;
  m_jpTable[0x09] = &OpcodeHandler::INC_R1_09;
  m_jpTable[0x0A] = &OpcodeHandler::INC_R2_0A;
  m_jpTable[0x0B] = &OpcodeHandler::INC_R3_0B;
  m_jpTable[0x0C] = &OpcodeHandler::INC_R4_0C;
  m_jpTable[0x0D] = &OpcodeHandler::INC_R5_0D;
  m_jpTable[0x0E] = &OpcodeHandler::INC_R6_0E;
  m_jpTable[0x0F] = &OpcodeHandler::INC_R7_0F;
  m_jpTable[0x10] = &OpcodeHandler::JBC;
  m_jpTable[0x11] = &OpcodeHandler::ACALL_11;
  m_jpTable[0x12] = &OpcodeHandler::LCALL;
  m_jpTable[0x13] = &OpcodeHandler::RRC;
  m_jpTable[0x14] = &OpcodeHandler::DEC_A_14;
  m_jpTable[0x15] = &OpcodeHandler::DEC_data_addr_15;
  m_jpTable[0x16] = &OpcodeHandler::DEC_at_R0_16;
  m_jpTable[0x17] = &OpcodeHandler::DEC_at_R1_17;
  m_jpTable[0x18] = &OpcodeHandler::DEC_R0_18;
  m_jpTable[0x19] = &OpcodeHandler::DEC_R1_19;
  m_jpTable[0x1A] = &OpcodeHandler::DEC_R2_1A;
  m_jpTable[0x1B] = &OpcodeHandler::DEC_R3_1B;
  m_jpTable[0x1C] = &OpcodeHandler::DEC_R4_1C;
  m_jpTable[0x1D] = &OpcodeHandler::DEC_R5_1D;
  m_jpTable[0x1E] = &OpcodeHandler::DEC_R6_1E;
  m_jpTable[0x1F] = &OpcodeHandler::DEC_R7_1F;
  m_jpTable[0x20] = &OpcodeHandler::JB;
  m_jpTable[0x21] = &OpcodeHandler::AJMP_21;
  m_jpTable[0x22] = &OpcodeHandler::RET;
  m_jpTable[0x23] = &OpcodeHandler::RL;
  m_jpTable[0x24] = &OpcodeHandler::ADD_data_24;
  m_jpTable[0x25] = &OpcodeHandler::ADD_data_addr_25;
  m_jpTable[0x26] = &OpcodeHandler::ADD_at_R0_26;
  m_jpTable[0x27] = &OpcodeHandler::ADD_at_R1_27;
  m_jpTable[0x28] = &OpcodeHandler::ADD_R0_28;
  m_jpTable[0x29] = &OpcodeHandler::ADD_R1_29;
  m_jpTable[0x2A] = &OpcodeHandler::ADD_R2_2A;
  m_jpTable[0x2B] = &OpcodeHandler::ADD_R3_2B;
  m_jpTable[0x2C] = &OpcodeHandler::ADD_R4_2C;
  m_jpTable[0x2D] = &OpcodeHandler::ADD_R5_2D;
  m_jpTable[0x2E] = &OpcodeHandler::ADD_R6_2E;
  m_jpTable[0x2F] = &OpcodeHandler::ADD_R7_2F;
  m_jpTable[0x30] = &OpcodeHandler::JNB;
  m_jpTable[0x31] = &OpcodeHandler::ACALL_31;
  m_jpTable[0x32] = &OpcodeHandler::RETI;
  m_jpTable[0x33] = &OpcodeHandler::RLC;
  m_jpTable[0x34] = &OpcodeHandler::ADDC_data_34;
  m_jpTable[0x35] = &OpcodeHandler::ADDC_data_addr_35;
  m_jpTable[0x36] = &OpcodeHandler::ADDC_at_R0_36;
  m_jpTable[0x37] = &OpcodeHandler::ADDC_at_R1_37;
  m_jpTable[0x38] = &OpcodeHandler::ADDC_R0_38;
  m_jpTable[0x39] = &OpcodeHandler::ADDC_R1_39;
  m_jpTable[0x3A] = &OpcodeHandler::ADDC_R2_3A;
  m_jpTable[0x3B] = &OpcodeHandler::ADDC_R3_3B;
  m_jpTable[0x3C] = &OpcodeHandler::ADDC_R4_3C;
  m_jpTable[0x3D] = &OpcodeHandler::ADDC_R5_3D;
  m_jpTable[0x3E] = &OpcodeHandler::ADDC_R6_3E;
  m_jpTable[0x3F] = &OpcodeHandler::ADDC_R7_3F;
  m_jpTable[0x40] = &OpcodeHandler::JC;
  m_jpTable[0x41] = &OpcodeHandler::AJMP_41;
  m_jpTable[0x42] = &OpcodeHandler::ORL_data_addr_A_42;
  m_jpTable[0x43] = &OpcodeHandler::ORL_data_addr_data_43;
  m_jpTable[0x44] = &OpcodeHandler::ORL_data_44;
  m_jpTable[0x45] = &OpcodeHandler::ORL_data_addr_45;
  m_jpTable[0x46] = &OpcodeHandler::ORL_at_R0_46;
  m_jpTable[0x47] = &OpcodeHandler::ORL_at_R1_47;
  m_jpTable[0x48] = &OpcodeHandler::ORL_R0_48;
  m_jpTable[0x49] = &OpcodeHandler::ORL_R1_49;
  m_jpTable[0x4A] = &OpcodeHandler::ORL_R2_4A;
  m_jpTable[0x4B] = &OpcodeHandler::ORL_R3_4B;
  m_jpTable[0x4C] = &OpcodeHandler::ORL_R4_4C;
  m_jpTable[0x4D] = &OpcodeHandler::ORL_R5_4D;
  m_jpTable[0x4E] = &OpcodeHandler::ORL_R6_4E;
  m_jpTable[0x4F] = &OpcodeHandler::ORL_R7_4F;
  m_jpTable[0x50] = &OpcodeHandler::JNC;
  m_jpTable[0x51] = &OpcodeHandler::ACALL_51;
  m_jpTable[0x52] = &OpcodeHandler::ANL_data_addr_A_52;
  m_jpTable[0x53] = &OpcodeHandler::ANL_data_addr_data_53;
  m_jpTable[0x54] = &OpcodeHandler::ANL_data_54;
  m_jpTable[0x55] = &OpcodeHandler::ANL_data_addr_55;
  m_jpTable[0x56] = &OpcodeHandler::ANL_at_R0_56;
  m_jpTable[0x57] = &OpcodeHandler::ANL_at_R1_57;
  m_jpTable[0x58] = &OpcodeHandler::ANL_R0_58;
  m_jpTable[0x59] = &OpcodeHandler::ANL_R1_59;
  m_jpTable[0x5A] = &OpcodeHandler::ANL_R2_5A;
  m_jpTable[0x5B] = &OpcodeHandler::ANL_R3_5B;
  m_jpTable[0x5C] = &OpcodeHandler::ANL_R4_5C;
  m_jpTable[0x5D] = &OpcodeHandler::ANL_R5_5D;
  m_jpTable[0x5E] = &OpcodeHandler::ANL_R6_5E;
  m_jpTable[0x5F] = &OpcodeHandler::ANL_R7_5F;
  m_jpTable[0x60] = &OpcodeHandler::JZ;
  m_jpTable[0x61] = &OpcodeHandler::AJMP_61;
  m_jpTable[0x62] = &OpcodeHandler::XRL_data_addr_A_62;
  m_jpTable[0x63] = &OpcodeHandler::XRL_data_addr_data_63;
  m_jpTable[0x64] = &OpcodeHandler::XRL_data_64;
  m_jpTable[0x65] = &OpcodeHandler::XRL_data_addr_65;
  m_jpTable[0x66] = &OpcodeHandler::XRL_at_R0_66;
  m_jpTable[0x67] = &OpcodeHandler::XRL_at_R1_67;
  m_jpTable[0x68] = &OpcodeHandler::XRL_R0_68;
  m_jpTable[0x69] = &OpcodeHandler::XRL_R1_69;
  m_jpTable[0x6A] = &OpcodeHandler::XRL_R2_6A;
  m_jpTable[0x6B] = &OpcodeHandler::XRL_R3_6B;
  m_jpTable[0x6C] = &OpcodeHandler::XRL_R4_6C;
  m_jpTable[0x6D] = &OpcodeHandler::XRL_R5_6D;
  m_jpTable[0x6E] = &OpcodeHandler::XRL_R6_6E;
  m_jpTable[0x6F] = &OpcodeHandler::XRL_R7_6F;
  m_jpTable[0x70] = &OpcodeHandler::JNZ;
  m_jpTable[0x71] = &OpcodeHandler::ACALL_71;
  m_jpTable[0x72] = &OpcodeHandler::ORL_C_bit_addr_72;
  m_jpTable[0x73] = &OpcodeHandler::JMP_A_DPTR;
  m_jpTable[0x74] = &OpcodeHandler::MOV_data_74;
  m_jpTable[0x75] = &OpcodeHandler::MOV_data_addr_75;
  m_jpTable[0x76] = &OpcodeHandler::MOV_at_R0_76;
  m_jpTable[0x77] = &OpcodeHandler::MOV_at_R1_77;
  m_jpTable[0x78] = &OpcodeHandler::MOV_R0_78;
  m_jpTable[0x79] = &OpcodeHandler::MOV_R1_79;
  m_jpTable[0x7A] = &OpcodeHandler::MOV_R2_7A;
  m_jpTable[0x7B] = &OpcodeHandler::MOV_R3_7B;
  m_jpTable[0x7C] = &OpcodeHandler::MOV_R4_7C;
  m_jpTable[0x7D] = &OpcodeHandler::MOV_R5_7D;
  m_jpTable[0x7E] = &OpcodeHandler::MOV_R6_7E;
  m_jpTable[0x7F] = &OpcodeHandler::MOV_R7_7F;
  m_jpTable[0x80] = &OpcodeHandler::SJMP;
  m_jpTable[0x81] = &OpcodeHandler::AJMP_81;
  m_jpTable[0x82] = &OpcodeHandler::ANL_C_bit_addr_82;
  m_jpTable[0x83] = &OpcodeHandler::MOVC_A_PC_83;
  m_jpTable[0x84] = &OpcodeHandler::DIV;
  m_jpTable[0x85] = &OpcodeHandler::MOV_data_addr_data_addr_85;
  m_jpTable[0x86] = &OpcodeHandler::MOV_data_addr_at_R0_86;
  m_jpTable[0x87] = &OpcodeHandler::MOV_data_addr_at_R1_87;
  m_jpTable[0x88] = &OpcodeHandler::MOV_data_addr_R0_88;
  m_jpTable[0x89] = &OpcodeHandler::MOV_data_addr_R1_89;
  m_jpTable[0x8A] = &OpcodeHandler::MOV_data_addr_R2_8A;
  m_jpTable[0x8B] = &OpcodeHandler::MOV_data_addr_R3_8B;
  m_jpTable[0x8C] = &OpcodeHandler::MOV_data_addr_R4_8C;
  m_jpTable[0x8D] = &OpcodeHandler::MOV_data_addr_R5_8D;
  m_jpTable[0x8E] = &OpcodeHandler::MOV_data_addr_R6_8E;
  m_jpTable[0x8F] = &OpcodeHandler::MOV_data_addr_R7_8F;
  m_jpTable[0x90] = &OpcodeHandler::MOV_DPTR;
  m_jpTable[0x91] = &OpcodeHandler::ACALL_91;
  m_jpTable[0x92] = &OpcodeHandler::MOV_from_C;
  m_jpTable[0x93] = &OpcodeHandler::MOVC_A_DPTR_93;
  m_jpTable[0x94] = &OpcodeHandler::SUBB_data_94;
  m_jpTable[0x95] = &OpcodeHandler::SUBB_data_addr_95;
  m_jpTable[0x96] = &OpcodeHandler::SUBB_at_R0_96;
  m_jpTable[0x97] = &OpcodeHandler::SUBB_at_R1_97;
  m_jpTable[0x98] = &OpcodeHandler::SUBB_R0_98;
  m_jpTable[0x99] = &OpcodeHandler::SUBB_R1_99;
  m_jpTable[0x9A] = &OpcodeHandler::SUBB_R2_9A;
  m_jpTable[0x9B] = &OpcodeHandler::SUBB_R3_9B;
  m_jpTable[0x9C] = &OpcodeHandler::SUBB_R4_9C;
  m_jpTable[0x9D] = &OpcodeHandler::SUBB_R5_9D;
  m_jpTable[0x9E] = &OpcodeHandler::SUBB_R6_9E;
  m_jpTable[0x9F] = &OpcodeHandler::SUBB_R7_9F;
  m_jpTable[0xA0] = &OpcodeHandler::ORL_C_bit_addr_A0;
  m_jpTable[0xA1] = &OpcodeHandler::AJMP_A1;
  m_jpTable[0xA2] = &OpcodeHandler::MOV_to_C;
  m_jpTable[0xA3] = &OpcodeHandler::INC_DPTR;
  m_jpTable[0xA4] = &OpcodeHandler::MUL;
  m_jpTable[0xA5] = &OpcodeHandler::RESERVED_OPCODE;
  m_jpTable[0xA6] = &OpcodeHandler::MOV_at_R0_data_addr_A6;
  m_jpTable[0xA7] = &OpcodeHandler::MOV_at_R1_data_addr_A7;
  m_jpTable[0xA8] = &OpcodeHandler::MOV_R0_data_addr_A8;
  m_jpTable[0xA9] = &OpcodeHandler::MOV_R1_data_addr_A9;
  m_jpTable[0xAA] = &OpcodeHandler::MOV_R2_data_addr_AA;
  m_jpTable[0xAB] = &OpcodeHandler::MOV_R3_data_addr_AB;
  m_jpTable[0xAC] = &OpcodeHandler::MOV_R4_data_addr_AC;
  m_jpTable[0xAD] = &OpcodeHandler::MOV_R5_data_addr_AD;
  m_jpTable[0xAE] = &OpcodeHandler::MOV_R6_data_addr_AE;
  m_jpTable[0xAF] = &OpcodeHandler::MOV_R7_data_addr_AF;
  m_jpTable[0xB0] = &OpcodeHandler::ANL_C_bit_addr_B0;
  m_jpTable[0xB1] = &OpcodeHandler::ACALL_B1;
  m_jpTable[0xB2] = &OpcodeHandler::CPL_bit;
  m_jpTable[0xB3] = &OpcodeHandler::CPL_C;
  m_jpTable[0xB4] = &OpcodeHandler::CJNE_A_B4;
  m_jpTable[0xB5] = &OpcodeHandler::CJNE_A_data_addr_B5;
  m_jpTable[0xB6] = &OpcodeHandler::CJNE_at_R0_B6;
  m_jpTable[0xB7] = &OpcodeHandler::CJNE_at_R1_B7;
  m_jpTable[0xB8] = &OpcodeHandler::CJNE_R0_B8;
  m_jpTable[0xB9] = &OpcodeHandler::CJNE_R1_B9;
  m_jpTable[0xBA] = &OpcodeHandler::CJNE_R2_BA;
  m_jpTable[0xBB] = &OpcodeHandler::CJNE_R3_BB;
  m_jpTable[0xBC] = &OpcodeHandler::CJNE_R4_BC;
  m_jpTable[0xBD] = &OpcodeHandler::CJNE_R5_BD;
  m_jpTable[0xBE] = &OpcodeHandler::CJNE_R6_BE;
  m_jpTable[0xBF] = &OpcodeHandler::CJNE_R7_BF;
  m_jpTable[0xC0] = &OpcodeHandler::PUSH;
  m_jpTable[0xC1] = &OpcodeHandler::AJMP_C1;
  m_jpTable[0xC2] = &OpcodeHandler::CLR_bit;
  m_jpTable[0xC3] = &OpcodeHandler::CLR_C;
  m_jpTable[0xC4] = &OpcodeHandler::SWAP;
  m_jpTable[0xC5] = &OpcodeHandler::XCH_data_addr_C5;
  m_jpTable[0xC6] = &OpcodeHandler::XCH_at_R0_C6;
  m_jpTable[0xC7] = &OpcodeHandler::XCH_at_R1_C7;
  m_jpTable[0xC8] = &OpcodeHandler::XCH_R0_C8;
  m_jpTable[0xC9] = &OpcodeHandler::XCH_R1_C9;
  m_jpTable[0xCA] = &OpcodeHandler::XCH_R2_CA;
  m_jpTable[0xCB] = &OpcodeHandler::XCH_R3_CB;
  m_jpTable[0xCC] = &OpcodeHandler::XCH_R4_CC;
  m_jpTable[0xCD] = &OpcodeHandler::XCH_R5_CD;
  m_jpTable[0xCE] = &OpcodeHandler::XCH_R6_CE;
  m_jpTable[0xCF] = &OpcodeHandler::XCH_R7_CF;
  m_jpTable[0xD0] = &OpcodeHandler::POP;
  m_jpTable[0xD1] = &OpcodeHandler::ACALL_D1;
  m_jpTable[0xD2] = &OpcodeHandler::SETB_bit;
  m_jpTable[0xD3] = &OpcodeHandler::SETB_C;
  m_jpTable[0xD4] = &OpcodeHandler::DA;
  m_jpTable[0xD5] = &OpcodeHandler::DJNZ_data_addr_D5;
  m_jpTable[0xD6] = &OpcodeHandler::XCHD_at_R0_D6;
  m_jpTable[0xD7] = &OpcodeHandler::XCHD_at_R1_D7;
  m_jpTable[0xD8] = &OpcodeHandler::DJNZ_R0_D8;
  m_jpTable[0xD9] = &OpcodeHandler::DJNZ_R1_D9;
  m_jpTable[0xDA] = &OpcodeHandler::DJNZ_R2_DA;
  m_jpTable[0xDB] = &OpcodeHandler::DJNZ_R3_DB;
  m_jpTable[0xDC] = &OpcodeHandler::DJNZ_R4_DC;
  m_jpTable[0xDD] = &OpcodeHandler::DJNZ_R5_DD;
  m_jpTable[0xDE] = &OpcodeHandler::DJNZ_R6_DE;
  m_jpTable[0xDF] = &OpcodeHandler::DJNZ_R7_DF;
  m_jpTable[0xE0] = &OpcodeHandler::MOVX_A_DPTR_E0;
  m_jpTable[0xE1] = &OpcodeHandler::AJMP_E1;
  m_jpTable[0xE2] = &OpcodeHandler::MOVX_A_at_R0_E2;
  m_jpTable[0xE3] = &OpcodeHandler::MOVX_A_at_R1_E3;
  m_jpTable[0xE4] = &OpcodeHandler::CLR_A;
  m_jpTable[0xE5] = &OpcodeHandler::MOV_A_data_addr_E5;
  m_jpTable[0xE6] = &OpcodeHandler::MOV_A_at_R0_E6;
  m_jpTable[0xE7] = &OpcodeHandler::MOV_A_at_R1_E7;
  m_jpTable[0xE8] = &OpcodeHandler::MOV_A_R0_E8;
  m_jpTable[0xE9] = &OpcodeHandler::MOV_A_R1_E9;
  m_jpTable[0xEA] = &OpcodeHandler::MOV_A_R2_EA;
  m_jpTable[0xEB] = &OpcodeHandler::MOV_A_R3_EB;
  m_jpTable[0xEC] = &OpcodeHandler::MOV_A_R4_EC;
  m_jpTable[0xED] = &OpcodeHandler::MOV_A_R5_ED;
  m_jpTable[0xEE] = &OpcodeHandler::MOV_A_R6_EE;
  m_jpTable[0xEF] = &OpcodeHandler::MOV_A_R7_EF;
  m_jpTable[0xF0] = &OpcodeHandler::MOVX_DPTR_A_F0;
  m_jpTable[0xF1] = &OpcodeHandler::ACALL_F1;
  m_jpTable[0xF2] = &OpcodeHandler::MOVX_at_R0_A_F2;
  m_jpTable[0xF3] = &OpcodeHandler::MOVX_at_R1_A_F3;
  m_jpTable[0xF4] = &OpcodeHandler::CPL_A;
  m_jpTable[0xF5] = &OpcodeHandler::MOV_data_addr_A_F5;
  m_jpTable[0xF6] = &OpcodeHandler::MOV_at_R0_A_F6;
  m_jpTable[0xF7] = &OpcodeHandler::MOV_at_R1_A_F7;
  m_jpTable[0xF8] = &OpcodeHandler::MOV_R0_A_F8;
  m_jpTable[0xF9] = &OpcodeHandler::MOV_R1_A_F9;
  m_jpTable[0xFA] = &OpcodeHandler::MOV_R2_A_FA;
  m_jpTable[0xFB] = &OpcodeHandler::MOV_R3_A_FB;
  m_jpTable[0xFC] = &OpcodeHandler::MOV_R4_A_FC;
  m_jpTable[0xFD] = &OpcodeHandler::MOV_R5_A_FD;
  m_jpTable[0xFE] = &OpcodeHandler::MOV_R6_A_FE;
  m_jpTable[0xFF] = &OpcodeHandler::MOV_R7_A_FF;
}