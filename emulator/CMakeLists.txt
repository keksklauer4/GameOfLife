cmake_minimum_required(VERSION 3.0)
project(Dirty8051Emulator)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

enable_testing()

add_subdirectory(3rd-party/google-test)
add_subdirectory(src)
add_subdirectory(test)
