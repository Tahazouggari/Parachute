#include "ParachuteModel.h"
#include <QRandomGenerator>

ParachuteModel::ParachuteModel(int sectors, int tracks)
    : sectors(sectors), tracks(tracks), 
      backgroundColor(Qt::white), parachuteColor(Qt::red), sectorColor(Qt::white),
      randomColorMode(false) {
}

void ParachuteModel::setMessage(const std::vector<int> &message) {
    encodedMessage = message;
    if (randomColorMode) {
        generateRandomColors();
    }
}

void ParachuteModel::setSectors(int s) {
    sectors = s;
    if (randomColorMode) {
        generateRandomColors();
    }
}

void ParachuteModel::setTracks(int t) {
    tracks = t;
    if (randomColorMode) {
        generateRandomColors();
    }
}

int ParachuteModel::getSectors() const {
    return sectors;
}

int ParachuteModel::getTracks() const {
    return tracks;
}

std::vector<int> ParachuteModel::getEncodedMessage() const {
    return encodedMessage;
}

void ParachuteModel::setBackgroundColor(const QColor &color) {
    backgroundColor = color;
}

void ParachuteModel::setParachuteColor(const QColor &color) {
    parachuteColor = color;
}

void ParachuteModel::setSectorColor(const QColor &color) {
    sectorColor = color;
}

void ParachuteModel::setRandomColorMode(bool enabled) {
    randomColorMode = enabled;
    if (randomColorMode) {
        generateRandomColors();
    }
}

void ParachuteModel::generateRandomColors() {
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

QColor ParachuteModel::getBackgroundColor() const {
    return backgroundColor;
}

QColor ParachuteModel::getParachuteColor() const {
    return parachuteColor;
}

QColor ParachuteModel::getSectorColor() const {
    return sectorColor;
}

bool ParachuteModel::getRandomColorMode() const {
    return randomColorMode;
}

QMap<int, QColor> ParachuteModel::getRandomColors() const {
    return randomColors;
}
