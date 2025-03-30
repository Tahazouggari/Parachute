#include "utils/LanguageManager.h"
#include <QDebug>
#include <QCoreApplication>

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent), currentLanguage("en") {}

void LanguageManager::switchLanguage(const QString &languageCode) {
    if (currentLanguage == languageCode) {
        return; 
    }


    qApp->removeTranslator(&translator);
    


    QString translationFile = "../resources/translations/" + languageCode + ".qm";
    qDebug() << "Attempting to load translation file:" << translationFile;

    if (translator.load(translationFile)) {
        qApp->installTranslator(&translator);
        currentLanguage = languageCode;
        
        QCoreApplication::processEvents();
        
        emit languageChanged();
        qDebug() << "Loaded translation file:" << translationFile;
    } else {
        qWarning() << "Failed to load translation file:" << translationFile;
        
        if (languageCode != "en") {
            switchLanguage("en");
        }
    }
}

QString LanguageManager::getCurrentLanguage() const {
    return currentLanguage;
}
