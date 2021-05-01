#ifndef EMU_OPCODE_HANDLER_HPP
#define EMU_OPCODE_HANDLER_HPP

#include "types.hpp"
#include "emulator.hpp"
#include "opcode_parse_helpers.hpp"
#include "debug_opcode_helpers.hpp"

#include <iostream>

namespace emu
{

  class OpcodeHandler;

  typedef void (OpcodeHandler::*jp_table_entry_t)();

  class OpcodeHandler
  {
    public:
      OpcodeHandler(state_t& state): m_state(state){ init(); }

      void execOpcode();

    private:
      void init();

    private:
      uint8_t m_opcode;
      state_t& m_state;
      jp_table_entry_t m_jpTable[256];

    private: // macro functions
      void NOP();
      void ACALL(uint16_t upper_bits);
      void ADD(uint8_t value);
      void ADDC(uint8_t value);
      void AJMP(uint16_t upper_bits);
      void ANL_BYTE(uint8_t* dest, uint8_t value);
      void ANL_C(bool cpl);
      void CJNE(uint8_t value, uint8_t comp);
      void CLR_A();
      void CLR_bit();
      void CLR_C();
      void CPL_A();
      void CPL_bit();
      void CPL_C();
      void DA();
      void DEC(uint8_t* address);
      void DIV();
      void DJNZ(uint8_t* address);
      void INC(uint8_t* address);
      void INC_DPTR();
      void JB();
      void JBC();
      void JC();
      void JMP_A_DPTR();
      void JNB();
      void JNC();
      void JNZ();
      void JZ();
      void LCALL();
      void LJMP();
      void MOV(uint8_t* dest, uint8_t val);
      void MOV_to_C();
      void MOV_from_C();
      void MOV_DPTR();
      void MOVC_IND_16(uint16_t* half_ptr);
      void MOVX(uint8_t* dest, uint8_t* src);
      void MUL();
      void ORL_BYTE(uint8_t* dest, uint8_t data);
      void ORL_C(bool cpl);
      void POP();
      void PUSH();
      void RET();
      void RETI();
      void RL();
      void RLC();
      void RR();
      void RRC();
      void SETB_C();
      void SETB_bit();
      void SJMP();
      void SUBB(uint8_t val);
      void SWAP();
      void XCH(uint8_t* address);
      void XCHD(uint8_t* address);
      void XRL(uint8_t* dest, uint8_t data);

      // actual jump table entries

      void AJMP_01() { AJMP(AJMP_BITS(0b000)); }
      void AJMP_21() { AJMP(AJMP_BITS(0b001)); }
      void AJMP_41() { AJMP(AJMP_BITS(0b010)); }
      void AJMP_61() { AJMP(AJMP_BITS(0b011)); }
      void AJMP_81() { AJMP(AJMP_BITS(0b100)); }
      void AJMP_A1() { AJMP(AJMP_BITS(0b101)); }
      void AJMP_C1() { AJMP(AJMP_BITS(0b110)); }
      void AJMP_E1() { AJMP(AJMP_BITS(0b111)); }

      void INC_A_04() { INC(A_REG); }
      void INC_data_addr_05() { RD_ADDRESS() INC(GET_DATA_ADDRESS()); }
      void INC_at_R0_06() { INC(GET_REG_IND(0)); }
      void INC_at_R1_07() { INC(GET_REG_IND(1)); }
      void INC_R0_08() { INC(GET_REG(0)); }
      void INC_R1_09() { INC(GET_REG(1)); }
      void INC_R2_0A() { INC(GET_REG(2)); }
      void INC_R3_0B() { INC(GET_REG(3)); }
      void INC_R4_0C() { INC(GET_REG(4)); }
      void INC_R5_0D() { INC(GET_REG(5)); }
      void INC_R6_0E() { INC(GET_REG(6)); }
      void INC_R7_0F() { INC(GET_REG(7)); }

      void DEC_A_14() { DEC(A_REG); }
      void DEC_data_addr_15() { RD_ADDRESS() DEC(GET_DATA_ADDRESS()); }
      void DEC_at_R0_16() { DEC(GET_REG_IND(0)); }
      void DEC_at_R1_17() { DEC(GET_REG_IND(1)); }
      void DEC_R0_18() { DEC(GET_REG(0)); }
      void DEC_R1_19() { DEC(GET_REG(1)); }
      void DEC_R2_1A() { DEC(GET_REG(2)); }
      void DEC_R3_1B() { DEC(GET_REG(3)); }
      void DEC_R4_1C() { DEC(GET_REG(4)); }
      void DEC_R5_1D() { DEC(GET_REG(5)); }
      void DEC_R6_1E() { DEC(GET_REG(6)); }
      void DEC_R7_1F() { DEC(GET_REG(7)); }

      void ACALL_11() { ACALL(AJMP_BITS(0b000)); }
      void ACALL_31() { ACALL(AJMP_BITS(0b001)); }
      void ACALL_51() { ACALL(AJMP_BITS(0b010)); }
      void ACALL_71() { ACALL(AJMP_BITS(0b011)); }
      void ACALL_91() { ACALL(AJMP_BITS(0b100)); }
      void ACALL_B1() { ACALL(AJMP_BITS(0b101)); }
      void ACALL_D1() { ACALL(AJMP_BITS(0b110)); }
      void ACALL_F1() { ACALL(AJMP_BITS(0b111)); }

      void ADD_data_24() { ADD(IMMEDIATE()); }
      void ADD_data_addr_25() { RD_ADDRESS() ADD(*GET_DATA_ADDRESS()); }
      void ADD_at_R0_26() { ADD(*GET_REG_IND(0)); }
      void ADD_at_R1_27() { ADD(*GET_REG_IND(1)); }
      void ADD_R0_28() { ADD(*GET_REG(0)); }
      void ADD_R1_29() { ADD(*GET_REG(1)); }
      void ADD_R2_2A() { ADD(*GET_REG(2)); }
      void ADD_R3_2B() { ADD(*GET_REG(3)); }
      void ADD_R4_2C() { ADD(*GET_REG(4)); }
      void ADD_R5_2D() { ADD(*GET_REG(5)); }
      void ADD_R6_2E() { ADD(*GET_REG(6)); }
      void ADD_R7_2F() { ADD(*GET_REG(7)); }


      void ADDC_data_34() { ADDC(IMMEDIATE()); }
      void ADDC_data_addr_35() { RD_ADDRESS() ADDC(*GET_DATA_ADDRESS()); }
      void ADDC_at_R0_36() { ADDC(*GET_REG_IND(0)); }
      void ADDC_at_R1_37() { ADDC(*GET_REG_IND(1)); }
      void ADDC_R0_38() { ADDC(*GET_REG(0)); }
      void ADDC_R1_39() { ADDC(*GET_REG(1)); }
      void ADDC_R2_3A() { ADDC(*GET_REG(2)); }
      void ADDC_R3_3B() { ADDC(*GET_REG(3)); }
      void ADDC_R4_3C() { ADDC(*GET_REG(4)); }
      void ADDC_R5_3D() { ADDC(*GET_REG(5)); }
      void ADDC_R6_3E() { ADDC(*GET_REG(6)); }
      void ADDC_R7_3F() { ADDC(*GET_REG(7)); }


      void ORL_data_addr_A_42() { RD_ADDRESS() ORL_BYTE(GET_DATA_ADDRESS(), *A_REG); }
      void ORL_data_addr_data_43() { RD_ADDRESS() ORL_BYTE(GET_DATA_ADDRESS(), IMMEDIATE()); }
      void ORL_data_44() { ORL_BYTE(A_REG, IMMEDIATE()); }
      void ORL_data_addr_45() { ORL_BYTE(A_REG, *READ_ADDRESS_1B()); }
      void ORL_at_R0_46() { ORL_BYTE(A_REG, *GET_REG_IND(0)); }
      void ORL_at_R1_47() { ORL_BYTE(A_REG, *GET_REG_IND(1)); }
      void ORL_R0_48() { ORL_BYTE(A_REG, *GET_REG(0)); }
      void ORL_R1_49() { ORL_BYTE(A_REG, *GET_REG(1)); }
      void ORL_R2_4A() { ORL_BYTE(A_REG, *GET_REG(2)); }
      void ORL_R3_4B() { ORL_BYTE(A_REG, *GET_REG(3)); }
      void ORL_R4_4C() { ORL_BYTE(A_REG, *GET_REG(4)); }
      void ORL_R5_4D() { ORL_BYTE(A_REG, *GET_REG(5)); }
      void ORL_R6_4E() { ORL_BYTE(A_REG, *GET_REG(6)); }
      void ORL_R7_4F() { ORL_BYTE(A_REG, *GET_REG(7)); }
      void ORL_C_bit_addr_72() { ORL_C(false); }
      void ORL_C_bit_addr_A0() { ORL_C(true); }


      void ANL_data_addr_A_52() { RD_ADDRESS() ANL_BYTE(GET_DATA_ADDRESS(), *A_REG); }
      void ANL_data_addr_data_53() { RD_ADDRESS() ANL_BYTE(GET_DATA_ADDRESS(), IMMEDIATE()); }
      void ANL_data_54() { ANL_BYTE(A_REG, IMMEDIATE()); }
      void ANL_data_addr_55() { ANL_BYTE(A_REG, *READ_ADDRESS_1B()); }
      void ANL_at_R0_56() { ANL_BYTE(A_REG, *GET_REG_IND(0)); }
      void ANL_at_R1_57() { ANL_BYTE(A_REG, *GET_REG_IND(1)); }
      void ANL_R0_58() { ANL_BYTE(A_REG, *GET_REG(0)); }
      void ANL_R1_59() { ANL_BYTE(A_REG, *GET_REG(1)); }
      void ANL_R2_5A() { ANL_BYTE(A_REG, *GET_REG(2)); }
      void ANL_R3_5B() { ANL_BYTE(A_REG, *GET_REG(3)); }
      void ANL_R4_5C() { ANL_BYTE(A_REG, *GET_REG(4)); }
      void ANL_R5_5D() { ANL_BYTE(A_REG, *GET_REG(5)); }
      void ANL_R6_5E() { ANL_BYTE(A_REG, *GET_REG(6)); }
      void ANL_R7_5F() { ANL_BYTE(A_REG, *GET_REG(7)); }
      void ANL_C_bit_addr_82() { ANL_C(false); }
      void ANL_C_bit_addr_B0() { ANL_C(true); }


      void XRL_data_addr_A_62() { RD_ADDRESS() XRL(GET_DATA_ADDRESS(), *A_REG); }
      void XRL_data_addr_data_63() { RD_ADDRESS() XRL(GET_DATA_ADDRESS(), IMMEDIATE()); }
      void XRL_data_64() { XRL(A_REG, IMMEDIATE()); }
      void XRL_data_addr_65() { XRL(A_REG, *READ_ADDRESS_1B()); }
      void XRL_at_R0_66() { XRL(A_REG, *GET_REG_IND(0)); }
      void XRL_at_R1_67() { XRL(A_REG, *GET_REG_IND(1)); }
      void XRL_R0_68() { XRL(A_REG, *GET_REG(0)); }
      void XRL_R1_69() { XRL(A_REG, *GET_REG(1)); }
      void XRL_R2_6A() { XRL(A_REG, *GET_REG(2)); }
      void XRL_R3_6B() { XRL(A_REG, *GET_REG(3)); }
      void XRL_R4_6C() { XRL(A_REG, *GET_REG(4)); }
      void XRL_R5_6D() { XRL(A_REG, *GET_REG(5)); }
      void XRL_R6_6E() { XRL(A_REG, *GET_REG(6)); }
      void XRL_R7_6F() { XRL(A_REG, *GET_REG(7)); }


      void SUBB_data_94() { SUBB(IMMEDIATE()); }
      void SUBB_data_addr_95() { RD_ADDRESS() SUBB(*GET_DATA_ADDRESS()); }
      void SUBB_at_R0_96() { SUBB(*GET_REG_IND(0)); }
      void SUBB_at_R1_97() { SUBB(*GET_REG_IND(1)); }
      void SUBB_R0_98() { SUBB(*GET_REG(0)); }
      void SUBB_R1_99() { SUBB(*GET_REG(1)); }
      void SUBB_R2_9A() { SUBB(*GET_REG(2)); }
      void SUBB_R3_9B() { SUBB(*GET_REG(3)); }
      void SUBB_R4_9C() { SUBB(*GET_REG(4)); }
      void SUBB_R5_9D() { SUBB(*GET_REG(5)); }
      void SUBB_R6_9E() { SUBB(*GET_REG(6)); }
      void SUBB_R7_9F() { SUBB(*GET_REG(7)); }


      void CJNE_A_B4() { CJNE(*A_REG, IMMEDIATE()); }
      void CJNE_A_data_addr_B5() { RD_ADDRESS() CJNE(*A_REG, *GET_DATA_ADDRESS()); }
      void CJNE_at_R0_B6() { CJNE(*GET_REG_IND(0), IMMEDIATE()); }
      void CJNE_at_R1_B7() { CJNE(*GET_REG_IND(1), IMMEDIATE()); }
      void CJNE_R0_B8() { CJNE(*GET_REG(0), IMMEDIATE()); }
      void CJNE_R1_B9() { CJNE(*GET_REG(1), IMMEDIATE()); }
      void CJNE_R2_BA() { CJNE(*GET_REG(2), IMMEDIATE()); }
      void CJNE_R3_BB() { CJNE(*GET_REG(3), IMMEDIATE()); }
      void CJNE_R4_BC() { CJNE(*GET_REG(4), IMMEDIATE()); }
      void CJNE_R5_BD() { CJNE(*GET_REG(5), IMMEDIATE()); }
      void CJNE_R6_BE() { CJNE(*GET_REG(6), IMMEDIATE()); }
      void CJNE_R7_BF() { CJNE(*GET_REG(7), IMMEDIATE()); }


      void XCH_data_addr_C5() { RD_ADDRESS() XCH(GET_DATA_ADDRESS()); }
      void XCH_at_R0_C6() { XCH(GET_REG_IND(0)); }
      void XCH_at_R1_C7() { XCH(GET_REG_IND(1)); }
      void XCH_R0_C8() { XCH(GET_REG(0)); }
      void XCH_R1_C9() { XCH(GET_REG(1)); }
      void XCH_R2_CA() { XCH(GET_REG(2)); }
      void XCH_R3_CB() { XCH(GET_REG(3)); }
      void XCH_R4_CC() { XCH(GET_REG(4)); }
      void XCH_R5_CD() { XCH(GET_REG(5)); }
      void XCH_R6_CE() { XCH(GET_REG(6)); }
      void XCH_R7_CF() { XCH(GET_REG(7)); }


      void XCHD_at_R0_D6() { XCHD(GET_REG_IND(0)); }
      void XCHD_at_R1_D7() { XCHD(GET_REG_IND(1)); }


      void DJNZ_data_addr_D5() { RD_ADDRESS() DJNZ(GET_DATA_ADDRESS()); }
      void DJNZ_R0_D8() { DJNZ(GET_REG(0)); }
      void DJNZ_R1_D9() { DJNZ(GET_REG(1)); }
      void DJNZ_R2_DA() { DJNZ(GET_REG(2)); }
      void DJNZ_R3_DB() { DJNZ(GET_REG(3)); }
      void DJNZ_R4_DC() { DJNZ(GET_REG(4)); }
      void DJNZ_R5_DD() { DJNZ(GET_REG(5)); }
      void DJNZ_R6_DE() { DJNZ(GET_REG(6)); }
      void DJNZ_R7_DF() { DJNZ(GET_REG(7)); }


      void MOV_data_74() { MOV(A_REG, IMMEDIATE()); }
      void MOV_data_addr_75() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), IMMEDIATE()); }
      void MOV_at_R0_76() { MOV(GET_REG_IND(0), IMMEDIATE()); }
      void MOV_at_R1_77() { MOV(GET_REG_IND(1), IMMEDIATE()); }
      void MOV_R0_78() { MOV(GET_REG(0), IMMEDIATE()); }
      void MOV_R1_79() { MOV(GET_REG(1), IMMEDIATE()); }
      void MOV_R2_7A() { MOV(GET_REG(2), IMMEDIATE()); }
      void MOV_R3_7B() { MOV(GET_REG(3), IMMEDIATE()); }
      void MOV_R4_7C() { MOV(GET_REG(4), IMMEDIATE()); }
      void MOV_R5_7D() { MOV(GET_REG(5), IMMEDIATE()); }
      void MOV_R6_7E() { MOV(GET_REG(6), IMMEDIATE()); }
      void MOV_R7_7F() { MOV(GET_REG(7), IMMEDIATE()); }


      void MOV_data_addr_data_addr_85() { RD_ADDRESSES() MOV(GET_DATA_ADDRESS(), *GET_DATA_ADDRESS_SEC()); }
      void MOV_data_addr_at_R0_86() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG_IND(0)); }
      void MOV_data_addr_at_R1_87() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG_IND(1)); }
      void MOV_data_addr_R0_88() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(0)); }
      void MOV_data_addr_R1_89() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(1)); }
      void MOV_data_addr_R2_8A() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(2)); }
      void MOV_data_addr_R3_8B() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(3)); }
      void MOV_data_addr_R4_8C() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(4)); }
      void MOV_data_addr_R5_8D() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(5)); }
      void MOV_data_addr_R6_8E() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(6)); }
      void MOV_data_addr_R7_8F() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *GET_REG(7)); }

      void MOVC_A_PC_83() { MOVC_IND_16((uint16_t*)PC_MEM_LOC); }
      void MOVC_A_DPTR_93() { MOVC_IND_16(DPTR_REG); }

      void MOV_at_R0_data_addr_A6() { RD_ADDRESSES() MOV(GET_REG_IND(0), *GET_DATA_ADDRESS()); }
      void MOV_at_R1_data_addr_A7() { RD_ADDRESSES() MOV(GET_REG_IND(1), *GET_DATA_ADDRESS()); }
      void MOV_R0_data_addr_A8() { RD_ADDRESS() MOV(GET_REG(0), *GET_DATA_ADDRESS()); }
      void MOV_R1_data_addr_A9() { RD_ADDRESS() MOV(GET_REG(1), *GET_DATA_ADDRESS()); }
      void MOV_R2_data_addr_AA() { RD_ADDRESS() MOV(GET_REG(2), *GET_DATA_ADDRESS()); }
      void MOV_R3_data_addr_AB() { RD_ADDRESS() MOV(GET_REG(3), *GET_DATA_ADDRESS()); }
      void MOV_R4_data_addr_AC() { RD_ADDRESS() MOV(GET_REG(4), *GET_DATA_ADDRESS()); }
      void MOV_R5_data_addr_AD() { RD_ADDRESS() MOV(GET_REG(5), *GET_DATA_ADDRESS()); }
      void MOV_R6_data_addr_AE() { RD_ADDRESS() MOV(GET_REG(6), *GET_DATA_ADDRESS()); }
      void MOV_R7_data_addr_AF() { RD_ADDRESS() MOV(GET_REG(7), *GET_DATA_ADDRESS()); }


      void MOV_A_data_addr_E5() { RD_ADDRESS() MOV(A_REG, *GET_DATA_ADDRESS()); }
      void MOV_A_at_R0_E6() { MOV(A_REG, *GET_REG_IND(0)); }
      void MOV_A_at_R1_E7() { MOV(A_REG, *GET_REG_IND(1)); }
      void MOV_A_R0_E8() { MOV(A_REG, *GET_REG(0)); }
      void MOV_A_R1_E9() { MOV(A_REG, *GET_REG(1)); }
      void MOV_A_R2_EA() { MOV(A_REG, *GET_REG(2)); }
      void MOV_A_R3_EB() { MOV(A_REG, *GET_REG(3)); }
      void MOV_A_R4_EC() { MOV(A_REG, *GET_REG(4)); }
      void MOV_A_R5_ED() { MOV(A_REG, *GET_REG(5)); }
      void MOV_A_R6_EE() { MOV(A_REG, *GET_REG(6)); }
      void MOV_A_R7_EF() { MOV(A_REG, *GET_REG(7)); }

      void MOV_data_addr_A_F5() { RD_ADDRESS() MOV(GET_DATA_ADDRESS(), *A_REG); }
      void MOV_at_R0_A_F6() { MOV(GET_REG_IND(0), *A_REG); }
      void MOV_at_R1_A_F7() { MOV(GET_REG_IND(1), *A_REG); }
      void MOV_R0_A_F8() { MOV(GET_REG(0), *A_REG); }
      void MOV_R1_A_F9() { MOV(GET_REG(1), *A_REG); }
      void MOV_R2_A_FA() { MOV(GET_REG(2), *A_REG); }
      void MOV_R3_A_FB() { MOV(GET_REG(3), *A_REG); }
      void MOV_R4_A_FC() { MOV(GET_REG(4), *A_REG); }
      void MOV_R5_A_FD() { MOV(GET_REG(5), *A_REG); }
      void MOV_R6_A_FE() { MOV(GET_REG(6), *A_REG); }
      void MOV_R7_A_FF() { MOV(GET_REG(7), *A_REG); }

      void MOVX_A_DPTR_E0() { MOVX(A_REG, DPTR_EXTERNAL()); }
      void MOVX_A_at_R0_E2() { MOVX(A_REG, GET_REG_IND_EXT(0)); }
      void MOVX_A_at_R1_E3() { MOVX(A_REG, GET_REG_IND_EXT(1)); }

      void MOVX_DPTR_A_F0() { MOVX(DPTR_EXTERNAL(), A_REG); }
      void MOVX_at_R0_A_F2() { MOVX(GET_REG_IND_EXT(0), A_REG); }
      void MOVX_at_R1_A_F3() { MOVX(GET_REG_IND_EXT(1), A_REG); }
  };
}


#endif