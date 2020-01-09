#include "parameterdistance.h"

#include <QDebug>

//! need previous distance
ParameterDistance::ParameterDistance(const QList<SCANNING_POINT *> &refPoints, QObject *parent) :
    m_ReferencePoints(refPoints),
    QObject(parent)
{
    qDebug() << "ParameterDistance::ParameterDistance()";
}

ParameterDistance::~ParameterDistance()
{
    delete &m_ReferencePoints;
}

QList<SCANNING_POINT *> ParameterDistance::get() const
{
    qDebug() << QString("ParameterDistance::get() count:%1").arg( m_ReferencePoints.count());
    return m_ReferencePoints;
}

void ParameterDistance::set(double currdist)
{
    double _distance = m_ReferencePoints.at(0)->distance + currdist;

    for(int i =0; i < m_ReferencePoints.count(); i++){
        qDebug() << QString("ParameterDistance::set() name:%1").arg( m_ReferencePoints.at(i)->name);
        m_ReferencePoints.at(i)->distance = _distance;
    }
}

