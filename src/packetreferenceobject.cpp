#include "packetreferenceobject.h"
#include <QDebug>
#include <QRect>
#include <QPainter>
//!
//! constructor
//! \brief PacketReferenceObject::PacketReferenceObject
//! \param referencelist
//! \param projectorid
//! \param coordinatename
//! \param isactive
//!
PacketReferenceObject::PacketReferenceObject(vector<Referencepoint> referencelist,
                                             const QString projectorid,
                                             const QString coordinatename,
                                             bool isactive):
    m_referenceobject(new Referenceobject),
    m_projectorid(projectorid),
    m_coordinatename(coordinatename)
{
    qDebug() << "PacketReferenceObject::PacketReferenceObject";
    qDebug() << coordinatename;
    m_referenceobject->__set_coordinateSystem(coordinatename.toStdString());
    qDebug() << projectorid;
    m_referenceobject->__set_projectorID(projectorid.toStdString());
    qDebug() << isactive;
    m_referenceobject->__set_activated(isactive);
    qDebug() << QString::fromStdString(referencelist.data()->name);
    m_referenceobject->__set_refPointList(referencelist);
}

//!
//! \brief destructor
//!
PacketReferenceObject::~PacketReferenceObject()
{
    qDebug() << "PacketReferenceObject::~PacketReferenceObject()";
    delete m_referenceobject;
}

//!
//! \brief change referencepoint
//! \param Referencepoint
//!
void PacketReferenceObject::changereferencepoint(const Referencepoint &point)
{
    vector<Referencepoint>::const_iterator i;
    for(i = m_referenceobject->refPointList.cbegin(); i != m_referenceobject->refPointList.cend(); ++i){
        if(*i == point){
            m_referenceobject->refPointList.erase(i);
            m_referenceobject->refPointList.push_back(point);
        }
    }

}

//!
//! \brief add referencepoint to list
//! \param Referencepoint
//!
void PacketReferenceObject::addreferencepoint(const Referencepoint &point)
{
    m_referenceobject->refPointList.push_back(point);
}

//!
//! \brief remove Reverencepoint
//! \param Referencepoint
//!
void PacketReferenceObject::removereverencepoint(const Referencepoint &point)
{
    vector<Referencepoint>::const_iterator i;
    for(i = m_referenceobject->refPointList.cbegin(); i != m_referenceobject->refPointList.cend(); ++i){
        if(*i == point){
            m_referenceobject->refPointList.erase(i);
        }
    }
}

//!
//! \brief clear list
//!
void PacketReferenceObject::clear()
{
    m_referenceobject->refPointList.clear();
}

//!
//! \return Referenceobject
//!
Referenceobject PacketReferenceObject::getPacketReferenceobject()
{
    qDebug() << "PacketReferenceObject::getPacketReferenceobject()";
    Referenceobject objectReturn;
    qDebug() << QString::fromStdString(m_referenceobject->coordinateSystem);
    objectReturn.__set_coordinateSystem(m_referenceobject->coordinateSystem);

    qDebug() << QString::fromStdString(m_referenceobject->projectorID);
    objectReturn.__set_projectorID(m_referenceobject->projectorID);

    qDebug() << m_referenceobject->activated;
    objectReturn.__set_activated(m_referenceobject->activated);

    qDebug() << QString::fromStdString(m_referenceobject->refPointList[0].name);
    objectReturn.__set_refPointList(m_referenceobject->refPointList);

    qDebug() << QString::fromStdString(m_referenceobject->name);
    objectReturn.__set_name(m_referenceobject->name);
    return objectReturn;
}

//!name
//! \brief Referenceobject name
//! \param string name
//!
void PacketReferenceObject::setname(const QString name)
{
    m_referenceobject->__set_name(name.toStdString());
}

void PacketReferenceObject::setReferenceobject(Referenceobject referenceObject)
{
    m_referenceobject = &referenceObject;
}

bool PacketReferenceObject::isActive()
{
    return m_referenceobject->activated;
}

bool PacketReferenceObject::checkAvailableRefernceObject(const QString &coordname, const QString &refobjname)
{
    return false;
}

vector<Referencepoint> PacketReferenceObject::getReferenceList()
{
    return m_referenceobject->refPointList;
}


Polyline PacketReferenceObject::ConstructPolyLinesFromObject()
{
    qDebug() << "GetPolyLinesFromObject()";
    zlaser::thrift::Polyline result;
    vector<Vector3D> Points, lineleft, linetop, lineright, linebottom;
    vector<vector<Vector3D>> multiLineString;
    int count = 0;
    for(zlaser::thrift::Referencepoint pointfrom: m_referenceobject->refPointList){
        Vector3D Point = pointfrom.refPoint;
        Points.push_back(Point);
        qDebug() << QString("Name:%1").arg(QString::fromStdString(pointfrom.name));
        qDebug() << QString("Point x:%1 and point y: %2").arg(Points.at(count).x).arg(Points.at(count).y);
    }
    // at here linestring have 2 line 0,0 1000,1000
    // to buid rectangle we need 4 line
    Point3D point;
    //! line left
    point.__set_x(Points[0].x);
    point.__set_y(Points[0].y);
    point.__set_z(0);
    lineleft.push_back(point);
    point.__set_x(Points[1].x);
    point.__set_y(Points[1].y);
    point.__set_z(0);
    lineleft.push_back(point);
    qDebug() << "Line Left";
    qDebug() << QString("line bottom left point:%1,%2").arg(lineleft.at(0).x).arg(lineleft.at(0).y);
    qDebug() << QString("line top left point:%1,%2").arg(lineleft.at(1).x).arg(lineleft.at(1).y);
    //! line top
    point.__set_x(Points[1].x);
    point.__set_y(Points[1].y);
    point.__set_z(0);
    linetop.push_back(point);
    point.__set_x(Points[2].x);
    point.__set_y(Points[2].y);
    point.__set_z(0);
    linetop.push_back(point);
    qDebug() << "Line TOP";
    qDebug() << QString("line top left point:%1,%2").arg(linetop.at(0).x).arg(linetop.at(0).y);
    qDebug() << QString("line top right point:%1,%2").arg(linetop.at(1).x).arg(linetop.at(1).y);
    //! line right
    point.__set_x(Points[2].x);
    point.__set_y(Points[2].y);
    point.__set_z(0);
    lineright.push_back(point);
    point.__set_x(Points[3].x);
    point.__set_y(Points[3].y);
    point.__set_z(0);
    lineright.push_back(point);
    qDebug() << "Line Right";
    qDebug() << QString("line top right point:%1,%2").arg(lineright.at(0).x).arg(lineright.at(0).y);
    qDebug() << QString("line bottom right point:%1,%2").arg(lineright.at(1).x).arg(lineright.at(1).y);
    //! line bottom
    point.__set_x(Points[0].x);
    point.__set_y(Points[0].y);
    point.__set_z(0.0);
    linebottom.push_back(point);
    point.__set_x(Points[3].x);
    point.__set_y(Points[3].y);
    point.__set_z(0.0);
    linebottom.push_back(point);
    qDebug() << "Line Bottom";
    qDebug() << QString("line bottom right point:%1,%2").arg(linebottom.at(0).x).arg(linebottom.at(0).y);
    qDebug() << QString("line bottom left point:%1,%2").arg(linebottom.at(1).x).arg(linebottom.at(1).y);

    //! append for all line
    multiLineString.push_back(lineleft);
    multiLineString.push_back(linetop);
    multiLineString.push_back(lineright);
    multiLineString.push_back(linebottom);

    result.__set_polylineList(multiLineString);
    return result;
}

void PacketReferenceObject::setCoordinateSystem(QString coordname)
{
    m_referenceobject->__set_coordinateSystem(m_coordinatename.toStdString());
}

void PacketReferenceObject::setProjectorId(QString projectorid)
{
    m_referenceobject->__set_projectorID(projectorid.toStdString());
}

void PacketReferenceObject::setReferencepoints(QList<Referencepoint> referencepoints)
{
    if(referencepoints.count() >= 0){
        qDebug() << QString("Assignment more than :%1").arg(referencepoints.count());
        return;
    }
    m_referenceobject->refPointList.clear();
    Q_FOREACH(Referencepoint point, referencepoints)
        m_referenceobject->refPointList.push_back(point);

}

void PacketReferenceObject::setActive(bool isactive)
{
    m_referenceobject->__set_activated(isactive);
}

void PacketReferenceObject::setTransformationMatrix(const Matrix4x4 matrix)
{
    m_referenceobject->__set_fieldTransMat(matrix);
}



