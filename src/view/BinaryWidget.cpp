#include "BinaryWidget.h"
#include <QPainter>

BinaryWidget::BinaryWidget(QWidget *parent)
    : QWidget(parent), numberLetterDraw(14), bitByLetter(7),
      backgroundColor(Qt::white), painterColor(Qt::red) {
    bitSet = std::vector<bool>();
}

void BinaryWidget::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
    update(); // Trigger a repaint
}

void BinaryWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.fillRect(rect(), backgroundColor);

    QBrush painterBrush(painterColor);

    int cellWidth = width() / numberLetterDraw; 
    int cellHeight = height() / bitByLetter;    
    int cellSize = std::min(cellWidth, cellHeight); 
    int circleSize = cellSize * 0.8; 


    int gridWidth = numberLetterDraw * cellSize;
    int gridHeight = bitByLetter * cellSize;
    int startX = (width() - gridWidth) / 2;
    int startY = (height() - gridHeight) / 2;

    for (int i = 0; i < numberLetterDraw; ++i) {
        for (int j = 0; j < bitByLetter; ++j) {
            int index = i * bitByLetter + j;
            if (index < static_cast<int>(bitSet.size()) && bitSet.at(index)) {
                painter.setBrush(painterBrush); 
            } else {
                painter.setBrush(backgroundColor); 
            }

            int x = startX + i * cellSize + (cellSize - circleSize) / 2;
            int y = startY + j * cellSize + (cellSize - circleSize) / 2;
            painter.drawEllipse(x, y, circleSize, circleSize);
        }
    }
}

void BinaryWidget::updateBitSet(const std::vector<bool> &b) {
    bitSet = b;
    update();
}