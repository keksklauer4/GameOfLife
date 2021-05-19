#include "LEDWidget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

LEDWidget::LEDWidget(unsigned int cols, unsigned int rows, QWidget* parent)
  : QWidget(parent), rows_(rows), cols_(cols)
{
  showCell(1, 1, Qt::black);
}

unsigned int LEDWidget::getColumnCount() const
{
  return cols_;
}

unsigned int LEDWidget::getRowCount() const
{
  return rows_;
}

void LEDWidget::clear()
{
  visibleCells_.clear();
}

void LEDWidget::showCell(unsigned int col, unsigned int row, QColor color)
{
  visibleCells_.insert(std::make_pair(std::make_pair(col, row), color));
}

void LEDWidget::hideCell(unsigned int col, unsigned int row, QColor)
{
  visibleCells_.erase(std::make_pair(col, row));
}

void LEDWidget::paintEvent(QPaintEvent*)
{
  cellWidth_ = width() / cols_;
  cellHeight_ = height() / rows_;

  QPainter painter(this);
  drawGrid(painter);
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
  for(auto it = visibleCells_.begin(); it != visibleCells_.end(); ++it)
  {
    float x = cellWidth_ * it->first.first;
    float y = cellHeight_ * it->first.second;

    painter.fillRect(QRectF(x, y, cellWidth_, cellHeight_), it->second);
  }
}
