#include <QtGui/QApplication>
#include <QTextCodec>
#include "openmahjongclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenMahjongClient w;
    w.show();

    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    return a.exec();
}
