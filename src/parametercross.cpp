#include "parametercross.h"
#include <QDebug>

ParameterCross::ParameterCross(const QList<SCANNING_POINT *> refPoints, QObject *parent):
    m_ReferencePoints(refPoints),
    QObject(parent)
{
    qDebug() << "ParameterCross::ParameterCross()";
}

QList<SCANNING_POINT*> ParameterCross::get() const
{
    qDebug() << QString("ParameterCross::get().%1").arg(m_ReferencePoints.count());
    QList<SCANNING_POINT*> _result;
    QList<SCANNING_POINT*>::const_iterator i;
    for(i = m_ReferencePoints.cbegin(); i != m_ReferencePoints.cend(); ++i){
        _result.append(*i);
    }
    return _result;
}

void ParameterCross::setCross(double DX, double DY)
{
    Vector2D crossSize;
    crossSize.__set_x(DX);
    crossSize.__set_y(DY);

    for(int i =0; i < m_ReferencePoints.count(); i++){
        m_ReferencePoints.at(i)->crosssize  = crossSize;
    }
}
