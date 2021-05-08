#ifndef EMU_CONFIG_HPP
#define EMU_CONFIG_HPP


// configure these macros. 0
#define __DEBUG__ 1
#define __ENABLE_TIMER_0__ 1
#define __ENABLE_TIMER_1__ 1
#define __ENABLE_INTERRUPTS__ 1





#if __ENABLE_TIMER_0__ == 1
#define TIMER0(content) content
#else
#define TIMER0(content)
#endif

#if __ENABLE_TIMER_1__ == 1
#define TIMER1(content) content
#else
#define TIMER1(content)
#endif


#if __ENABLE_INTERRUPTS__ == 1
#define INTERRUPT(content) content
#else
#define INTERRUPT(content)
#endif

// compiler/architecture specifics
#define POPCOUNT(val) __builtin_popcount(val)
#define BSWAP16(v) __builtin_bswap16(v)

#endif