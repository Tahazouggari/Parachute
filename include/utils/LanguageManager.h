#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>

class LanguageManager : public QObject {
    Q_OBJECT

private:
    QTranslator translator; 
    QString currentLanguage; 

signals:
    void languageChanged(); 

public:
    explicit LanguageManager(QObject *parent = nullptr);

    void switchLanguage(const QString &languageCode); 
    QString getCurrentLanguage() const; 
};

#endif // LANGUAGEMANAGER_H
