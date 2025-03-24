#include "utils/LanguageManager.h"
#include <QDebug>
#include <QCoreApplication>

LanguageManager::LanguageManager(QObject *parent)
    : QObject(parent), currentLanguage("en") {}

void LanguageManager::switchLanguage(const QString &languageCode) {
    if (currentLanguage == languageCode) {
        return; 
    }

    // Supprimer le traducteur actuel
    qApp->removeTranslator(&translator);
    
    // Au lieu de réinitialiser le traducteur (qui n'est pas possible car Q_DISABLE_COPY),
    // assurons-nous simplement qu'il n'y a pas de résidus des traductions précédentes

    QString translationFile = "../resources/translations/" + languageCode + ".qm";
    qDebug() << "Attempting to load translation file:" << translationFile;

    if (translator.load(translationFile)) {
        // Installer le nouveau traducteur
        qApp->installTranslator(&translator);
        currentLanguage = languageCode;
        
        // Force l'application à traiter les événements pour s'assurer que les traductions sont appliquées
        QCoreApplication::processEvents();
        
        // Signaler le changement de langue
        emit languageChanged();
        qDebug() << "Loaded translation file:" << translationFile;
    } else {
        qWarning() << "Failed to load translation file:" << translationFile;
        
        // En cas d'échec, utiliser l'anglais par défaut
        if (languageCode != "en") {
            switchLanguage("en");
        }
    }
}

QString LanguageManager::getCurrentLanguage() const {
    return currentLanguage;
}
