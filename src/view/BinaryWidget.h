#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QWidget>
#include <vector>
#include <QColor>

class BinaryWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<bool> bitSet; // Stores the binary data
    int numberLetterDraw;     // Number of letters to draw
    int bitByLetter;          // Number of bits per letter
    QColor backgroundColor;   // Background color
    QColor painterColor;      // Color for bits set to 1

public:
    explicit BinaryWidget(QWidget *parent = nullptr);

    void updateBitSet(const std::vector<bool> &b); // Update the binary data
    void setNumberLetterDraw(int number);         // Set the number of letters to draw
    void setBitByLetter(int bit);                 // Set the number of bits per letter
    void setBackgroundColor(const QColor &color);            // Set the background color
    void setPainterColor(QColor p);               // Set the color for bits set to 1

protected:
    void paintEvent(QPaintEvent *event) override; // Custom paint event
};

#endif // BINARYWIDGET_H