cmake_minimum_required(VERSION 3.5)
project(Dirty8051EmulatorTesting)

add_executable(Dirty8051EmulatorTesting
    emulator_testing.cpp
    test_main.cpp
)

target_link_libraries(Dirty8051EmulatorTesting PRIVATE
    Dirty8051EmulatorStatic
    GTest::GTest
    GTest::Main
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(test_all Dirty8051EmulatorTesting)

add_executable(gen_test_data
    gen_test_data.cpp
)

target_link_libraries(gen_test_data PRIVATE
    GTest::GTest
    GTest::Main
)
