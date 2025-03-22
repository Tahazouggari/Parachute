#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QWidget>
#include <vector>
#include <QColor>
#include <QMap>

class BinaryWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<bool> bitSet; // Stores the binary data
    QColor backgroundColor;   // Background color
    QColor painterColor;      // Color for bits set to 1
    QColor bitOffColor;       // Color for bits set to 0
    bool randomColorMode;     // Mode couleurs aléatoires
    QMap<int, QColor> randomColors; // Couleurs aléatoires pour chaque bit à 1

public:
    explicit BinaryWidget(QWidget *parent = nullptr);

    void updateBitSet(const std::vector<bool> &b); // Update the binary data
    void setBackgroundColor(const QColor &color);  // Set the background color
    void setBitOnColor(const QColor &color);       // Set color for bits set to 1
    void setBitOffColor(const QColor &color);      // Set color for bits set to 0
    void setRandomColorMode(bool enabled);         // Activer/désactiver le mode couleurs aléatoires
    void generateRandomColors();                   // Générer des couleurs aléatoires

protected:
    void paintEvent(QPaintEvent *event) override; // Custom paint event
};

#endif // BINARYWIDGET_H