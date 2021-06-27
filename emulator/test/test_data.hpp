#ifndef EMU_TEST_DATA_HPP
#define EMU_TEST_DATA_HPP

#include <vector>
#include <string>
#include <map>

#include "../src/types.hpp"
#include "emulator_testing.hpp"

#define BREAK_ADDRESS 0x400
#define JUMP_END() "ljmp l_breakpoint\n"
#define LINE(s) s "\n"
#define MK_FILENAME(name) "data/" name
#define PRE() ".ORG 0h\nljmp l_start\n\n.ORG 100h\nl_start:\n"
#define POST() "\n\n.ORG 400h\nl_breakpoint:\nNOP\nNOP\nNOP\nNOP\n"
#define TEST_DATA(filename, ...) std::make_pair(MK_FILENAME(filename) ".asm",PRE() __VA_ARGS__ POST())
namespace emu
{
namespace testing
{
  
static std::vector<std::pair<std::string, std::string>> data
{
  TEST_DATA(
    "test_add",
    LINE("mov a, #25")
    LINE("mov b, #45")
    LINE("add a, b")
    JUMP_END()
  ),
  TEST_DATA(
    "test_sub",
    LINE("mov a, #45")
    LINE("mov r2, #25")
    LINE("subb a, r2")
    JUMP_END()
  ),
  TEST_DATA(
    "test_push",
    LINE("mov p0, #10")
    LINE("push p0")
    LINE("pop b")
    JUMP_END()
  )
};

#define CREATE_TEST(fname) tests.emplace_back(test_config_t{MK_FILENAME(fname) ".bin"}); test = &tests.back()
#define ADD_COND(type, ...) test->m_conditions.push_back(std::unique_ptr<TestCondition>( new type{__VA_ARGS__}))
inline std::vector<test_config_t> create_test_conditions()
{
  std::vector<test_config_t> tests{};
  test_config_t* test;
  CREATE_TEST("test_add");
  ADD_COND(MemoryContains, A_OFFSET, 70, SFR);
  ADD_COND(MemoryContains, B_OFFSET, 45, SFR);

  CREATE_TEST("test_sub");
  ADD_COND(RegisterContains, 2, 25);
  ADD_COND(MemoryContains, A_OFFSET, 20, SFR);


  CREATE_TEST("test_push");
  ADD_COND(MemoryContains, P0_OFFSET, 10, SFR);
  ADD_COND(MemoryContains, B_OFFSET, 10, SFR);

  
  

  return tests;
}

}
}

#undef JUMP_END
#undef LINE
#undef PRE
#undef POST
#undef TEST_DATA
#undef MK_FILENAME

#endif