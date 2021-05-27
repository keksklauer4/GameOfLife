# GameOfLife
A game of life implementation in assembly for 8051 (trashy uController).

## Intro
This project contains a [GameOfLife implementation in assembly for the 8051](https://github.com/keksklauer4/GameOfLife/blob/master/main/main.asm) uController and an [emulator for the 8051](https://github.com/keksklauer4/GameOfLife/tree/master/emulator). The latter was needed because the emulator we used was extremely slow which is why we decided to implement our own Interpreter for 8051 assembly.

The GameOfLife implementation consists of a 32x32 matrix on which the user can draw pixels. Around the matrix a border is drawn to indicate that the neither the user nor the tiles can pass the matrix.
The asm was implemented for our own emulator and its GUI (written with Qt5) and cannot properly run on the mcu8051ide (because we used the PCON register in conjunction with 16 bit timers for elegant sleep routines in IDLE mode and a latching LED matrix).

## Key mappings
The GUI will show the LED matrix and buttons for interaction. The current button and key press mapping is:
- Navigate Up using key ```W``` or ```Button 0```
- Navigate Left using key ```A``` or ```Button 1```
- Naviagte Down using key ```S``` or ```Button 2```
- Naviagte Right using Key ```D``` or ```Button 3```
- Toggle current pixel's state using key ```Q``` or ```Button 4```
- Start/Stop using Key ```E``` or ```Button 5```
- Insert predefined pattern using key ```R``` or ```Button 6``` (see below for more precise information on that functionality)

Note that when start mode is enabled, you cannot place/erase (=toggle) pixels.

## Inset predefined patterns
This functionality enables a user to insert a predefined pattern. In order to use it, the user has to hold ```button 6``` or hold key ```R``` while in stop mode.
The game will wait for the button to be released and then copy the other buttons' states and interprete the value as a selection index for the pattern.
The above mentioned numbers button mappings (```button 0``` for up etc.) are interpreted as bit positions. If ```button 0``` was pressed when ```button 6``` (key ```R```) was released, bit 0 will be set (because of ```button 0``` being held). Therefore ```0x01``` is the index of the selected pattern (bit at ```position 0``` is set to 1). If ```button 1``` and ```button 0``` are pressed ```0x03``` (```= 00000011``` in binary) will be the index of the selected pattern.

Shortly after releasing ```button 6```, the button/key mappings will again be interpreted as explained above. Therefore, the user should quickly release the other buttons as well.

!!! Important !!!:
Currently, there are only ```4``` patterns defined in the [table](https://github.com/keksklauer4/GameOfLife/blob/master/main/lookup_tables.asm#L132) which implies that if a higher selection index is chosen, the game will crash! Regardless of that, in [lookup_tables.asm](https://github.com/keksklauer4/GameOfLife/blob/master/main/lookup_tables.asm) one can find an explanation to add more patterns.

## GUI
Due to the fact that we are really do not like creating UIs, it does not look optimal... But who cares anyway!

![Emulator running GameOfLife with a classical Glider drawn.](https://github.com/keksklauer4/GameOfLife/blob/master/img/example.png)

And here you can see pattern with selection index 0 copied (it is a nice oscillating pattern!).
![Emulator running GameOfLife with pattern 0 copied onto the screen.](https://github.com/keksklauer4/GameOfLife/blob/master/img/pattern0.png)
