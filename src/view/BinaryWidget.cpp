#include "BinaryWidget.h"
#include <QPainter>

BinaryWidget::BinaryWidget(QWidget *parent)
    : QWidget(parent), numberLetterDraw(14), bitByLetter(7),
      backgroundColor(Qt::white), painterColor(Qt::red) {
    bitSet = std::vector<bool>();
}

void BinaryWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush backgroundBrush(backgroundColor);
    QBrush painterBrush(painterColor);

    // Draw the binary grid
    for (int i = 0; i < numberLetterDraw; ++i) {
        for (int j = 0; j < bitByLetter; ++j) {
            int index = i * 7 + j;
            if (index < static_cast<int>(bitSet.size()) && bitSet.at(index)) {
                painter.setBrush(painterBrush); // Red for 1
            } else {
                painter.setBrush(backgroundBrush); // White for 0
            }
            painter.drawEllipse(i * 12, j * 12, 10, 10); // Draw circle
        }
    }
}

void BinaryWidget::updateBitSet(const std::vector<bool> &b) {
    bitSet = b;
    update(); // Trigger repaint
}
