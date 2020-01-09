#ifndef INTERFACEEXTEXE_H
#define INTERFACEEXTEXE_H

#include <QObject>
#include <QRegExp>
#include <QString>
#include <QStringList>

#define REGEX_JAVA_VERSION "^.*\"(\\d+)\\.(\\d+)\\.(\\d+)_(\\d+)\"$"
#define REGEX_LF "[\\r\\n]"
#define CLI_TIMEOUT 5 * 60 * 1000

class InterfaceExtExe : public QObject
{
    Q_OBJECT
private:
    QString _exe;
    QRegExp _lf;

public:
    explicit InterfaceExtExe(const QString &exe,QObject *parent = nullptr);

    struct Result
    {
        int code;
        QStringList error;
        QStringList output;
    };
    inline Result exec(const QString &arg) { return exec(QStringList(arg)); }
    virtual Result exec(const QStringList &args = QStringList());

Q_SIGNALS:
    void executed(Result);
    void executing(QString, QStringList);


public slots:
};

Q_DECLARE_METATYPE(InterfaceExtExe::Result)

#endif // INTERFACEEXTEXE_H
