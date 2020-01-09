#include "dialogfilename.h"
#include "ui_dialogfilename.h"
#include "dialogcoordinatesystems.h"

DialogFileName::DialogFileName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFileName)
{
    ui->setupUi(this);
    this->setWindowTitle("Create new coordinate system:");
}

DialogFileName::~DialogFileName()
{
    delete ui;
}

void DialogFileName::on_buttonBox_accepted()
{
    emit this->set_new_name_cordinatesystem(ui->lineEdit->text().trimmed(), ui->lineEdit_2->text().trimmed());
    this->close();
}

void DialogFileName::on_buttonBox_rejected()
{
    this->close();
}
