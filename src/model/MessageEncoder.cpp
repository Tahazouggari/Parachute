#include "model/MessageEncoder.h"

int MessageEncoder::referenceCharacter = 64;

std::vector<int> MessageEncoder::encodeMessage(const QString &message, bool mode10) {
    std::vector<int> binaryMessage;
    
    for (QChar c : message) {
        int asciiValue = c.unicode() - referenceCharacter; 
        
        for (int i = 6; i >= 0; --i) {
            binaryMessage.push_back((asciiValue >> i) & 1);
        }
        
    
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
