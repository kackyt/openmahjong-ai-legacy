#ifndef FILEPATH_H
#define FILEPATH_H
#include <QCoreApplication>
#include <QFile>
#include <QDir>

static inline QString getFilePath(const QString &relativepath)
{
    QString ret;
    QDir dir = QDir::current();
    QDir dir2;

    ret = QCoreApplication::applicationDirPath() + "/" + relativepath;

    dir2.setPath(ret);

    if(QFile::exists(ret) || dir2.exists()){
        return ret;
    }

    ret = dir.absolutePath() + "/" + relativepath;

    dir2.setPath(ret);

    if(QFile::exists(ret) || dir2.exists()){
        return ret;
    }

    return QString();
}

#endif // FILEPATH_H
