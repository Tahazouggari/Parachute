#include "MessageEncoder.h"

std::vector<int> MessageEncoder::encodeMessage(const QString &message) {
    std::vector<int> encodedMessage;
    for (QChar c : message) {
        int asciiValue = c.unicode();
        for (int i = 6; i >= 0; --i) {
            encodedMessage.push_back((asciiValue >> i) & 1);
        }
    }
    return encodedMessage;
}
