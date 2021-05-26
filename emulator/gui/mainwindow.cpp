#include "mainwindow.h"
#include "LEDWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <cstdint>

using namespace emu;
using namespace std::chrono;

// for the emulator
#define REFRESH_FREQ 30
#define AMOUNT_CYCLES 12000000 / REFRESH_FREQ
#define PERIOD_LENGTH (std::chrono::milliseconds(1000 / REFRESH_FREQ))

static const std::unordered_map<Qt::Key, uint8_t> keyMapping
{
  { Qt::Key::Key_W, 0x01 },
  { Qt::Key::Key_A, 0x02 },
  { Qt::Key::Key_S, 0x04 },
  { Qt::Key::Key_D, 0x08 },
  { Qt::Key::Key_Q, 0x10 },
  { Qt::Key::Key_E, 0x20 },
  { Qt::Key::Key_R, 0x40 }
};

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  QWidget* main = new QWidget(this);

  ledWidget_ = new LEDWidget();
  ledWidget_->setMinimumSize(500, 500);
  ledWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  buttonLayout_ = new QHBoxLayout;
  for(int i = 0; i < 7; ++i)
  {
    auto verticalLayout = new QVBoxLayout;

    checkboxes_[2*i] = new QCheckBox(QString("Latch btn %1").arg(i));
    checkboxes_[2*i + 1] = new QCheckBox(QString("Btn %1 state").arg(i));
    checkboxes_[2*i + 1]->setEnabled(false);
    buttons_[i] = new QPushButton(QString("%1").arg(i));
    verticalLayout->addWidget(checkboxes_[2*i]);
    verticalLayout->addWidget(checkboxes_[2*i+1]);
    verticalLayout->addWidget(buttons_[i]);
    buttonLayout_->addLayout(verticalLayout);
    uint8_t mask = 0x01 << i;
    QObject::connect(buttons_[i], &QPushButton::pressed, this, [mask, this]() {
      onActionButtonPressed(mask);
    });
    QObject::connect(buttons_[i], &QPushButton::released, this, [mask, this]() {
      onActionButtonReleased(mask);
    });
    QObject::connect(checkboxes_[2*i], &QCheckBox::clicked, this, [this, i](bool newState) {
      if (newState)
      {
        this->holdState_ |= (0x01 << i);
        this->checkboxes_[2*i + 1]->setEnabled(true);
        this->buttons_[i]->setEnabled(false);
      }
      else
      {
        this->holdState_ &= (~(0x01 << i));
        this->checkboxes_[2*i + 1]->setEnabled(false);
        this->buttons_[i]->setEnabled(true);
      }
    });
    QObject::connect(checkboxes_[2*i + 1], &QCheckBox::clicked, this, [this, i](bool newState) {
      if (newState)
      {
        this->buttonMask_ |= (0x01 << i);
      }
      else {
        this->buttonMask_ &= (~(0x01 << i));
      }
    });
  }

  mainLayout_ = new QVBoxLayout;
  mainLayout_->addWidget(ledWidget_, Qt::AlignCenter);
  mainLayout_->addLayout(buttonLayout_);

  main->setLayout(mainLayout_);
  setCentralWidget(main);
  buttonMask_ = 0;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if (event == nullptr) return;
  auto buttonMapped = keyMapping.find((Qt::Key)event->key());
  if (buttonMapped != keyMapping.end())
  {
    buttonMask_ |= ((~holdState_) & buttonMapped->second);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
  if (event == nullptr) return;
  auto buttonMapped = keyMapping.find((Qt::Key)event->key());
  if (buttonMapped != keyMapping.end())
  {
    buttonMask_ &= ~((~holdState_) & buttonMapped->second);
  }
}

MainWindow::~MainWindow()
{}

void MainWindow::onActionButtonPressed(uint8_t mask)
{
  buttonMask_ |= mask;
}

void MainWindow::onActionButtonReleased(uint8_t mask)
{
  buttonMask_ &= (~mask);
}

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
      if (*(state.regs.P0) & 0x80)
      { // draw
        //std::cout << "Drawing " << std::hex << static_cast<uint32_t>(*(state.regs.P1)) << "-" << static_cast<uint32_t>(*(state.regs.P3)) << std::endl;
        led_handler.setLines(*(state.regs.P1), *(state.regs.P3));

        if (__DEBUG__) {led_handler.forceRepaint();}
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
        led_handler.forceRepaint();
        cycles_passed = 0;
        std::this_thread::sleep_until(timer + PERIOD_LENGTH);
        timer = high_resolution_clock::now();
      }
    }
  });
  emulatorThread_.detach();
}
