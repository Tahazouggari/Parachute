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

    // Fill the entire widget with the background color
    painter.fillRect(rect(), backgroundColor);

    QBrush painterBrush(painterColor);

    // Calculate the size of the grid dynamically
    int cellWidth = width() / numberLetterDraw; // Width of each cell
    int cellHeight = height() / bitByLetter;    // Height of each cell
    int cellSize = std::min(cellWidth, cellHeight); // Use the smaller dimension for square cells
    int circleSize = cellSize * 0.8; // Circle size is 80% of the cell size

    // Calculate the starting position to center the grid
    int gridWidth = numberLetterDraw * cellSize;
    int gridHeight = bitByLetter * cellSize;
    int startX = (width() - gridWidth) / 2;
    int startY = (height() - gridHeight) / 2;

    // Draw the binary grid
    for (int i = 0; i < numberLetterDraw; ++i) {
        for (int j = 0; j < bitByLetter; ++j) {
            int index = i * bitByLetter + j;
            if (index < static_cast<int>(bitSet.size()) && bitSet.at(index)) {
                painter.setBrush(painterBrush); // Red for 1
            } else {
                painter.setBrush(backgroundColor); // Use background color for 0
            }
            // Draw circle centered in the cell
            int x = startX + i * cellSize + (cellSize - circleSize) / 2;
            int y = startY + j * cellSize + (cellSize - circleSize) / 2;
            painter.drawEllipse(x, y, circleSize, circleSize);
        }
    }
}

void BinaryWidget::updateBitSet(const std::vector<bool> &b) {
    bitSet = b;
    update(); // Trigger repaint
}