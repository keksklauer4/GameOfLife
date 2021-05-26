#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cinttypes>
#include <cstring>
#include <thread>
#include <functional>
#include <chrono>
#include <QKeyEvent>
#include <QCheckBox>
#include <unordered_map>

#include "../src/emulator.hpp"
#include "led_handler.h"

class LEDWidget;

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setFilename(std::string filename) { filename_ = filename; }
  void startEmulator();

public slots:
  void onActionButtonPressed(uint8_t mask);
  void onActionButtonReleased(uint8_t mask);

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  LEDWidget* ledWidget_;
  QPushButton* buttons_[7];
  QCheckBox* checkboxes_[14];
  QVBoxLayout* mainLayout_;
  QHBoxLayout* buttonLayout_;
  std::string filename_;
  uint8_t buttonMask_;
  uint8_t holdState_ = 0;
};
#endif // MAINWINDOW_H
