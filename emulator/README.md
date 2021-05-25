# Yet another selfmade 8051 Emulator

For this project, we decided that we need another emulator because emulating a millisecond using the integrated emulator in the IDE took around a second which was not acceptable. Therefore, we wrote our own not particularly sopisticated emulator (obviously no dynarec because x86 is simply disgusting!). The interpreter is fast enough for realtime emulation, can easily be customized (one can add additional logic through emulating memory-mapped registers using a callback) and emulates the ```PCON``` register (yes, you can use ```IDLE mode``` for power saving!).

For technical details of the inner workings of the 8051 and a great overview and explanation of registers and instruction set, see [the originial Intel User's Manual](https://web.mit.edu/6.115/www/document/8051.pdf).


## Why use this emulator instead of the mcu8051ide

- This emulator runs in real time (if ```__DEBUG__``` is off)
- This emulator has an LED matrix that can latch individual rows' states
- It emulates ```PCON``` (you can elegantly go into IDLE mode instead of using peasantry-like NOP loops for sleeping!)
- Because it is superior!


## Features
Currently implemented features are:
- Visualization through a multiplexed LED matrix. See below on how to use the .
- All opcodes implemented (except for decimal adjust ```DA``` but that will change)
- Cycle accurate - at least somewhat... The interpreter emulates on a instruction basis (no microinstructions or such lunacy)
- ```Timer 0``` and ```Timer 1``` are emulated (```modes 0 to 3```)
- ```External Interrupt 0``` and ```External Interrupt 1``` can be triggered.
- Configuration options: ```Debug```, ```Timer 0```, ```Timer 1``` (decide whether Timers 0 and 1 should be emulated), ```Exception``` (how to handle unexpected states?)
- Custom callback can be specified in the form of a ```std::function<void(state_t&)>```. The callback receives a reference on the state_t struct of the emulator. ```state_t``` contains all registers, memory locations and a ```cycles_passed``` variable that indicates how many instruction cycles have been passed.
- ```External reset``` can be triggered.
- ```PORT2``` being used for ```paging external memory``` when using external 16 bit addresses is emulated.
- For specifc functionality (namely ```popcount``` and ```bswap16```) both compiler intrinsics or manually implemented functionality can be used.


## To be implemented
- ```Interrupt priorization```
- Maybe ```serial communication```
- Examples


## Notes
- Timer register contents will be updated after each instruction executed.
- It is important to know that when addressing external memory with a 16 bit address, more specifically, when using ```MOVX``` with ```@DPTR```, PORT2 is used for paging the memory. Therefore, when addressing that way, one cannot rely on PORT2's values. After addressing, the value is restored.
- Don't be mad at me for extensively making use of C macros... They enabled me to stay as efficient as possible while minizing function calls. I did not want to implement a dynarec but still wanted the Interpreter to be as fast as possible. Therefore, I had to rely on the compiler optimizing as much as possible.


## How to use the LED matrix?
Easy! Instead of doing it like in the mcu8051ide, we simulate an LED matrix that latches states of individual LEDs. Therefore you can write to the LEDs and they will keep their state (obviously superior to that peasantry the ide does!).

We use ```P0.7``` as a flag to indicate that the LED matrix should latch in the values at ```P1``` and ```P3```. Thus, ```P0.7``` can be interpreted as a ```enable write flag```.

```P1``` is used for selecting the rows to write to. A value of ```0x03``` would select the upper two rows.
```P3``` is the new setting for the selected rows.

Due to the fact that I really do not like inverted logic (except for reset etc.), we do not use it. ```P0.7``` high means "draw" just as ```P3 = 0xFF``` means set all LEDs in selected rows to high.

Example:
```
CLR P0.7          ; P0.7 should be always off (if you do not want to draw something new)

...

; P0.7 should be low to prevent unwanted changes of LED states.
                  ; 0x18 = 0b00011000 in binary
MOV P1, #0x18     ; select rows 4 and 5 (those in the middle)
MOV P3, #0x18     ; select set columns 4 and 5 to high and all others to low
SETB P0.7         ; enable writing. The emulator will now set the rows
NOP               ; in practice you should add these two NOPs to make sure the data is
NOP               ; actually latched but since this is an emulation you can leave them away
CLR P0.7          ; disable writing again

...               ; You can now do with P1 and P3 whatever you want since writing is disabled.
```


## Build instructions:
Yeah, well building is usually awful. But it should not be with this project. If you think otherwise, tell me.

We used ```Qt5``` for implementing the GUI. If you're using linux, this should be no problem.
Just run
```
sudo apt install qt5-default
```
If this fails, see [here](https://wiki.qt.io/Install_Qt_5_on_Ubuntu) for regularly updated instructions on how to install Qt5 on Ubuntu.

Apart from that you need the following packages:
```
sudo apt install g++ make
```

Then, you can either build with the ```.sh``` file provided.
```
# you must be in this directory (in emulator/)
bash build_all.sh
# the executable will be in this directory and should be named "LEDMatrix"
```

If you want to configure anything, see the [config.hpp](https://github.com/keksklauer4/GameOfLife/blob/master/emulator/src/config.hpp).
If you do not use GCC or you are not on x86, you need to set ```__GCC_X86__``` to 0 (because I used x86 and GCC specific intrinsics but implemented alternative functions). Note that I did only test this on Ubuntu 20.04 on a x86 64 bit machine.

To build manually, run
```
make   # build the emulator
cd gui
mkdir build
cd build
qmake ..
make
# the executable will be placed in emulator/gui/build and will be named "LEDMatrix".
```


### How to run the GUI?
If you have built the project, just navigate to the executable then type
```
./LEDMatrix "<path to your bin file>"
```

If you do not have a bin file (since the mcu8051ide does only generate ```.hex``` files) should navigate to your ```.hex``` file and type
```
objcopy -I ihex <your hex file>.hex -O binary output.bin
```
Obviously, you have to replace the name with your hex file's name. You can configure the output filename.
If you cannot find a ```.hex``` file, in the mcu8051ide press ```F11``` (or ```Tools``` >> ```compile``` in the options).