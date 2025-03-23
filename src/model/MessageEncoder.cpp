#include "model/MessageEncoder.h"

std::vector<int> MessageEncoder::encodeMessage(const QString &message, bool mode10) {
    std::vector<int> binaryMessage;
    
    for (QChar c : message) {
        int asciiValue = c.unicode() - 64; // Décalage pour que '@' soit 0
        
        // Encodage des 7 bits du caractère (identique pour les deux modes)
        for (int i = 6; i >= 0; --i) {
            binaryMessage.push_back((asciiValue >> i) & 1);
        }
        
        // En mode 10, on doitajouter 3 bits blancs (0) après chaque caractère
        if (mode10) {
            binaryMessage.push_back(0);
            binaryMessage.push_back(0);
            binaryMessage.push_back(0);
        }
    }
    return binaryMessage;
}
