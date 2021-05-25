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