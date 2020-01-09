#include <vector>
#include <thread>
#include <iostream>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/math/constants/constants.hpp>

#include "common/client.h"

using namespace std;
using namespace zlaser::thrift;
using namespace boost::math::double_constants;

zlaser::thrift::Polyline createPolyline()
{
    zlaser::thrift::Polyline polyline;
    polyline.__set_activated(true);

    Vector3D point;
    vector<Vector3D> lineStringA, lineStringB;
    vector<vector<Vector3D>> multiLineString;

    point.__set_x(100);
    point.__set_y(100);
    point.__set_z(0);
    lineStringA.push_back(point);

    point.__set_x(100);
    point.__set_y(-100);
    point.__set_z(0);
    lineStringA.push_back(point);

    point.__set_x(-100);
    point.__set_y(-100);
    point.__set_z(0);
    lineStringB.push_back(point);

    point.__set_x(-100);
    point.__set_y(100);
    point.__set_z(0);
    lineStringB.push_back(point);

    multiLineString.push_back(lineStringA);
    multiLineString.push_back(lineStringB);

    polyline.__set_polylineList(multiLineString);

    return polyline;
}

zlaser::thrift::CircleSegment createCircle()
{
    zlaser::thrift::CircleSegment circle;

    Point3D center;
    center.__set_x(200);
    center.__set_y(200);
    center.__set_z(0);
    circle.__set_center(center);

    circle.__set_radius(250);

    Vector3D normal;
    double tilt = 60. / 180. * pi;
    normal.__set_x(sin(tilt));
    normal.__set_y(0);
    normal.__set_z(cos(tilt));
    circle.__set_normal(normal);

    circle.__set_activated(true);

    return circle;
}

zlaser::thrift::CircleSegment createArc()
{
    zlaser::thrift::CircleSegment arc;

    Point3D center;
    center.__set_x(-200);
    center.__set_y(400);
    center.__set_z(0);
    arc.__set_center(center);

    arc.__set_radius(200);
    arc.__set_chordHeight(5);
    arc.__set_startAngle(90);
    arc.__set_endAngle(180);
    arc.__set_activated(true);

    return arc;
}

zlaser::thrift::TextElement createText()
{
    zlaser::thrift::TextElement text;
    Point3D position;

    position.__set_x(100);
    position.__set_y(0);
    position.__set_z(0);
    text.__set_position(position);

    text.__set_text("Z-Laser");
    text.__set_charSpacing(100);
    text.__set_height(100);
    text.__set_angle(25);
    text.__set_activated(true);

    return text;
}

void createProjection(const string ip, const int port)
{
    try
    {
        cout << "Example: Create projection." << endl;

        // initialize system

        shared_ptr<ZlpThriftClient> client(ZlpThriftClient::create());
        client->connect(port, ip);

        vector<string> projectors = client->scanProjectors();
        if (projectors.empty())
            throw runtime_error("No projector found.");

        const string projector = projectors.front();
        client->activateProjector(projector);

        // create projection elements

        zlaser::thrift::Polyline polyline = createPolyline();
        zlaser::thrift::CircleSegment circle = createCircle();
        zlaser::thrift::CircleSegment arc = createArc();
        zlaser::thrift::TextElement text = createText();

        // create group

        string projectionGroup = "my_group";
//[set name]
        polyline.__set_name(projectionGroup + "/my_polyline");
        circle.__set_name(projectionGroup + "/my_circle");
        arc.__set_name(projectionGroup + "/my_arc");
        text.__set_name(projectionGroup + "/my_text");

        // set coordinate systems to project into

        vector<string> availableCoordinateSystems = client->getCoordinateSystems();
// [load all coordinate systems]
        vector<string> selectedCoordinateSystems;
        selectedCoordinateSystems.push_back(projector + "_FCW"); // default coordinate system of projector (factory calibration wall)
        selectedCoordinateSystems.push_back("CoordinateSystemXY"); // you can also add a custom coordinate system

        // set list
        polyline.__set_coordinateSystemList(selectedCoordinateSystems);
        circle.__set_coordinateSystemList(selectedCoordinateSystems);
        arc.__set_coordinateSystemList(selectedCoordinateSystems);
        text.__set_coordinateSystemList(selectedCoordinateSystems);

        // transfer and project elements
// set to interface
        client->SetPolyLine(polyline);
        client->SetCircleSegment(circle);
        client->SetCircleSegment(arc);
        client->SetTextElement(text);
// triger
        client->TriggerProjection();
        this_thread::sleep_for(chrono::seconds(5));

        // turn off polyline

        polyline.__set_activated(false);
        client->SetPolyLine(polyline);

        client->TriggerProjection();
        this_thread::sleep_for(chrono::seconds(5));

        // clean-up

        client->RemoveGeoTreeElem(projectionGroup);
        client->deactivateProjector(projector);
        client->disconnect();
    }
    catch (ZlpThriftClient::Exception& e)
    {
        printf("Client exception: %s", e.what());
    }
    catch (zlaser::thrift::PropertyNotFound& e1)
    {
        printf("Property not found: %s\n", e1.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e2)
    {
        printf("Property change failed: %s\n", e2.what());
    }
    catch (zlaser::thrift::ElementDoesNotExist& e)
    {
        printf("Element does not exist: %s\n", e.what());
    }
}
