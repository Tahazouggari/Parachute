#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QWidget>
#include <QString>
#include <QColor>

class HexView : public QWidget {
    Q_OBJECT

private:
    QString hexData; 
    QColor backgroundColor; 
    QColor textColor;

public:
    explicit HexView(QWidget *parent = nullptr);

    void setHexData(const QString &data); 
    void setBackgroundColor(const QColor &color);
    void setTextColor(const QColor &color); 

protected:
    void paintEvent(QPaintEvent *event) override; 
};

#endif // HEXVIEW_H
