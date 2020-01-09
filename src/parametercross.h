#ifndef PARAMETERCROSS_H
#define PARAMETERCROSS_H

#include <QObject>

//! SDK
#include "interface_types.h"
#include "common/client.h"

#include "outbondpacket.h"
#include "parameter.h"

using namespace std;
using namespace zlaser::thrift;

class ParameterCross : public QObject, public Parameter
{
    Q_OBJECT
public:
    explicit ParameterCross(const QList<SCANNING_POINT *> refPoints, QObject *parent = nullptr);

    QList<SCANNING_POINT*> get() const;

    void setCross(double DX, double DY);

    QString get_parameter_name() {return "CrosSize";}

    PARAMETER_TYPE type() { return CROSS_MODEL; }

private:
    QList<SCANNING_POINT*>  m_ReferencePoints;



signals:

public slots:

};

#endif // PARAMETERCROSS_H
