#include "packetreferencepoint.h"
#include <QDebug>


PacketReferencePoint::PacketReferencePoint(Parameters *parameters, QObject *parent):
    m_Parameters(parameters),
    QObject(parent)
{
    qDebug() << "PacketReferencePoint::PacketReferencePoint()";
}

PacketReferencePoint::~PacketReferencePoint()
{
    delete m_Parameters;
}

vector<Referencepoint> PacketReferencePoint::get() const
{
    qDebug() << "PacketReferencePoint::get()";
    vector<Referencepoint> referencePoints;

    Q_FOREACH(Parameter *parameter, m_Parameters->items()){
        switch(parameter->type()){
        case CROSS_MODEL:
        {
            referencePoints.clear();
            ParameterCross *crossmodel = (ParameterCross*) parameter;
            Q_FOREACH(SCANNING_POINT *p, crossmodel->get()){
                Referencepoint refpoint;
                refpoint.__set_name(p->name.toStdString());
                refpoint.__set_refPoint(p->refpoint);
                refpoint.__set_crossSize(p->crosssize);
                refpoint.__set_distance(p->distance);
                refpoint.__set_tracePoint(p->tracepoint);
                refpoint.__set_activated(p->activated);
                referencePoints.push_back(refpoint);
            }
        } break;
        case DISTANCE:
        {
            referencePoints.clear();
            ParameterDistance *distance = (ParameterDistance*) parameter;
            Q_FOREACH(struct SCANNING_POINT *p, distance->get()){
                Referencepoint refpoint;
                refpoint.__set_name(p->name.toStdString());
                refpoint.__set_refPoint(p->refpoint);
                refpoint.__set_crossSize(p->crosssize);
                refpoint.__set_distance(p->distance);
                refpoint.__set_tracePoint(p->tracepoint);
                refpoint.__set_activated(p->activated);
                referencePoints.push_back(refpoint);
            }
        } break;
         case POINT_MODEL:
        {
            referencePoints.clear();
            ParameterTracePoint *pointmodel = (ParameterTracePoint*) parameter;
            Q_FOREACH(SCANNING_POINT *p, pointmodel->get()){
                Referencepoint refpoint;
                refpoint.__set_name(p->name.toStdString());
                refpoint.__set_refPoint(p->refpoint);
                refpoint.__set_crossSize(p->crosssize);
                refpoint.__set_distance(p->distance);
                refpoint.__set_tracePoint(p->tracepoint);
                refpoint.__set_activated(p->activated);
                referencePoints.push_back(refpoint);
            }
        }break;
        }
    }
    return referencePoints;
}

