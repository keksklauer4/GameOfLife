#include "mainwindow.h"
#include "LEDWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

using namespace emu;
using namespace std::chrono;

// for the emulator
#define REFRESH_FREQ 100
#define AMOUNT_CYCLES 12000000 / REFRESH_FREQ
#define PERIOD_LENGTH (std::chrono::milliseconds(1000 / REFRESH_FREQ))

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  QWidget* main = new QWidget(this);

  ledWidget_ = new LEDWidget(8, 8);
  ledWidget_->setMinimumSize(500, 500);
  ledWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  buttonLayout_ = new QHBoxLayout;
  for(int i = 0; i < 8; ++i)
  {
    buttons_[i] = new QPushButton(QString("%1").arg(i));
    buttonLayout_->addWidget(buttons_[i]);
    uint8_t mask = 0x01 << i;
    QObject::connect(buttons_[i], &QPushButton::clicked, this, [=]() {
      onActionButtonClicked(mask);
    });
  }

  mainLayout_ = new QVBoxLayout;
  mainLayout_->addWidget(ledWidget_, Qt::AlignCenter);
  mainLayout_->addLayout(buttonLayout_);

  main->setLayout(mainLayout_);
  setCentralWidget(main);
  buttonMask_ = 0;
}

MainWindow::~MainWindow()
{
}

void MainWindow::onActionButtonClicked(uint8_t mask)
{
  //ledWidget_->showCell(buttonNr, buttonNr, Qt::red);
  ledWidget_->repaint();
}


void MainWindow::startEmulator()
{
  LEDHandler led_handler(*ledWidget_);

  std::thread emulatorThread_([&] {
    emu::fuint32_t cycles_passed = 0;
    uint8_t mask = this->buttonMask_;
    auto callback = std::function<void(state_t&)>([&cycles_passed, &mask, &led_handler](state_t& state)
    {
      *(state.regs.P0) &= 0x80;
      *(state.regs.P0) |= mask;
      if (*(state.regs.P0) & 0x80)
      { // draw
        led_handler.setLines(*(state.regs.P1), *(state.regs.P3));
        cycles_passed += state.last_opcode;
      }
    });

    Emulator emu(callback);
    emu.setup(filename_);

    high_resolution_clock::time_point timer = high_resolution_clock::now();
    while (true)
    {
      emu.step();
      if (cycles_passed > AMOUNT_CYCLES)
      {
        cycles_passed = 0;
        std::this_thread::sleep_until(timer + PERIOD_LENGTH);
        timer = high_resolution_clock::now();
      }
    }
  });
}
