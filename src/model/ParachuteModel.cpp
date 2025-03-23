#include "ParachuteModel.h"
#include <QRandomGenerator>
#include <cmath>

ParachuteModel::ParachuteModel(int sectors, int tracks)
    : sectors(sectors), tracks(tracks), originalSectors(sectors),
      backgroundColor(Qt::white), parachuteColor(Qt::red), sectorColor(Qt::white),
      randomColorMode(false), mode10(false) {
}

void ParachuteModel::setMessage(const std::vector<int> &message) {
    encodedMessage = message;
    if (randomColorMode) {
        generateRandomColors();
    }
}

void ParachuteModel::setSectors(int s) {
    // Toujours stocker la valeur originale demandée
    originalSectors = s;
    
    // En mode normal (7 bits), utiliser directement la valeur fournie
    if (!mode10) {
        sectors = s;
    }
    // En mode 10, ajuster à un multiple de 10
    else {
        sectors = adjustSectorsForMode10(s);
    }
    
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

void ParachuteModel::setMode10(bool enabled) {
    bool oldMode = mode10;
    mode10 = enabled;
    
    if (oldMode != mode10) {
        // Passage au mode 10
        if (mode10) {
            // Ajuster le nombre de secteurs à un multiple de 10
            sectors = adjustSectorsForMode10(sectors);
        }
        // Passage au mode 7
        else {
            // Restaurer le nombre original de secteurs
            sectors = originalSectors;
        }
    }
}

bool ParachuteModel::getMode10() const {
    return mode10;
}

int ParachuteModel::adjustSectorsForMode10(int requestedSectors) const {
    // Si mode10 est actif, ajuster le nombre de secteurs au multiple de 10 supérieur ou égal
    if (mode10) {
        return (int)(std::ceil(requestedSectors / 10.0) * 10);
    }
    return requestedSectors;
}
