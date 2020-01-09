#ifndef SWITH_BUTTON_H
#define SWITH_BUTTON_H

#include <QObject>
#include <QPushButton>
#include <QPainter>

#include "global.h"

class swith_button : public QPushButton
{
    Q_OBJECT

public:
    explicit swith_button(const QString & name, QWidget *parent = nullptr);
    virtual ~swith_button();

    void *set_symbol();

private:
    // QWidget interface
};

#endif // SWITH_BUTTON_H
