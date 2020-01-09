#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <memory>
#include <iostream>

#include <QObject>
#include <QString>
#include <QList>

#include "common/client.h"
//! reference point
#include "interface_types.h"


using namespace std;
using namespace zlaser::thrift;


class bridge_projector : public QObject
{
    Q_OBJECT

public:
    explicit bridge_projector(const QString &ip, int port, QObject *parent = nullptr );
    ~bridge_projector();
    void sw_on_projector(const string &serialnumber);
    void sw_off_projector(const std::string & serialnumber);
    // properties
    QList<std::string> *list_projector() { return seek_projector_serialnumber();  };

    bool connected() {return is_connected;};

    void create_workspace(const QString &workspace_name);

    Vector3D create_point(const double x, const double y, const double z);

    std::vector<Vector3D> create_line(const Vector3D & pointA, const Vector3D & pointB );

    vector<vector<Vector3D> > create_multiline(const vector<Vector3D> & candidate_point);

    void create_box_portrait(const string &serialnumber);

    //! create titik coordinate
    Referencepoint *create_reference_point(const Vector3D point, const QString& reference_name);
    //! create coordinate
    void create_coordinate(const vector<Referencepoint> candidate_point, const QString& name);


    zlaser::thrift::Polyline create_polyline(const vector<vector<Vector3D> > candidate_line);


private:
    //! current port
    int m_port;

    //! current IP projector
    QString m_ip;

    //! interface projector
    ZlpThriftClient *m_client;

    //! indicator projecter is connected
    bool is_connected;

    //! available projector localhost
    QList<::string> *seek_projector_serialnumber();

    //! main active coordinate
    Referenceobject m_coordinate;

};

class refPointExt : public Referencepoint
{

};

#endif // GLOBAL_H


