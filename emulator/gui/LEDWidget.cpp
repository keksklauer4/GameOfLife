#include "LEDWidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <cstdint>

using emu::fuint32_t;

LEDWidget::LEDWidget(QWidget* parent)
  : QWidget(parent), visibleCellsCurrent_(0), visibleCells_(0)
{
  QObject::connect(this, &LEDWidget::repaintRequired, this, &LEDWidget::executeRepaint);
}

void LEDWidget::beginCellModification()
{
  mutex_.lock();
}

void LEDWidget::endCellModification()
{
  mutex_.unlock();
}

void LEDWidget::setScreen(uint64_t newVisibleCells)
{
  visibleCells_ = newVisibleCells;
}
void LEDWidget::executeRepaint()
{
  repaint();
}

void LEDWidget::paintEvent(QPaintEvent*)
{
  mutex_.lock();
  visibleCellsCurrent_ = visibleCells_;
  mutex_.unlock();

  cellWidth_ = width() / 8;
  cellHeight_ = height() / 8;

  QPainter painter(this);
  //drawGrid(painter);
  drawCells(painter);
}

void LEDWidget::resizeEvent(QResizeEvent *event)
{
  event->accept();

  if(event->size().width() > event->size().height()) {
    QWidget::resize(event->size().height(), event->size().height());
  } else {
    QWidget::resize(event->size().width(), event->size().width());
  }
}

void LEDWidget::drawGrid(QPainter &painter)
{
  float currentYVertical = 0;
  float currentXHorizontal = 0;
  unsigned int step = 0;

  do
  {
    currentYVertical = step * cellHeight_;
    currentXHorizontal = step * cellWidth_;

    if(currentXHorizontal < width())
    {
      painter.drawLine(QLineF(currentXHorizontal, 0, currentXHorizontal, height()));
    }

    if(currentYVertical < height())
    {
      painter.drawLine(QLineF(0, currentYVertical, width(), currentYVertical));
    }

    ++step;
  } while(currentXHorizontal < width() || currentYVertical < height());
}

void LEDWidget::drawCells(QPainter &painter)
{
  uint64_t cells = visibleCellsCurrent_;
  for (fuint32_t row = 0; row < 8; ++row)
  {
    for (fuint32_t col = 0; col < 8; ++col)
    {
      float x = cellWidth_ * col;
      float y = cellHeight_ * row;

      painter.fillRect(QRectF(x, y, cellWidth_, cellHeight_), (cells & 0x01 ? Qt::red : Qt::black));
      cells >>= 1;
    }
  }
}
