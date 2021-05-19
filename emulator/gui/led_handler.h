#ifndef LED_HANDLER_H
#define LED_HANDLER_H

#include <cinttypes>

#include "LEDWidget.h"
#include "../src/types.hpp"

#define AMOUNT_LINES 8

class LEDHandler
{
  public:
    LEDHandler(LEDWidget &_leds) : m_leds(_leds)
    {
      std::memset(m_ledStatus, 0, AMOUNT_LINES);
    }

    void setLines(uint8_t rowSelectMask, uint8_t colSetting)
    {
      emu::fuint32_t rowSelect = rowSelectMask;
      emu::fuint32_t colChangeMask;
      emu::fuint32_t newColSetting;
      emu::fuint32_t row = 0;
      emu::fuint32_t col;
      bool needsRepaint = false;
      while (rowSelect != 0)
      {
        col = 0;
        colChangeMask = colSetting ^ m_ledStatus[row]; // get bits that changed
        m_ledStatus[row] = colSetting;
        if (colChangeMask != 0)
        {
          newColSetting = colSetting; // copy new setting
          while(colChangeMask != 0)
          {
            if (colChangeMask & 0x01)
            {
              m_leds.showCell(col, row, (newColSetting & 0x01 ? Qt::red : Qt::black));
              needsRepaint = true;
            }
            col++;
            colChangeMask >>= 1;
            newColSetting >>= 1;
          }
        }
        row++;
      }
      if (needsRepaint) m_leds.repaint();
    }

  private:
    LEDWidget& m_leds;
    uint8_t m_ledStatus[AMOUNT_LINES];
};

#endif