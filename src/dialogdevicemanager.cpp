#include "dialogdevicemanager.h"
#include "ui_dialogdevicemanager.h"

DialogDeviceManager::DialogDeviceManager(ZlpThriftClient *client_interface, QString projectorsn, QWidget *parent) :
    m_clientInterface(client_interface),
    m_projectorsn(projectorsn),
    QDialog(parent),
    ui(new Ui::DialogDeviceManager)
{
    ui->setupUi(this);
    PROJECTOR_TYPE = new QHash<ProjectorType_t::type, QString>();
    PROJECTOR_TYPE->insert( ProjectorType_t::type::UNKNOWN, "UNKNOWN");
    PROJECTOR_TYPE->insert( ProjectorType_t::type::A2F500SOM, "A2F500SOM");
    PROJECTOR_TYPE->insert( ProjectorType_t::type::ZLP1, "ZLP1");
    PROJECTOR_TYPE->insert( ProjectorType_t::type::ZLP2, "ZLP2");
    PROJECTOR_TYPE->insert( ProjectorType_t::type::SIMULATOR,"SIMULATOR");
    QStringList headers;
    headers << "Type" << "Major Version" << "Minor Version" << "Patch Version";

    ui->tableWidget->setColumnCount(headers.count());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionMode( QAbstractItemView::SingleSelection);
    setup();
    this->setWindowFlags(Qt::Dialog);

}

DialogDeviceManager::~DialogDeviceManager()
{
    delete ui;
}

void DialogDeviceManager::setup()
{
    ProjectorInfo_t projector;
    m_clientInterface->getProjectorInfo(projector, m_projectorsn.toStdString());

    // construct list
    ui->tableWidget->setRowCount(6);
    QStringList hwRevision;
    hwRevision << QString("%1").arg(projector.hwRevision.majorVersion)
               << QString("%1").arg(projector.hwRevision.minorVersion)
               << QString("%1").arg(projector.hwRevision.patchVersion);

    QStringList fpgaIp;
    fpgaIp << QString("%1").arg(projector.fpgaIp.majorVersion)
           << QString("%1").arg(projector.fpgaIp.minorVersion)
           << QString("%1").arg(projector.fpgaIp.patchVersion);

    QStringList firmware;
    firmware << QString("%1").arg(projector.firmware.majorVersion)
             << QString("%1").arg(projector.firmware.minorVersion)
             << QString("%1").arg(projector.firmware.patchVersion);
    QStringList lpcom;
    lpcom << QString("%1").arg(projector.lpcom.majorVersion)
          << QString("%1").arg(projector.lpcom.minorVersion)
          << QString("%1").arg(projector.lpcom.patchVersion);

    QString Revision(QString::fromStdString(projector.revision));
    ProjectorType_t::type projectorType = projector.kind;
    // TeachTable_t
    ui->tableWidget->setItem( 0, 0, new QTableWidgetItem("HwRevision"));
    ui->tableWidget->setItem( 1, 0, new QTableWidgetItem("fpgaIp"));
    ui->tableWidget->setItem( 2, 0, new QTableWidgetItem("firmware"));
    ui->tableWidget->setItem( 3, 0, new QTableWidgetItem("lpcom"));

    ui->tableWidget->setItem( 4, 0, new QTableWidgetItem("Revision"));
    ui->tableWidget->setItem( 4, 1, new QTableWidgetItem(Revision));

    ui->tableWidget->setItem( 5, 0, new QTableWidgetItem("projectorType"));
    ui->tableWidget->setItem( 5, 1, new QTableWidgetItem(PROJECTOR_TYPE->value(projectorType)));

    for(int column = 1; column < 4; column++){
        QString item = QString("%1").arg(hwRevision.count() > column ? hwRevision.at(column) : tr("NA"));
        ui->tableWidget->setItem(0, column, new QTableWidgetItem(item));
    }
    for(int column = 1; column < 4; column++){
        QString item = QString("%1").arg(fpgaIp.count() > column ? fpgaIp.at(column) : tr("NA"));
        ui->tableWidget->setItem(1, column, new QTableWidgetItem(item));
    }
    for(int column = 1; column < 4; column++){
        QString item = QString("%1").arg(firmware.count() > column ? firmware.at(column) : tr("NA"));
        ui->tableWidget->setItem(2, column, new QTableWidgetItem(item));
    }
    for(int column = 1; column < 4; column++){
        QString item = QString("%1").arg(lpcom.count() > column ? lpcom.at(column) : tr("NA"));
        ui->tableWidget->setItem(3, column, new QTableWidgetItem(item));
    }

}
