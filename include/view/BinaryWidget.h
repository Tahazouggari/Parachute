#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QWidget>
#include <vector>
#include <QColor>
#include <QMap>

class BinaryWidget : public QWidget {
    Q_OBJECT

private:
    std::vector<bool> bitSet; 
    QColor backgroundColor;  
    QColor painterColor;      
    QColor bitOffColor;       
    bool randomColorMode;     
    QMap<int, QColor> randomColors; 

public:
    explicit BinaryWidget(QWidget *parent = nullptr);

    void updateBitSet(const std::vector<bool> &b); 
    void setBackgroundColor(const QColor &color);  
    void setBitOnColor(const QColor &color);      
    void setBitOffColor(const QColor &color);      
    void setRandomColorMode(bool enabled);         
    void generateRandomColors();                  

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // BINARYWIDGET_H