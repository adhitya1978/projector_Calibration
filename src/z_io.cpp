#include "z_io.h"
#include <QStringList>

z_io::z_io(QObject *parent)
#ifdef Q_OS_LINUX
    :   InterfaceExtExe("zio.sh", parent)
#else
    :   InterfaceExtExe("z-io.exe", parent)
#endif
{

}

InterfaceExtExe::Result z_io::Executing(const QString &input_file, const QStringList &arg)
{
    QStringList args;
    args << "a";
    args << input_file;
    args << "-r";
    args << arg;

    return exec(args);
}
