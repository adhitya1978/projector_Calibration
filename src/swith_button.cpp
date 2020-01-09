#include "swith_button.h"


swith_button::swith_button(const QString &name, QWidget *parent) :
    QPushButton(parent)
{
    this->setStyleSheet("background-color: none;  ");
    this->setText(name);
}

swith_button::~swith_button()
{
}

void *swith_button::set_symbol( )
{
    QPainter *Paint = new QPainter(this);
    QRect rect(10, 10, 10, 10);

    Paint->setPen(Qt::black);

    Paint->translate(50, 50);
    Paint->rotate(60.0);
    Paint->scale(0.6, 0.9);
    Paint->translate(-50, -50);

    Paint->drawRect(rect);


    Paint->setRenderHint(QPainter::Antialiasing, false);
    Paint->setPen(palette().dark().color());
    Paint->setBrush(Qt::NoBrush);
    Paint->drawRect(QRect(0, 0, width() - 1, height() - 1));

}




