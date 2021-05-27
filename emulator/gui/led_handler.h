#ifndef LED_HANDLER_H
#define LED_HANDLER_H

#include <cinttypes>
#include <cstdint>

#include "LEDWidget.h"
#include "../src/types.hpp"

class LEDHandler
{
  public:
    LEDHandler(LEDWidget &_leds) : m_leds(_leds)
    {}

    // manipulates the LED matrix
    // the row select mask contains a bitset (of width 8).
    //    each bit set in rowSelectMask a row is selected that
    //    will change the led setting to colSettings contents.
    //
    // Example:
    // rowSelectMask = 0x87 = 0b10000111
    // colSetting    = 0xF1 = 0b11110001
    // Calling setLines with these parameters will affec rows 0,1,2 and 7.
    // (lowest bit corresponds to lowest row index. Upmost row is row with index 0)
    // Rows 0, 1, 2 and 7 will have setting 0xF1 which corresponds to the leftmost led
    //    and the 4 rightmost leds high with all others low in these rows.
    // Rows 3, 4, 5 and 6 will not be affected at all.
    void setLines(uint8_t rowSelectMask, uint8_t colSetting)
    {
      if (rowSelectMask == 0) return;

      uint64_t screen = m_leds.getScreen();
      emu::fuint32_t rowSelect = rowSelectMask;
      uint64_t selectMask = 0xFF;
      uint64_t changeMask = colSetting;

      while(rowSelect != 0)
      {
        if (rowSelect & 0x01)
        {
          screen &= (~selectMask);
          screen |= changeMask;
        }
        selectMask <<= 8;
        changeMask <<= 8;
        rowSelect >>= 1;
      }
      //std::cout << "Screen " << screen  << " Mask " << static_cast<uint32_t>(rowSelectMask) << " Col " << static_cast<uint32_t>(colSetting) << std::endl;

      m_leds.beginCellModification();
      m_leds.setScreen(screen);
      m_leds.endCellModification();
    }

    void forceRepaint()
    {
      emit m_leds.repaintRequired();
    }

  private:
    LEDWidget& m_leds;
};

#endif