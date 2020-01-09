
#include "log_widget.h"
#include "global.h"
// main interface


/**
void searchAndActivateProjector(const string ip, const int port)
{
    try
    {
        //cout << "Example: Search and Activate Projector." << endl;


        shared_ptr<ZlpThriftClient> client(ZlpThriftClient::create());
        client->connect(port, ip);

        vector<string> projectors = client->scanProjectors();

        if (!projectors.empty())
        {


            for (const string& serialNumber : projectors){
                cout << serialNumber << endl;
                DPRINT(0, QString("Available projectors:%0").arg(QString::fromStdString(serialNumber)));
            }

            const string projector = projectors.front();

            client->activateProjector(projector);

            // do something with the projector...

            client->deactivateProjector(projector);
        }
        else
        {
            cout << "No projector found." << endl;
        }

        client->disconnect(); // not neccessary because client is also disconnected on destruction
    }
    catch (ZlpThriftClient::Exception& e)
    {
        cout << "Client exception: " << e.what() << endl;
    }
}

**/

bridge_projector::bridge_projector(const QString &ip, int port, QObject *parent):
   QObject(parent),
   m_port(port),
   m_ip(ip),
   m_client(ZlpThriftClient::create())
{
}

bridge_projector::~bridge_projector()
{
    m_client = nullptr;
}

void bridge_projector::sw_on_projector(const std::string & serialnumber)
{
    try{
        DPRINT(2, QString("Activating...%0").arg(QString::fromStdString(serialnumber)));
        DPRINT(2, QString("Activated projector id:%0").arg(QString::fromStdString(serialnumber)));
        m_client->connect(this->m_port, this->m_ip.toStdString());
        m_client->activateProjector(serialnumber);
        this->is_connected = true;
    }
    catch(ZlpThriftClient::Exception& e){
        DPRINT(0, QString("switch on projector:%0").arg(e.what()));
    }
}

void bridge_projector::sw_off_projector(const string &serialnumber)
{
    try{
        DPRINT(2, QString("deactivating..."));
        m_client->deactivateProjector(serialnumber);
        m_client->disconnect();
        is_connected = false;
        DPRINT(2, QString("deactivated projector id:%0").arg(QString::fromStdString(serialnumber)));

    }
    catch(ZlpThriftClient::Exception& e){
        DPRINT(0, QString("switch off projector:%0").arg(e.what()));
    }
}

void bridge_projector::create_workspace(const QString &workspace_name)
{

}

Vector3D bridge_projector::create_point(const double x, const double y, const double z)
{
   Vector3D point;
   point.__set_x(x);
   point.__set_y(y);
   point.__set_z(z);
   return point;
}
// create line 2 titik
std::vector<Vector3D> bridge_projector::create_line(const Vector3D &pointA, const Vector3D &pointB)
{
    std::vector<Vector3D> linestring;
    linestring.push_back(pointA);
    linestring.push_back(pointB);

    return linestring;
}

vector<vector<Vector3D>> bridge_projector::create_multiline(const vector<Vector3D> &new_line)
{
    vector<vector<Vector3D>> multiLineString;
    multiLineString.push_back(new_line);
    return multiLineString;
}

void bridge_projector::create_box_portrait(const std::string & serialnumber)
{
    std::vector<Vector3D> line_left = create_line(create_point(0.0,0,0.0), create_point(0, 297,0.0));
    std::vector<Vector3D> line_right = create_line(create_point(210.0,0.0,0.0), create_point(210, 297,0.0));
    std::vector<Vector3D> line_footer = create_line(create_point(0, 0, 0), create_point(0, 0, 0));
    std::vector<Vector3D> line_header = create_line(create_point(0, 0, 0), create_point(0, 0,0.0));

    vector<vector<Vector3D>> combine_all_line = create_multiline(line_header);
    combine_all_line.push_back(line_footer);
    combine_all_line.push_back(line_left);
    combine_all_line.push_back(line_right);

    zlaser::thrift::Polyline candidate_polyline = create_polyline(combine_all_line);


    // prepare variable name coordinate
    vector<std::string> candidate_coordinate;
    candidate_coordinate.push_back(serialnumber + "_FCW");
    candidate_coordinate.push_back("CoordinateSystemXY");

    candidate_polyline.__set_coordinateSystemList(candidate_coordinate);

    if(this->is_connected){
        m_client->SetPolyLine(candidate_polyline);
        m_client->TriggerProjection();
    }
    else
    {
        DPRINT(0, QString("Projecter not connected"));
    }

}

Referencepoint *bridge_projector::create_reference_point(const Vector3D point,
                                                        const QString &reference_name)
{
    Referencepoint *candidate_point = new Referencepoint;
    candidate_point->__set_refPoint(point);
    candidate_point->__set_name(reference_name.toStdString());
    return candidate_point;
}

void bridge_projector::create_coordinate(const vector<Referencepoint> candidate_points, const QString &name)
{
    //! [set name]
    m_coordinate.__set_coordinateSystem(name.toStdString());
    //! [template coordinate system]
    m_coordinate.__set_refPointList(candidate_points);
}

zlaser::thrift::Polyline bridge_projector::create_polyline(const vector<vector<Vector3D> > candidate_line)
{
    zlaser::thrift::Polyline candidate_polyline;
    std::string group = "my_group";
    candidate_polyline.__set_name(group + "/my_polyline");

    candidate_polyline.__set_activated(true);
    candidate_polyline.__set_polylineList(candidate_line);

    return  candidate_polyline;
}

QList<::string> *bridge_projector::seek_projector_serialnumber(){

    QList<::string> *projector_list = new QList<::string>();
    try
    {
        //cout << "Example: Search and Activate Projector." << endl;
\
        m_client->connect(this->m_port, this->m_ip.toStdString());

        vector<string> projectors = m_client->scanProjectors();

        if (!projectors.empty())
        {
            for (const string& serialNumber : projectors){
                projector_list->append(serialNumber);
            }

        }
        else{
            DPRINT(0, QString("no projector at IP %0 port %1").arg(this->m_ip).arg(this->m_port));
        }


        m_client->disconnect(); // not neccessary because client is also disconnected on destruction
    }
    catch (ZlpThriftClient::Exception& e)
    {
        DPRINT(0, QString("Client exception:%0").arg(e.what()));
        m_client->disconnect();
    }
    return  projector_list;
}

