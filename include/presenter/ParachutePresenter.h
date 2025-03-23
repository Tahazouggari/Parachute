#ifndef PARACHUTEPRESENTER_H
#define PARACHUTEPRESENTER_H

#include "../model/ParachuteModel.h"

class MainWindow; 

class ParachutePresenter {
private:
    ParachuteModel model;
    MainWindow *view;

public:
    explicit ParachutePresenter(MainWindow *view);
    void updateSectors(int sectors);
    void updateTracks(int tracks);
};

#endif // PARACHUTEPRESENTER_H
