#include "mainwindow.h"
#include "config_gui.hpp"
#include "LEDWidget.h"

using namespace emu;
using namespace std::chrono;

// for the emulator
#define REFRESH_FREQ 30
#define AMOUNT_CYCLES 12000000 / REFRESH_FREQ
#define PERIOD_LENGTH (std::chrono::milliseconds(1000 / REFRESH_FREQ))

void MainWindow::startEmulator()
{
  std::thread emulatorThread_([&] {
    fuint32_t cycles_passed = 0;
    auto* buttomMaskPtr = &(this->buttonMask_);
    LEDHandler led_handler(*ledWidget_);
    auto callback = std::function<void(state_t&)>([&cycles_passed, buttomMaskPtr, &led_handler](state_t& state)
    {
      cycles_passed += state.last_opcode;
      *(state.regs.P0) &= 0x80;
      *(state.regs.P0) |= *buttomMaskPtr;

      #if USE_LED_MATRIX == 1
      if (*(LED_MATRIX_CONTROL_BYTE) & (0x01 << (LED_MATRIX_CONTROL_BIT)))
      { // draw
        //std::cout << "Drawing " << std::hex << static_cast<uint32_t>(*(state.regs.P1)) << "-" << static_cast<uint32_t>(*(state.regs.P3)) << std::endl;
        led_handler.setLines(*(LED_MATRIX_ROW_SELECT), *(LED_MATRIX_COLUMN_SET));

        if (__DEBUG__) {led_handler.forceRepaint();}
      }
      #endif
    });

    Emulator emu(callback);
    emu.setup(filename_);

    high_resolution_clock::time_point timer = high_resolution_clock::now();
    while (true)
    {
      emu.step();
      if (cycles_passed > AMOUNT_CYCLES)
      {
        led_handler.forceRepaint();
        cycles_passed = 0;
        std::this_thread::sleep_until(timer + PERIOD_LENGTH);
        timer = high_resolution_clock::now();
      }
    }
  });
  emulatorThread_.detach();
}
