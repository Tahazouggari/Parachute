#include "ParachuteView.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFileDialog>
#include <cmath>

ParachuteView::ParachuteView(QWidget *parent) 
    : QWidget(parent), sectors(7), tracks(5), 
      backgroundColor(Qt::white), parachuteColor(Qt::red) {}

void ParachuteView::setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage) {
    this->sectors = sectors;
    this->tracks = tracks;
    this->encodedMessage = encodedMessage;
    update(); // Redraw
}

void ParachuteView::setBackgroundColor(QColor color) {
    backgroundColor = color;
    update();
}

void ParachuteView::setParachuteColor(QColor color) {
    parachuteColor = color;
    update();
}

void ParachuteView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set background color
    painter.fillRect(rect(), backgroundColor);

    int centerX = width() / 2;
    int centerY = height() / 2;
    double radius = std::min(width(), height()) / 2.2;
    double trackWidth = radius / tracks;
    double angleStep = 2 * M_PI / sectors;

    QBrush whiteBrush(Qt::white);
    QBrush fillBrush(parachuteColor);
    QPen borderPen(Qt::black);
    borderPen.setWidth(1);

    int index = 0;
    for (int t = 0; t < tracks; t++) {
        double innerRadius = t * trackWidth;
        double outerRadius = (t + 1) * trackWidth;

        for (int s = 0; s < sectors; s++) {
            if (index >= static_cast<int>(encodedMessage.size())) break;

            double startAngle = s * angleStep;
            double endAngle = (s + 1) * angleStep;

            QPointF p1(innerRadius * cos(startAngle) + centerX, innerRadius * sin(startAngle) + centerY);
            QPointF p2(innerRadius * cos(endAngle) + centerX, innerRadius * sin(endAngle) + centerY);
            QPointF p3(outerRadius * cos(startAngle) + centerX, outerRadius * sin(startAngle) + centerY);
            QPointF p4(outerRadius * cos(endAngle) + centerX, outerRadius * sin(endAngle) + centerY);

            QPolygonF trapezoid;
            trapezoid << p1 << p2 << p4 << p3;
            painter.setPen(borderPen);
            painter.setBrush(encodedMessage[index] ? fillBrush : whiteBrush);
            painter.drawPolygon(trapezoid);

            index++;
        }
    }
}

void ParachuteView::saveParachuteImage(const QString &filename) {
    QPixmap pixmap(size());
    render(&pixmap);
    pixmap.save(filename);
}