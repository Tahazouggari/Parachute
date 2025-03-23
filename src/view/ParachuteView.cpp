#include "ParachuteView.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFileDialog>
#include <cmath>

ParachuteView::ParachuteView(QWidget *parent) 
    : QWidget(parent), sectors(7), tracks(5), originalSectors(7),
      backgroundColor(Qt::white), parachuteColor(Qt::red), sectorColor(Qt::white),
      randomColorMode(false), mode10(false) {}

void ParachuteView::setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage) {
    // En mode normal (7 bits), utiliser directement les valeurs fournies
    if (!mode10) {
        this->sectors = sectors;
        this->originalSectors = sectors; // Mémoriser le nombre original de secteurs
        this->tracks = tracks;
        this->encodedMessage = encodedMessage;
    } 
    // En mode 10, ajuster le nombre de secteurs si nécessaire
    else {
        this->tracks = tracks;
        this->encodedMessage = encodedMessage;
        
        // En mode 10, on mémorise d'abord la valeur originale si elle n'est pas un multiple de 10
        if (sectors % 10 != 0) {
            this->originalSectors = sectors;
        }
        
        // Vérifier que le nombre de secteurs est un multiple de 10
        if (sectors % 10 != 0) {
            this->sectors = (int)(std::ceil(sectors / 10.0) * 10);
        } else {
            this->sectors = sectors;
        }
    }
    
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

void ParachuteView::setMode10(bool enabled) {
    bool oldMode = mode10;
    mode10 = enabled;
    
    if (oldMode != mode10) {
        // Passage du mode 7 au mode 10
        if (mode10) {
            // Sauvegarder le nombre original de secteurs avant d'ajuster
            originalSectors = sectors;
            
            // Ajuster le nombre de secteurs à un multiple de 10
            if (sectors % 10 != 0) {
                sectors = (int)(std::ceil(sectors / 10.0) * 10);
            }
        }
        // Passage du mode 10 au mode 7
        else {
            // Restaurer le nombre original de secteurs
            sectors = originalSectors;
        }
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
    
    // Fixed maximum radius regardless of track count
    double maxRadius = std::min(width(), height()) / 2.2;
    
    // Calculate track width based on the fixed maximum radius
    double trackWidth = maxRadius / tracks;

    double angleStep = 2 * M_PI / sectors;

    QBrush sectorBrush(sectorColor);
    QPen borderPen(Qt::black);
    borderPen.setWidth(1);

    // Draw all tracks, regardless of message size
    for (int t = 0; t < tracks; t++) {
        double innerRadius = t * trackWidth;
        double outerRadius = (t + 1) * trackWidth;
        
        // Draw all sectors in the track
        for (int s = 0; s < sectors; s++) {
            int index = t * sectors + s;
            double startAngle = s * angleStep;
            double endAngle = (s + 1) * angleStep;

            QPointF p1(innerRadius * cos(startAngle) + centerX, innerRadius * sin(startAngle) + centerY);
            QPointF p2(innerRadius * cos(endAngle) + centerX, innerRadius * sin(endAngle) + centerY);
            QPointF p3(outerRadius * cos(startAngle) + centerX, outerRadius * sin(startAngle) + centerY);
            QPointF p4(outerRadius * cos(endAngle) + centerX, outerRadius * sin(endAngle) + centerY);

            QPolygonF trapezoid;
            trapezoid << p1 << p2 << p4 << p3;
            painter.setPen(borderPen);
            
            // Only color the sector if it exists in the encoded message
            if (index < static_cast<int>(encodedMessage.size()) && encodedMessage[index]) {
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
        }
    }
}

void ParachuteView::saveParachuteImage(const QString &filename) {
    QPixmap pixmap(size());
    render(&pixmap);
    pixmap.save(filename);
}

int ParachuteView::getOriginalSectors() const {
    return originalSectors;
}