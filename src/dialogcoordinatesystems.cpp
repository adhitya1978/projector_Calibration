#include "dialogcoordinatesystems.h"
#include "ui_dialogcoordinatesystems.h"
#include "log_widget.h"

#include <QList>
#include <QDebug>

DialogCoordinateSystems::DialogCoordinateSystems(QHash<QString, QString> referenceObjectNames, QWidget *parent) :
    m_referenceobjects(referenceObjectNames),
    QDialog(parent),
    ui(new Ui::DialogCoordinateSystems)
{
    qDebug() << "DialogCoordinateSystems::DialogCoordinateSystems()";
    for(int i = 0; i < referenceObjectNames.count(); i++)
        qDebug() << tr("key:%1").arg(referenceObjectNames.keys().at(i)) ;
    ui->setupUi(this);

    //! construct header table widget
    QStringList headers;
    headers << "Coordinate system name" << "Reference name";

    ui->tableWidget->setColumnCount(headers.count());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    this->constructTableWidget();
    this->setWindowTitle("available coordinate system");

}

DialogCoordinateSystems::~DialogCoordinateSystems()
{
    delete ui;
}

bool DialogCoordinateSystems::exist(const QString coordinatename)
{
    QList<QTableWidgetItem *> coordinatesystems = ui->tableWidget->findItems(coordinatename, Qt::MatchExactly);
    //! coordinate name
    for(QTableWidgetItem *item : coordinatesystems){
        if(item->text().toUpper() == coordinatename.toUpper())
                return true;
    }
    return false;
}

void DialogCoordinateSystems::setNewCordinateName(const QString newname)
{
    new_filename = newname;
}
//!
//! \brief DialogCoordinateSystems::constructTableWidget
//!
void DialogCoordinateSystems::constructTableWidget()
{
    //! set total row
    ui->tableWidget->setRowCount(m_referenceobjects.count());
    for (int row=0; row < m_referenceobjects.count(); row++)
    {
        QString coordSystem = QString("%1").arg(m_referenceobjects.keys().at(row));
        QString refObject = QString("%1").arg(m_referenceobjects.values().at(row));
        ui->tableWidget->setItem( row, 0, new QTableWidgetItem(coordSystem));
        ui->tableWidget->setItem( row, 1, new QTableWidgetItem(refObject));
    }
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    if(ui->tableWidget->rowCount() > -1)
        ui->tableWidget->selectRow(0);
}

void DialogCoordinateSystems::update()
{
}

void DialogCoordinateSystems::deleteItem()
{
    qDebug() << "deleteItem()";
    //! emit remove reference object
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    if(items.isEmpty()) return;
    qDebug() << items.at(0)->text();
    qDebug() << items.at(1)->text();
    emit signalRemoveReferenceObject(items.at(0)->text(), items.at(1)->text());

    //! \remove widget
    //!
    QModelIndexList index;
    if((index =ui->tableWidget->selectionModel()->selectedRows()).count() != 0){
        emit ui->tableWidget->removeRow(index.at(0).row()); // remove selected widget item
    }
}

void DialogCoordinateSystems::openitem(const QString &coordname, const QString &refname)
{
   emit signalLoadReferenceObject(coordname, refname);
}
//!
//! \brief SLOT for inserting new coordinate system or Referenceobject in the widget
//! \param string name coordname
//! \param string name Referenceobject
//!
void DialogCoordinateSystems::set_new_name_cordinatesystem(const QString &coordname, const QString &refname)
{
    bool exist = this->exist(coordname);
    if(exist){
        DPRINT(0, QString("Coordinate system name already exist!.."));
        return;
    }
    //! new row
    int new_row = ui->tableWidget->rowCount();

    qDebug() << new_row;

    QStringList new_item;
    new_item << coordname << refname;

    ui->tableWidget->insertRow(new_row);
    //ui->tableWidget->setRowCount(new_row);

    ui->tableWidget->setItem(new_row, 0, new QTableWidgetItem(coordname));
    ui->tableWidget->setItem(new_row, 1, new QTableWidgetItem(refname));


    //! item select on new item
    ui->tableWidget->selectRow(new_row);

}

void DialogCoordinateSystems::on_listWidget_itemClicked(QListWidgetItem *item)
{
   // m_selectitem = item;
}

void DialogCoordinateSystems::on_pushButton_del_clicked()
{
    this->deleteItem();
}

void DialogCoordinateSystems::on_pushButton_new_clicked()
{
    this->dialogFilename = new DialogFileName(this);
    connect(dialogFilename, SIGNAL(set_new_name_cordinatesystem(QString,QString)), this, SLOT(set_new_name_cordinatesystem(QString,QString)));
    this->dialogFilename->exec();
}

void DialogCoordinateSystems::on_pushButton_open_clicked()
{
    QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
    this->openitem(items.at(0)->text(), items.at(1)->text());
    this->close();
}

void DialogCoordinateSystems::on_pushButton_cancel_clicked()
{
    this->close();
}
