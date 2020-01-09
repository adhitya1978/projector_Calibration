#ifndef PACKETPOLYLINE_H
#define PACKETPOLYLINE_H

#include <QObject>
#include <QString>

//! SDK
#include "interface_types.h"
#include "common/client.h"

using namespace std;
using namespace zlaser::thrift;

#include "projection.h"
#include "pointindentifier.h"

class PacketPolyline : public QObject, public Projection
{
    Q_OBJECT
public:
    explicit PacketPolyline(const PointIndentifier *pointIndentifier,
                            QString groupname,
                            QString elementname,
                            QObject *parent = nullptr);
    ~PacketPolyline();
    //! get current polyline
    Polyline get() const;
    //! set coordinate system for current text
    bool setCoordinateSystem(const QString coordname);
    //! get coordinate system which already sign in to current text
    QList<QString> getCoordinateSystem();
    //! interface projection
    QString get_projection_name();

    QString get_group_name() { return m_groupname; }

    bool get_activated();

    QList<QString> get_coordinateSystemList();

    PROJECTION_TYPE type();

private:
    zlaser::thrift::Polyline *m_Polyline;
    const PointIndentifier *m_pointIndentifier;
    PROJECTION_TYPE m_Type;
    QString m_elementname;
    QString m_groupname;
    void ConstructPolyline();

signals:

public slots:

};

#endif // PACKETPOLYLINE_H
