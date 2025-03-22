#include "BinaryWidget.h"
#include <QPainter>
#include <QDebug>

BinaryWidget::BinaryWidget(QWidget *parent)
    : QWidget(parent), backgroundColor(Qt::white), painterColor(Qt::red) {
    bitSet = std::vector<bool>();
}

void BinaryWidget::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
    update(); // Trigger a repaint
}

void BinaryWidget::updateBitSet(const std::vector<bool> &b) {
    bitSet = b;
    update();
}

void BinaryWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);

    QBrush painterBrush(painterColor);

    // Calculate the number of letters and bits dynamically
    int totalBits = static_cast<int>(bitSet.size());
    int numberLetterDraw = std::max(1, totalBits / 7 + (totalBits % 7 != 0 ? 1 : 0)); // Number of columns
    int bitByLetter = 7; // Fixed number of bits per letter (rows)

    // Calculate the size of the grid dynamically
    int cellWidth = std::max(1, width() / numberLetterDraw);
    int cellHeight = std::max(1, height() / bitByLetter);
    int cellSize = std::min(cellWidth, cellHeight);
    int circleSize = cellSize * 0.8;

    // Calculate the starting position to center the grid
    int gridWidth = numberLetterDraw * cellSize;
    int gridHeight = bitByLetter * cellSize;
    int startX = (width() - gridWidth) / 2;
    int startY = (height() - gridHeight) / 2;

    // Draw the binary grid
    for (int i = 0; i < numberLetterDraw; ++i) {
        for (int j = 0; j < bitByLetter; ++j) {
            int index = i * bitByLetter + j;
            if (index < totalBits) {
                if (bitSet.at(index)) {
                    painter.setBrush(painterBrush); // Red for 1
                } else {
                    painter.setBrush(backgroundColor); // Use background color for 0
                }
                int x = startX + i * cellSize + (cellSize - circleSize) / 2;
                int y = startY + j * cellSize + (cellSize - circleSize) / 2;
                painter.drawEllipse(x, y, circleSize, circleSize);
            }
        }
    }
}