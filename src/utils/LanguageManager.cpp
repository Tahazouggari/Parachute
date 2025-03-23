#include "utils/LanguageManager.h"
#include <QDebug> // Add this header for qDebug()

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent), currentLanguage("en") {}

void LanguageManager::switchLanguage(const QString &languageCode) {
    if (currentLanguage == languageCode) {
        return; // No need to switch if the language is already active
    }

    qApp->removeTranslator(&translator);

    // Use a relative path to the translations directory
    QString translationFile = "../resources/translations/" + languageCode + ".qm";
    qDebug() << "Attempting to load translation file:" << translationFile;

    if (translator.load(translationFile)) {
        qApp->installTranslator(&translator);
        currentLanguage = languageCode;
        emit languageChanged();
        qDebug() << "Loaded translation file:" << translationFile;
    } else {
        qWarning() << "Failed to load translation file:" << translationFile;
    }
}

QString LanguageManager::getCurrentLanguage() const {
    return currentLanguage;
}
