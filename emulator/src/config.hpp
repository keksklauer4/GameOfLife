#ifndef EMU_CONFIG_HPP
#define EMU_CONFIG_HPP


// configure these macros.
#define __DEBUG__ 0
#define __ENABLE_TIMER_0__ 1
#define __ENABLE_TIMER_1__ 1
#define __ENABLE_INTERRUPTS__ 1
#define __THROW_EXCEPTION_ON_ERRONEOUS_STATE__ 1
#define __GCC_X86__ 1 // flag to determine whether compiled with gcc on x86 (because of intrinsics)




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

#if __THROW_EXCEPTION_ON_ERRONEOUS_STATE__ == 1
#define THROW_EXCEPTION(content) content
#else
#define THROW_EXCEPTION(content)
#endif


#endif