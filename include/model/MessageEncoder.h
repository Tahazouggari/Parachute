#ifndef MESSAGEENCODER_H
#define MESSAGEENCODER_H

#include <vector>
#include <QString>

class MessageEncoder {
private:
    static int referenceCharacter; // Le caractère de référence (par défaut '@' qui est 64 en ASCII)

public:
    static std::vector<int> encodeMessage(const QString &message, bool mode10 = false);
    
    // Nouvelles méthodes pour gérer le caractère de référence
    static void setReferenceCharacter(QChar character);
    static QChar getReferenceCharacter();
    static int getReferenceValue();
};

#endif // MESSAGEENCODER_H
