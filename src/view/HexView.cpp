#include "view/HexView.h"
#include <QPainter>
#include <QFontMetrics>

HexView::HexView(QWidget *parent)
    : QWidget(parent), backgroundColor(Qt::white), textColor(Qt::black) {}

void HexView::setHexData(const QString &data) {
    hexData = data;
    update(); // Trigger a repaint
}

void HexView::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
    update();
}

void HexView::setTextColor(const QColor &color) {
    textColor = color;
    update();
}

void HexView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);

    painter.setPen(textColor);
    painter.setFont(QFont("Courier", 12));

    QFontMetrics metrics(painter.font());
    int lineHeight = metrics.height();
    int x = 10;
    int y = lineHeight;

    for (int i = 0; i < hexData.length(); i += 2) {
        QString hexPair = hexData.mid(i, 2);
        painter.drawText(x, y, hexPair);
        x += metrics.horizontalAdvance(hexPair) + 10;

        if (x + metrics.horizontalAdvance(hexPair) > width()) {
            x = 10;
            y += lineHeight + 5;
        }
    }
}
