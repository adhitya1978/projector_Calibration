#ifndef PARAMETERTRACEPOINT_H
#define PARAMETERTRACEPOINT_H

#include <QObject>

//! SDK
#include "interface_types.h"
#include "common/client.h"

#include "outbondpacket.h"
#include "parameter.h"

using namespace std;
using namespace zlaser::thrift;

class ParameterTracePoint : public QObject, public Parameter
{
    Q_OBJECT
public:
    explicit ParameterTracePoint(const QList<SCANNING_POINT*>  &refPoints, QObject *parent = nullptr);
    ~ParameterTracePoint();

    void setTracePoint(double DX, double DY);

    QList<SCANNING_POINT*> get() const;

    QString get_parameter_name() { return "Tracepoint";   }

    PARAMETER_TYPE type() { return POINT_MODEL;  }

private:
    QList<SCANNING_POINT*> m_ReferencePoints;

signals:
    //! auto update while changed

public slots:


};

#endif // PARAMETERTRACEPOINT_H
