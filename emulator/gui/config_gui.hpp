#ifndef CONFIG_GUI_HPP
#define CONFIG_GUI_HPP


// ----------------------------------------------------------------
// helpers to specify addresses. If you need an address, you should use these macros
#define EXTERNAL_ADDR(offset) (state.external_memory + offset)
#define INTERNAL_DATA(offset) (state.internal_data + offset)
#define SFR_DATA(offset) (state.sfr_memory + offset)
// !Important!: Be aware that sfr memory space is located with an offset of 128. Thus, if you have
// some sfr (special function register) memory address, you have to subtract 128 from it.
// Apart from that, addressing in 8051 is somewhat difficult. Try to either use
// some specific regs (look at types.hpp in the actual emulator) or use
// external addresses. The problem is that depending on your addressing mode,
// at address 0x95 (>= 0x80) you will either be in internal data or sfr memory space.
// Therefore, just do not use sfr/internal data, if you are not sure about the actual
// inner workings.

// ----------------------------------------------------------------
// configs to handle led matrix stuff
// USE_LED_MATRIX = 1 turns on matrix
// LED_MATRIX_CONTROL_BYTE is the byte where the "draw" bit is located (default: m_state.regs.PO)
// LED_MATRIX_CONTROL_BIT is the bit position in in LED_MATRIX_CONTROL_BIT
//        --> value of 7 (default) is the bit that corresponds to value 128
#define USE_LED_MATRIX 1
#define LED_MATRIX_CONTROL_BYTE state.regs.P0
#define LED_MATRIX_CONTROL_BIT 7
#define LED_MATRIX_ROW_SELECT state.regs.P1
#define LED_MATRIX_COLUMN_SET state.regs.P3

// ----------------------------------------------------------------

// ----------------------------------------------------------------
// configs to handle 7 segment display


#endif