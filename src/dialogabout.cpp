#include "dialogabout.h"
#include "ui_dialogabout.h"

Dialogabout::Dialogabout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogabout)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup);
}

Dialogabout::~Dialogabout()
{
    delete ui;
}

void Dialogabout::on_buttonBox_accepted()
{
    this->close();
}
