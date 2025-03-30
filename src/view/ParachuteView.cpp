#include "view/ParachuteView.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFileDialog>


#include <cmath>

ParachuteView::ParachuteView(QWidget *parent) 
    : QWidget(parent), sectors(7), tracks(5), originalSectors(7),
      backgroundColor(Qt::white), parachuteColor(Qt::red), sectorColor(Qt::white),
      randomColorMode(false), mode10(false), useBackgroundImage(false) {}

void ParachuteView::setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage) {
    // En mode 10, on ajuste le nombre de secteurs pour être un multiple de 10
    if (!mode10) {
        this->sectors = sectors;
        this->originalSectors = sectors;
        this->tracks = tracks;
        this->encodedMessage = encodedMessage;
    } 
    else {
        this->tracks = tracks;
        this->encodedMessage = encodedMessage;
        
        if (sectors % 10 != 0) {
            this->originalSectors = sectors;
        }
        
        //pour arrondir au multiple de 10 supérieur
        if (sectors % 10 != 0) {
            this->sectors = (int)(std::ceil(sectors / 10.0) * 10);
        } else {
            this->sectors = sectors;
        }
    }
    
    if (randomColorMode) {
        generateRandomColors();
    }
    
    update();
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
        if (mode10) {
            originalSectors = sectors;
            
            if (sectors % 10 != 0) {
                sectors = (int)(std::ceil(sectors / 10.0) * 10);
            }
        }
        else {
            sectors = originalSectors;
        }
    }
    
    update();
}

void ParachuteView::setBackgroundImage(const QString &imagePath) {
    if (backgroundImage.load(imagePath)) {
        useBackgroundImage = true;
        update();
    }
}

void ParachuteView::clearBackgroundImage() {
    useBackgroundImage = false;
    backgroundImage = QPixmap();
    update();
}

bool ParachuteView::hasBackgroundImage() const {
    return useBackgroundImage && !backgroundImage.isNull();
}

void ParachuteView::generateRandomColors() {
    randomColors.clear();
    QRandomGenerator *rng = QRandomGenerator::global();
    
    for (int i = 0; i < sectors * tracks; i++) {
        if (i < encodedMessage.size() && encodedMessage[i]) {
            int hue = rng->bounded(360);
            QColor randomColor = QColor::fromHsv(hue, 200 + rng->bounded(55), 200 + rng->bounded(55));
            randomColors[i] = randomColor;
        }
    }
}

void ParachuteView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // on dessine l'image de fond si elle existe, sinon utiliser la couleur de fond
    if (useBackgroundImage && !backgroundImage.isNull()) {
        painter.drawPixmap(rect(), backgroundImage.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        painter.fillRect(rect(), backgroundColor);
    }

    int centerX = width() / 2;
    int centerY = height() / 2;
    
    // on calcule le rayon maximum pour que le parachute tienne dans la fenêtre
    double maxRadius = std::min(width(), height()) / 2.2;
    double trackWidth = maxRadius / tracks;
    double angleStep = 2 * M_PI / sectors;

    QBrush sectorBrush(sectorColor);
    QPen borderPen(Qt::black);
    borderPen.setWidth(1);

    // on dessine chaque piste (track) du parachute
    for (int t = 0; t < tracks; t++) {
        double innerRadius = t * trackWidth;
        double outerRadius = (t + 1) * trackWidth;
        
        //on dessine chaque secteur de la piste courante
        for (int s = 0; s < sectors; s++) {
            int index = t * sectors + s;
            double startAngle = s * angleStep;
            double endAngle = (s + 1) * angleStep;

            // on calcule les quatre points du trapèze qui forme le secteur
            QPointF p1(innerRadius * cos(startAngle) + centerX, innerRadius * sin(startAngle) + centerY);
            QPointF p2(innerRadius * cos(endAngle) + centerX, innerRadius * sin(endAngle) + centerY);
            QPointF p3(outerRadius * cos(startAngle) + centerX, outerRadius * sin(startAngle) + centerY);
            QPointF p4(outerRadius * cos(endAngle) + centerX, outerRadius * sin(endAngle) + centerY);

            QPolygonF trapezoid;
            trapezoid << p1 << p2 << p4 << p3;
            painter.setPen(borderPen);
            
            // on choisit la couleur du secteur en fonction du bit correspondant
            if (index < static_cast<int>(encodedMessage.size()) && encodedMessage[index]) {
                if (randomColorMode && randomColors.contains(index)) {
                    painter.setBrush(QBrush(randomColors[index]));
                } else {
                    painter.setBrush(QBrush(parachuteColor));
                }
            } else {
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