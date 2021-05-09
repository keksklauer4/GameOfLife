# Yet another selfmade 8051 Emulator

For this project, we decided that we need another emulator because emulating a millisecond using the integrated emulator in the IDE took around a second which was not acceptable. Therefore, we wrote our own not particularly sopisticated emulator (obviously no dynarec because x86 is simply disgusting!). The interpreter is fast enough for realtime emulation, can easily be customized (one can add additional logic through emulating memory-mapped registers using a callback) and emulates the ```PCON``` register (yes, you can use ```IDLE mode``` for power saving!).

For technical details of the inner workings of the 8051 and a great overview and explanation of registers and instruction set, see [the originial Intel User's Manual](https://web.mit.edu/6.115/www/document/8051.pdf).

## Features
Currently implemented features are:
- All opcodes implemented (except for decimal adjust ```DA``` but that will change)
- Cycle accurate - at least somewhat... The interpreter emulates on a instruction basis (no microinstructions or such lunacy)
- ```Timer 0``` and ```Timer 1``` are emulated (```modes 0 to 3```)
- ```External Interrupt 0``` and ```External Interrupt 1``` can be triggered.
- Configuration options: ```Debug```, ```Timer 0```, ```Timer 1``` (decide whether Timers 0 and 1 should be emulated), ```Exception``` (how to handle unexpected states?)
- Custom callback can be specified in the form of a ```std::function<void(state_t&)>```. The callback receives a reference on the state_t struct of the emulator. ```state_t``` contains all registers, memory locations and a ```cycles_passed``` variable that indicates how many instruction cycles have been passed.

## To be implemented
- Visualizations
- ```Reset``` functionality
- ```Interrupt priorization```
- Maybe ```serial communication```
- Port usage (when using MOVX @DPTR, ```Port 2``` is used to page external memory)
- Alternativ functions for ```GCC``` builtins
- Examples

## Notes
- Timer register contents will be updated after each instruction executed.
- Don't be mad at me for extensively making use of C macros... They enabled me to stay as efficient as possible while minizing function calls. I did not want to implement a dynarec but still wanted the Interpreter to be as fast as possible. Therefore, I had to rely on the compiler optimizing as much as possible.
