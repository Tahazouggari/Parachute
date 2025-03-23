#ifndef MESSAGEENCODER_H
#define MESSAGEENCODER_H

#include <vector>
#include <QString>

class MessageEncoder {
public:
    static std::vector<int> encodeMessage(const QString &message, bool mode10 = false);
};

#endif // MESSAGEENCODER_H
