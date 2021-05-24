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

    void setLines(uint8_t rowSelectMask, uint8_t colSetting)
    {
      if (rowSelectMask == 0) return;

      uint64_t screen = m_leds.getScreen();
      emu::fuint32_t rowSelect = rowSelectMask;
      uint64_t selectMask = 0;
      uint64_t changeMask = 0;
      while(rowSelect != 0)
      {
        changeMask <<= 8;
        selectMask <<= 8;
        if (rowSelect & 0x01)
        {
          changeMask |= colSetting;
          selectMask |= 0xff;
        }
        rowSelect >>= 1;
      }

      screen &= (~selectMask);
      screen |= changeMask;

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