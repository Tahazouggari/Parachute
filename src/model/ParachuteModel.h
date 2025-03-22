#ifndef PARACHUTEMODEL_H
#define PARACHUTEMODEL_H

#include <vector>
#include <QColor>
#include <QMap>

class ParachuteModel {
private:
    int sectors;
    int tracks;
    std::vector<int> encodedMessage;
    QColor backgroundColor;
    QColor parachuteColor;     // Couleur des bits à 1
    QColor sectorColor;        // Couleur des bits à 0
    bool randomColorMode;      // Mode couleurs aléatoires
    QMap<int, QColor> randomColors; // Couleurs aléatoires

public:
    ParachuteModel(int sectors = 7, int tracks = 5);
    void setMessage(const std::vector<int> &message);
    void setSectors(int s);
    void setTracks(int t);
    int getSectors() const;
    int getTracks() const;
    std::vector<int> getEncodedMessage() const;
    
    // Nouvelles méthodes pour la gestion des couleurs
    void setBackgroundColor(const QColor &color);
    void setParachuteColor(const QColor &color);
    void setSectorColor(const QColor &color);
    void setRandomColorMode(bool enabled);
    void generateRandomColors();
    
    QColor getBackgroundColor() const;
    QColor getParachuteColor() const;
    QColor getSectorColor() const;
    bool getRandomColorMode() const;
    QMap<int, QColor> getRandomColors() const;
};

#endif // PARACHUTEMODEL_H
