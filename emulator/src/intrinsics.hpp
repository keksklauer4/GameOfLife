#ifndef EMU_INTRINSICS_HPP
#define EMU_INTRINSICS_HPP

#include "config.hpp"
#include "types.hpp"

#if __GCC_X86__ == 1
// compiler/architecture specifics
#define POPCOUNT(val) __builtin_popcount(val)
#define BSWAP16(v) __builtin_bswap16(v)

#else
// we need popcount only for 8 bit values anyway
static emu::fuint32_t popcount_table[16] = {
    0, // 0000
    1, // 0001
    1, // 0010
    2, // 0011
    1, // 0100
    2, // 0101
    2, // 0110
    3, // 0111
    1, // 1000
    2, // 1001
    2, // 1010
    3, // 1011
    2, // 1100
    3, // 1101
    3, // 1110
    4  // 1111
};

emu::fuint32_t popcount(uint8_t v){
    return popcount_table[v & 0x0F] + popcount_table[(v >> 4) & 0x0F];
}
uint16_t bswap16_impl(uint16_t v){
    return (v << 8) | (v >> 8);
}

#define POPCOUNT(val) popcount(val)
#define BSWAP16(v) bswap16_impl(v)
#endif

#endif
