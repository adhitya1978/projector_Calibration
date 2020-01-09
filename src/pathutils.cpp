#include "pathutils.h"

#include <QDir>
//#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>


QString PathUtils::combine(const QString &rootdirectory, const QString &bindirectory)
{
    return textutils::rtrim(rootdirectory, '/') + '/' + textutils::ltrim(bindirectory, '/');
}

QString PathUtils::find(const QString &exe)
{
    if (QFile::exists(exe))
    {
        return exe;
    }
    QString bin;
#ifndef QT_VERSION > 0x050000
    bin = QStandardPaths::findExecutable(exe);
#else
    bin = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation).append(exe);
#endif
    if (QFile::exists(bin))
    {
        return bin;
    }

    /*get Z-IO directory*/
    QDir tmp;
    QString ZLPSuite;

#ifdef Q_OS_LINUX
    ZLPSuite = qgetenv("HOME");
    ZLPSuite = PathUtils::combine(ZLPSuite,"/ZLP-Suite/ZLP-Suite-1.1.0-Linux");
#else
    ZLPSuite = "C:/Z-LASER/ZLP-Suite 1.1.0";
#endif
    qDebug() << "PathUtils::find()";
    qDebug() << ZLPSuite;
    if (!ZLPSuite.isEmpty() && (tmp = QDir(PathUtils::combine(ZLPSuite, "bin"))).exists())
    {
        if (QFile::exists(bin = tmp.absoluteFilePath(exe)))
        {
            return bin;
        }
    }

    return exe;
}

QString PathUtils::temp(const QString &name, const QString &dir)
{
    QString prefix;
    const QString &chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 8; i++)
    {
        prefix += chars.at(rand() % chars.length());
    }
    return combine(dir.isEmpty() ? QDir::tempPath() : dir, prefix + '-' + name);
}

QString textutils::ltrim(const QString &str, const QChar &c)
{
    QString t(str);
    while (t.at(0) == c)
    {
        t = t.mid(1);
    }
    return t;
}

QString textutils::rtrim(const QString &str, const QChar &c)
{
    QString t(str);
    int l = t.size() - 1;
    while ((l >= 0) && (t.at(l) == c))
    {
        t = str.left(l);
        l--;
    }
    return t;
}
