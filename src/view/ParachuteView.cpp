#include "ParachuteView.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFileDialog>
#include <cmath>

ParachuteView::ParachuteView(QWidget *parent) 
    : QWidget(parent), sectors(7), tracks(5), 
      backgroundColor(Qt::white), parachuteColor(Qt::red), sectorColor(Qt::white),
      randomColorMode(false) {}

void ParachuteView::setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage) {
    this->sectors = sectors;
    this->tracks = tracks;
    this->encodedMessage = encodedMessage;
    
    // Générer de nouvelles couleurs aléatoires si le mode est activé
    if (randomColorMode) {
        generateRandomColors();
    }
    
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

void ParachuteView::setSectorColor(QColor color) {
    sectorColor = color;
    update();
}

void ParachuteView::setRandomColorMode(bool enabled) {
    randomColorMode = enabled;
    if (randomColorMode) {
        generateRandomColors();
    }
    update();
}

void ParachuteView::generateRandomColors() {
    randomColors.clear();
    QRandomGenerator *rng = QRandomGenerator::global();
    
    // Générer une couleur aléatoire pour chaque indice potentiel
    for (int i = 0; i < sectors * tracks; i++) {
        if (i < encodedMessage.size() && encodedMessage[i]) {
            // Couleur aléatoire avec une bonne saturation et luminosité
            int hue = rng->bounded(360);
            QColor randomColor = QColor::fromHsv(hue, 200 + rng->bounded(55), 200 + rng->bounded(55));
            randomColors[i] = randomColor;
        }
    }
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

    QBrush sectorBrush(sectorColor);
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
            
            if (encodedMessage[index]) {
                if (randomColorMode && randomColors.contains(index)) {
                    // Utiliser la couleur aléatoire pour ce secteur
                    painter.setBrush(QBrush(randomColors[index]));
                } else {
                    // Utiliser la couleur standard pour les bits à 1
                    painter.setBrush(QBrush(parachuteColor));
                }
            } else {
                // Utiliser la couleur pour les bits à 0
                painter.setBrush(sectorBrush);
            }
            
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