#include <QApplication>
#include "view/MainWindow.h"
#include "utils/LanguageManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
