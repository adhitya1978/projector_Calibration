#ifndef Z_IO_H
#define Z_IO_H

#include <QObject>

#include "interfaceextexe.h"

#define RESOURCES_DIRECTORY     "../resource"

class z_io:public InterfaceExtExe
{
    Q_OBJECT
public:
    z_io(QObject *parent = nullptr);

    static z_io *get();
    Result Executing(const QString &input_file, const QStringList &arg);
};

#endif // Z_IO_H
