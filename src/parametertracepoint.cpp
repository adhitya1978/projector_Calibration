#include "parametertracepoint.h"

ParameterTracePoint::ParameterTracePoint(const QList<SCANNING_POINT *> &refPoints, QObject *parent) :
    m_ReferencePoints(refPoints),
    QObject(parent)
{
}

ParameterTracePoint::~ParameterTracePoint()
{
}

void ParameterTracePoint::setTracePoint(double DX, double DY)
{
    Point2D AreaSize;
    AreaSize.__set_x(DX);
    AreaSize.__set_y(DY);

    for(int i =0; i < m_ReferencePoints.count(); i++){
        m_ReferencePoints.at(i)->tracepoint = AreaSize; // update areasize
    }
}

QList<SCANNING_POINT *> ParameterTracePoint::get() const
{
    QList<SCANNING_POINT*> _result;
    QList<SCANNING_POINT*>::const_iterator i;
    for(i = m_ReferencePoints.cbegin(); i != m_ReferencePoints.cend(); ++i){
        _result.append(*i);
    }
    return _result;
}
