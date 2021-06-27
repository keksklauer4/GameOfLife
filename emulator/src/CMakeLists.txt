project(Dirty8051EmulatorStatic)

add_library(Dirty8051EmulatorStatic STATIC
    emulator.cpp
    opcode_handler_macro.cpp
    opcode_handler.cpp
)

