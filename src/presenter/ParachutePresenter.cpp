#include "ParachutePresenter.h"
#include "../view/MainWindow.h"  // Use relative path


ParachutePresenter::ParachutePresenter(MainWindow *view) 
    : view(view) {}

void ParachutePresenter::updateSectors(int sectors) {
    model.setSectors(sectors);
}

void ParachutePresenter::updateTracks(int tracks) {
    model.setTracks(tracks);
}
