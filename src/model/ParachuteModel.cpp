#include "ParachuteModel.h"

ParachuteModel::ParachuteModel(int sectors, int tracks) 
    : sectors(sectors), tracks(tracks) {}

void ParachuteModel::setMessage(const std::vector<int> &message) {
    encodedMessage = message;
}

void ParachuteModel::setSectors(int s) {
    sectors = s;
}

void ParachuteModel::setTracks(int t) {
    tracks = t;
}

int ParachuteModel::getSectors() const {
    return sectors;
}

int ParachuteModel::getTracks() const {
    return tracks;
}

std::vector<int> ParachuteModel::getEncodedMessage() const {
    return encodedMessage;
}
