#ifndef PARACHUTEVIEW_H
#define PARACHUTEVIEW_H

#include <QWidget>
#include <vector>
#include <QColor>

class ParachuteView : public QWidget {
    Q_OBJECT

private:
    int sectors;
    int tracks;
    std::vector<int> encodedMessage;
    QColor backgroundColor;
    QColor parachuteColor;

public:
    explicit ParachuteView(QWidget *parent = nullptr);
    void setParachuteData(int sectors, int tracks, const std::vector<int>& encodedMessage);
    void setBackgroundColor(QColor color);
    void setParachuteColor(QColor color);
    void saveParachuteImage(const QString &filename);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PARACHUTEVIEW_H
