#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>
#include <QString>

#include "interface_types.h"
#include "common/client.h"

using namespace std;
using namespace zlaser::thrift;

namespace Ui {
class DialogDeviceManager;
}

class DialogDeviceManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeviceManager(ZlpThriftClient *client_interface = nullptr, QString projectorsn = nullptr, QWidget *parent = 0);
    ~DialogDeviceManager();

private:
    Ui::DialogDeviceManager *ui;
    ZlpThriftClient *m_clientInterface;
    QString m_projectorsn;
    QHash<ProjectorType_t::type, QString> *PROJECTOR_TYPE;
    void setup();
};

#endif // DIALOGSAVE_H
