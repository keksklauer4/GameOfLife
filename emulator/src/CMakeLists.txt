project(8051EmulatorStatic)

add_library(8051EmulatorStatic STATIC
    emulator.cpp
    opcode_handler_macro.cpp
    opcode_handler.cpp
)

