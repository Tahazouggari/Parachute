#ifndef MESSAGEENCODER_H
#define MESSAGEENCODER_H

#include <QString>
#include <vector>

class MessageEncoder {
public:
    static std::vector<int> encodeMessage(const QString &message);
};

#endif // MESSAGEENCODER_H
