#include "model/MessageEncoder.h"

// Initialisation du caractère de référence à '@' (64 en ASCII)
int MessageEncoder::referenceCharacter = 64;

std::vector<int> MessageEncoder::encodeMessage(const QString &message, bool mode10) {
    std::vector<int> binaryMessage;
    
    for (QChar c : message) {
        int asciiValue = c.unicode() - referenceCharacter; // pour utiliser le caractère de référence configurable
        
        // Encodage des 7 bits du caractère (identique pour les deux modes)
        for (int i = 6; i >= 0; --i) {
            binaryMessage.push_back((asciiValue >> i) & 1);
        }
        
        // En mode 10, on doit ajouter 3 bits blancs (0) après chaque caractère
        if (mode10) {
            binaryMessage.push_back(0);
            binaryMessage.push_back(0);
            binaryMessage.push_back(0);
        }
    }
    return binaryMessage;
}

void MessageEncoder::setReferenceCharacter(QChar character) {
    referenceCharacter = character.unicode();
}

QChar MessageEncoder::getReferenceCharacter() {
    return QChar(referenceCharacter);
}

int MessageEncoder::getReferenceValue() {
    return referenceCharacter;
}
