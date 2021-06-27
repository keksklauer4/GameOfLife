#ifndef EMU_TEST_CONDITIONS_HPP
#define EMU_TEST_CONDITIONS_HPP

#include "../src/emulator.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <cinttypes>
#include <stdexcept>

namespace emu
{
namespace testing
{
  enum MemoryType
  {
    INTERNAL, //
    SFR,      // special function registers
    EXTERNAL
  };
  namespace helper
  {
    inline uint8_t fetchFromAddress(state_t& state, uint16_t address, MemoryType type)
    {
      switch(type)
      {
        case INTERNAL:
        {
          if (address >= 256) throw std::range_error("Error address out of internal data range! " + ((int)address));
          return state.internal_data[address];
        }
        case SFR:
        {
          if (address >= 128) throw std::range_error("Error address out of SFR range! " + ((int)address));
          return state.sfr_memory[address];
        }
        case EXTERNAL:
        {
          return state.external_memory[address];
        }
        default:
          throw std::runtime_error("Error, unknown address type!");
      }
      return 0;
    }

    inline uint8_t getRegisterValue(state_t& state, int reg)
    {
      if (reg >= 8) throw new std::runtime_error("Error. Bad register!");

      int reg_bank = ((*state.regs.PSW) & 0x00011000);
      return *(state.internal_data + reg_bank + reg);
    }
  }

  class TestCondition
  {
  public:
    virtual ~TestCondition(){ }
    virtual void execute(state_t& state) = 0;
  };

  class MemoryContains : public TestCondition
  {
  public:
    MemoryContains(uint16_t address, uint8_t val, MemoryType type)
      : m_val(val), m_address(address), m_type(type) {}
    void execute(state_t& state) override
    {
      ASSERT_EQ(helper::fetchFromAddress(state, m_address, m_type), m_val);
    }

    private:
      uint16_t m_address;
      uint8_t m_val;
      MemoryType m_type;
  };

  class RegisterContains : public TestCondition
  {
  public:
    RegisterContains(int reg, uint8_t val)
      : m_reg(reg), m_val(val) {}
    void execute(state_t& state) override
    {
      ASSERT_EQ(helper::getRegisterValue(state, m_reg), m_val);
    }

    private:
      int m_reg;
      uint8_t m_val;
      MemoryType m_type;
  };

  class FlagCheck : public TestCondition
  {
    public:
      void execute(state_t& state) override
      {
        ASSERT_TRUE((getFlagSetting(state) != 0) == m_expected);
      }
      virtual bool getFlagSetting(state_t& state) = 0;
    protected:
      bool m_expected;
  };

  #define CREATE_FLAG_CHECK(Flag, addr_offset, bit_offset)  \
    class Flag##FlagCheck : public FlagCheck                \
    {                                                       \
    public:                                                 \
      Flag##FlagCheck(bool expected){ this->m_expected = expected; } \
      bool getFlagSetting(state_t& state) override { return (*(state.sfr_memory + addr_offset)) >> bit_offset; }  \
    }

  CREATE_FLAG_CHECK(Carry, PSW_OFFSET, 7);

}
}

#endif
