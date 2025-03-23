#include "view/BinaryWidget.h"
#include <QPainter>
#include <QDebug>
#include <QRandomGenerator>

BinaryWidget::BinaryWidget(QWidget *parent)
    : QWidget(parent), backgroundColor(Qt::white), painterColor(Qt::red), 
      bitOffColor(Qt::gray), randomColorMode(false) {
    bitSet = std::vector<bool>();
    // c'est la capacité initiale de la fenêtre
    setMinimumSize(100, 100);
}

void BinaryWidget::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
    update(); 
}

void BinaryWidget::updateBitSet(const std::vector<bool> &b) {
    bitSet = b;
    
    if (randomColorMode) {
        generateRandomColors();
    }
    
    update();
}

void BinaryWidget::setBitOnColor(const QColor &color) {
    painterColor = color;
    update();
}

void BinaryWidget::setBitOffColor(const QColor &color) {
    bitOffColor = color;
    update();
}

void BinaryWidget::setRandomColorMode(bool enabled) {
    randomColorMode = enabled;
    if (randomColorMode) {
        generateRandomColors();
    }
    update();
}

void BinaryWidget::generateRandomColors() {
    randomColors.clear();
    QRandomGenerator *rng = QRandomGenerator::global();
    
    for (size_t i = 0; i < bitSet.size(); i++) {
        if (bitSet[i]) {
            
            int hue = rng->bounded(360);
            QColor randomColor = QColor::fromHsv(hue, 200 + rng->bounded(55), 200 + rng->bounded(55));
            randomColors[i] = randomColor;
        }
    }
}

void BinaryWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);

    if (bitSet.empty()) return;

    
    int totalBits = static_cast<int>(bitSet.size());
    int numberLetterDraw = std::max(1, totalBits / 7 + (totalBits % 7 != 0 ? 1 : 0)); // Number of columns
    int bitByLetter = 7; // nombre de bits fixé

    
    int cellWidth = std::max(1, width() / numberLetterDraw);
    int cellHeight = std::max(1, height() / bitByLetter);
    int cellSize = std::min(cellWidth, cellHeight);
    int circleSize = cellSize * 0.8;

    int gridWidth = numberLetterDraw * cellSize;
    int gridHeight = bitByLetter * cellSize;
    int startX = (width() - gridWidth) / 2;
    int startY = (height() - gridHeight) / 2;

    // pour la représentation binaire
    for (int i = 0; i < numberLetterDraw; ++i) {
        for (int j = 0; j < bitByLetter; ++j) {
            int index = i * bitByLetter + j;
            if (index < totalBits) {
                if (bitSet.at(index)) {
                    if (randomColorMode && randomColors.contains(index)) {
                        painter.setBrush(QBrush(randomColors[index]));
                    } else {
                        painter.setBrush(QBrush(painterColor)); // Couleur pour bits à 1
                    }
                } else {
                    painter.setBrush(QBrush(bitOffColor)); // Couleur pour bits à 0
                }
                int x = startX + i * cellSize + (cellSize - circleSize) / 2;
                int y = startY + j * cellSize + (cellSize - circleSize) / 2;
                painter.drawEllipse(x, y, circleSize, circleSize);
            }
        }
    }
}