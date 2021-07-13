cmake_minimum_required(VERSION 3.5)
project(8051EmulatorTest)

add_executable(8051EmulatorTest
    emulator_testing.cpp
    test_main.cpp
)

target_link_libraries(8051EmulatorTest PRIVATE
    8051EmulatorStatic
    GTest::GTest
    GTest::Main
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(test_all 8051EmulatorTest)

add_executable(gen_test_data
    gen_test_data.cpp
)

target_link_libraries(gen_test_data PRIVATE
    GTest::GTest
    GTest::Main
)
