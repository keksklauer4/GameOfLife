#ifndef LEDWIDGET_H
#define LEDWIDGET_H

#include <QWidget>
#include <QHash>
#include <cstdint>
#include <iostream>
#include <cinttypes>

#include <mutex>

#include "../src/emulator.hpp"

class QPaintEvent;
class QPainter;

class LEDWidget : public QWidget
{
  Q_OBJECT

public:

  explicit LEDWidget(QWidget* parent = nullptr);

  void beginCellModification();
  void endCellModification();

public slots:

  uint64_t getScreen() { return visibleCells_; }
  void setScreen(uint64_t newVisibleCells);
  void executeRepaint();

signals:

  void repaintRequired();

protected:

  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

private:

  void drawGrid(QPainter &painter);
  void drawCells(QPainter &painter);

private:
  uint64_t visibleCellsCurrent_;
  uint64_t visibleCells_;
  float cellWidth_, cellHeight_;
  std::mutex mutex_;
};

inline uint qHash(const QPair<unsigned int, unsigned int> &key, uint seed)
{
  return qHash(key.first, seed) ^ qHash(key.second);
}

#endif // LEDWIDGET_H
