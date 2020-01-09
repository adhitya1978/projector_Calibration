#include "packetpolyline.h"
#include <QDebug>

PacketPolyline::PacketPolyline(const PointIndentifier *pointIndentifier,
                               QString groupname,
                               QString elementname,
                               QObject *parent) :
    m_elementname(elementname),                 //! element name
    m_Type(POLYLINE),                           //! type element
    m_groupname(groupname),                     //! which group
    m_pointIndentifier(pointIndentifier),       //! parameter element
    m_Polyline(new zlaser::thrift::Polyline),   //! constructor new element
    QObject(parent)
{
    qDebug() << "PacketPolyline::PacketPolyline()";
    ConstructPolyline();
}

PacketPolyline::~PacketPolyline()
{
    delete m_Polyline;
    delete m_pointIndentifier;
}

Polyline PacketPolyline::get() const
{
    return *m_Polyline;
}

bool PacketPolyline::setCoordinateSystem(const QString coordname)
{
    std::vector<std::string> _coordinatesystems;
    //! assign new one
    _coordinatesystems.push_back(coordname.toStdString());
    m_Polyline->__set_coordinateSystemList(_coordinatesystems);
    return true;
}

QString PacketPolyline::get_projection_name()
{
    return m_elementname;
}

bool PacketPolyline::get_activated()
{
    return m_Polyline->activated;
}

QList<QString> PacketPolyline::get_coordinateSystemList()
{
    QList<QString> _result;
    for(std::string coordname: m_Polyline->coordinateSystemList){
        _result.append(QString::fromStdString(coordname));
    }
    return _result;
}

PROJECTION_TYPE PacketPolyline::type()
{
    return m_Type;
}

void PacketPolyline::ConstructPolyline()
{
    vector<Vector3D> lineleft, linetop, lineright, linebottom;
    vector<vector<Vector3D>> multiLineString;
    lineleft.push_back(m_pointIndentifier[0].m_point);
    lineleft.push_back(m_pointIndentifier[1].m_point);

    linetop.push_back(m_pointIndentifier[1].m_point);
    linetop.push_back(m_pointIndentifier[2].m_point);

    lineright.push_back(m_pointIndentifier[2].m_point);
    lineright.push_back(m_pointIndentifier[3].m_point);

    linebottom.push_back(m_pointIndentifier[3].m_point);
    linebottom.push_back(m_pointIndentifier[0].m_point);

    multiLineString.push_back(lineleft);
    multiLineString.push_back(linetop);
    multiLineString.push_back(lineright);
    multiLineString.push_back(linebottom);
    m_Polyline->__set_activated(true);
    m_Polyline->__set_polylineList(multiLineString);
    m_groupname.append("/");
    //m_groupname.append(m_elementname);
    qDebug() << m_groupname;
    m_Polyline->__set_name(m_groupname.toStdString());

}
