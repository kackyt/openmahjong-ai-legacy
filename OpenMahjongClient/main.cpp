#include <QtGui/QApplication>
#include "openmahjongclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenMahjongClient w;
    w.show();

    return a.exec();
}
