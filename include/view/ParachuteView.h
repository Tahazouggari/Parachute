#ifndef PARACHUTEVIEW_H
#define PARACHUTEVIEW_H

#include <QWidget>
#include <vector>
#include <QColor>
#include <QMap>
#include <QRandomGenerator>
#include <QPixmap>

class ParachuteView : public QWidget {
    Q_OBJECT

private:
    int sectors;
    int tracks;
    int originalSectors;      // Stocke le nombre original de secteurs avant mode 10
    std::vector<int> encodedMessage;
    QColor backgroundColor;
    QColor parachuteColor;
    QColor sectorColor;      // Couleur des secteurs (bits à 0)
    bool randomColorMode;    // Mode couleurs aléatoires pour les bits à 1
    QMap<int, QColor> randomColors; // Stocke des couleurs aléatoires pour chaque secteur actif
    bool mode10;             // Mode 10 bits par caractère
    
    QPixmap backgroundImage;  // Image d'arrière-plan
    bool useBackgroundImage;  // Indique si l'image d'arrière-plan est utilisée

public:
    explicit ParachuteView(QWidget *parent = nullptr);
    void setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage);
    void setBackgroundColor(QColor color);
    void setParachuteColor(QColor color);
    void setSectorColor(QColor color);  // Nouvelle méthode pour définir la couleur des secteurs (bits à 0)
    void setRandomColorMode(bool enabled); // Active/désactive le mode couleurs aléatoires
    void setMode10(bool enabled);       // Active/désactive le mode 10 bits par caractère
    void generateRandomColors(); // Génère de nouvelles couleurs aléatoires
    void saveParachuteImage(const QString &filename);
    
    // Méthodes pour l'image d'arrière-plan
    void setBackgroundImage(const QString &imagePath);
    void clearBackgroundImage();
    bool hasBackgroundImage() const;
    
    // Getters
    int getOriginalSectors() const; // Renvoie le nombre original de secteurs

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PARACHUTEVIEW_H