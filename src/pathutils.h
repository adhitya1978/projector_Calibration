#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <QString>
#include <QtGlobal>

class PathUtils
{
public:
    static QString combine(const QString &rootdirectory, const QString &bindirectory);
    static QString find(const QString &exe);
    static QString temp(const QString &name, const QString &dir );
};

class textutils
{
public:
    static QString ltrim(const QString& str, const QChar& c = QChar::Separator_Space);
    static QString rtrim(const QString& str, const QChar& c = QChar::Separator_Space);
};

#endif // PATHUTILS_H
