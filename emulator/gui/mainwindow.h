#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cinttypes>
#include <cstring>
#include <thread>
#include <functional>
#include <chrono>

#include "../src/emulator.hpp"
#include "led_handler.h"

class LEDWidget;

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setFilename(std::string filename) { filename_ = filename; }
  void startEmulator();

public slots:
  void onActionButtonClicked(uint8_t mask);


private:
  LEDWidget* ledWidget_;
  QPushButton* buttons_[8];
  QVBoxLayout* mainLayout_;
  QHBoxLayout* buttonLayout_;
  std::string filename_;
  uint8_t buttonMask_;
};
#endif // MAINWINDOW_H
