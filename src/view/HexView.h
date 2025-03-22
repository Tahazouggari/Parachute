#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <QWidget>
#include <QString>
#include <QColor>

class HexView : public QWidget {
    Q_OBJECT

private:
    QString hexData; // Stores the hexadecimal representation
    QColor backgroundColor; // Background color
    QColor textColor; // Text color

public:
    explicit HexView(QWidget *parent = nullptr);

    void setHexData(const QString &data); // Update the hexadecimal data
    void setBackgroundColor(const QColor &color); // Set the background color
    void setTextColor(const QColor &color); // Set the text color

protected:
    void paintEvent(QPaintEvent *event) override; // Custom paint event
};

#endif // HEXVIEW_H
