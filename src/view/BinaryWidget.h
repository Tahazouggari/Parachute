#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QWidget>
#include <vector>
#include <QColor>

class BinaryWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<bool> bitSet; // Stores the binary data
    QColor backgroundColor;   // Background color
    QColor painterColor;      // Color for bits set to 1

public:
    explicit BinaryWidget(QWidget *parent = nullptr);

    void updateBitSet(const std::vector<bool> &b); // Update the binary data
    void setBackgroundColor(const QColor &color);  // Set the background color

protected:
    void paintEvent(QPaintEvent *event) override; // Custom paint event
};

#endif // BINARYWIDGET_H