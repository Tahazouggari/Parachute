#ifndef MESSAGEENCODER_H
#define MESSAGEENCODER_H

#include <vector>
#include <QString>

class MessageEncoder {
public:
    static std::vector<int> encodeMessage(const QString &message);
};

#endif // MESSAGEENCODER_H
