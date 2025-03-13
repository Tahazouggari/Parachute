#ifndef PARACHUTEMODEL_H
#define PARACHUTEMODEL_H

#include <vector>

class ParachuteModel {
private:
    int sectors;
    int tracks;
    std::vector<int> encodedMessage;

public:
    ParachuteModel(int sectors = 7, int tracks = 5);
    void setMessage(const std::vector<int> &message);
    void setSectors(int s);
    void setTracks(int t);
    int getSectors() const;
    int getTracks() const;
    std::vector<int> getEncodedMessage() const;
};

#endif // PARACHUTEMODEL_H
