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
    int originalSectors;      
    std::vector<int> encodedMessage;
    QColor backgroundColor;
    QColor parachuteColor;
    QColor sectorColor;      
    bool randomColorMode;    
    QMap<int, QColor> randomColors; 
    bool mode10;             
    
    QPixmap backgroundImage; 
    bool useBackgroundImage;  

public:
    explicit ParachuteView(QWidget *parent = nullptr);
    void setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage);
    void setBackgroundColor(QColor color);
    void setParachuteColor(QColor color);
    void setSectorColor(QColor color);  
    void setRandomColorMode(bool enabled); 
    void setMode10(bool enabled);       
    void generateRandomColors(); 
    void saveParachuteImage(const QString &filename);
    
    
    void setBackgroundImage(const QString &imagePath);
    void clearBackgroundImage();
    bool hasBackgroundImage() const;
    
    
    int getOriginalSectors() const; 

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PARACHUTEVIEW_H