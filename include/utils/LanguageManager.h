#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>

class LanguageManager : public QObject {
    Q_OBJECT

private:
    QTranslator translator; // Translator object
    QString currentLanguage; // Current language code

signals:
    void languageChanged(); // Signal to notify when the language changes

public:
    explicit LanguageManager(QObject *parent = nullptr);

    void switchLanguage(const QString &languageCode); // Switch language
    QString getCurrentLanguage() const; // Get current language code
};

#endif // LANGUAGEMANAGER_H
