#ifndef PARAMETERDISTANCE_H
#define PARAMETERDISTANCE_H

/* class for manage distance projector to object
 * paramer referencepoint
 * */
#include <QObject>
#include <QList>

//! SDK
#include "interface_types.h"
#include "common/client.h"

#include "outbondpacket.h"
#include "parameter.h"

using namespace std;
using namespace zlaser::thrift;

class ParameterDistance : public QObject, public Parameter
{
    Q_OBJECT
public:
    explicit ParameterDistance(const QList<SCANNING_POINT *> &refPoints, QObject *parent = nullptr);
    ~ParameterDistance();

    QList<SCANNING_POINT*> get() const;

    void set(double currdist);

    QString get_parameter_name() {      return "Distance";    }

    PARAMETER_TYPE type() {     return DISTANCE;    }

private:
    QList<SCANNING_POINT*>  m_ReferencePoints;



signals:

public slots:

};

#endif // PARAMETERDISTANCE_H
