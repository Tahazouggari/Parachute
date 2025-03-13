#include "MessageEncoder.h"

std::vector<int> MessageEncoder::encodeMessage(const QString &message) {
    std::vector<int> binaryMessage;
    
    for (QChar c : message) {
        int asciiValue = c.unicode() - 64; // Décalage pour que '@' soit 0
        for (int i = 6; i >= 0; --i) {
            binaryMessage.push_back((asciiValue >> i) & 1);
        }
    }
    return binaryMessage;
}
