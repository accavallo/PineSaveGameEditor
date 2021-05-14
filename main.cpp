#include "PineSaveGameEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PineSaveGameEditor w;
    w.show();
    return a.exec();
}
