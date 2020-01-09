#include "client.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <QDebug>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include "interface_types.h"
#include "event_channel_server.h"

using namespace std;
using namespace zlaser::thrift;

typedef boost::shared_ptr<TSocket> shared_socket;
typedef boost::shared_ptr<TBufferedTransport> shared_transport;
typedef boost::shared_ptr<TBinaryProtocol> shared_protocol;

ZlpThriftClient* ZlpThriftClient::create()
{
    shared_socket socket = shared_socket(new TSocket());
    shared_transport transport = shared_transport(new TBufferedTransport(socket));
    shared_protocol protocol  = shared_protocol(new TBinaryProtocol(transport));
    return new ZlpThriftClient(socket, transport, protocol);
}

ZlpThriftClient::ZlpThriftClient(shared_socket socket, shared_transport transport, shared_protocol protocol) :
    ServiceInterfaceClient(protocol),
    socket(socket),
    transport(transport),
    protocol(protocol),
    eventChannel(boost::make_shared<EventChannelServer>(boost::make_shared<EventChannelInterfaceHandler>()))
{}

ZlpThriftClient::~ZlpThriftClient()
{
    disconnect();
}

void ZlpThriftClient::connect(int port, string host)
{
    assert(port >= CommunicationPorts::SERVER_MIN && port <= CommunicationPorts::SERVER_MAX);

    try
    {
        // establish event channel

        eventChannel->start();
        this_thread::sleep_for(chrono::milliseconds(500));

        if (eventChannel->getState() != EventChannelServer::State::running)
        {
            throw Exception("Cannot open event channel. Server is not running yet.");
        }

        // establish connection to zlp-service

        socket->setHost(host);
        socket->setPort(port);
        socket->setConnTimeout(500);
        transport->open();

        ConnectClientEventChannel(eventChannel->getPort());

        socket->setConnTimeout(50000);  // timeout for longterm running functions
    }
    catch (const exception& e)
    {
        disconnect();
        throw Exception("Could not establish a connection: " + string(e.what()));
    }
}

void ZlpThriftClient::disconnect()
{
    try
    {
        if (transport && transport->isOpen())
        {
            DisconnectClientEventChannel();
            transport->close();
        }

        if (eventChannel)
        {
            eventChannel->stop();
        }
    }
    catch (...)
    {};
}

vector<string> ZlpThriftClient::getCoordinateSystems()
{
    vector<string> coordinateSystems;
    GetCoordinatesystemList(coordinateSystems);
    return coordinateSystems;
}

vector<zlaser::thrift::GeoTreeElemId> ZlpThriftClient::getGeoTreeIds(const string& branchName, zlaser::thrift::GeoTreeElemTypes::type typeOfElement)
{
    vector<zlaser::thrift::GeoTreeElemId> ids;
    GetGeoTreeIds(ids, branchName, typeOfElement);
    return ids;
}

zlaser::thrift::Polyline ZlpThriftClient::getPolyline(const string &name, bool onlyAttributes)
{
    zlaser::thrift::Polyline polyline;
    GetPolyLine(polyline, name, onlyAttributes);
    return polyline;
}

string ZlpThriftClient::getProperty(const string& name)
{
    string value;
    GetProperty(value, name);
    return value;
}

vector<string> ZlpThriftClient::getPropertyChildren(const string& name)
{
    vector<string> children;
    GetPropChildren(children, name);
    return children;
}

void ZlpThriftClient::transferFile(const string& localPath, const string& remoteFile, bool overwrite)
{
    if (boost::filesystem::is_directory(localPath))
    {
        throw Exception(str(boost::format("File \"%s\" is a directory!")%localPath));
    }

    if (!boost::filesystem::exists(localPath))
    {
        throw Exception(str(boost::format("File \"%s\" does'nt exist!")%localPath));
    }

    ifstream ifs(localPath, ifstream::in | ifstream::binary);
    stringstream ss;
    ss << ifs.rdbuf(); //read the file
    TransferDataToFile(ss.str(), remoteFile, overwrite);
    ifs.close();
}

vector<string> ZlpThriftClient::scanProjectors(string scanAddresses) {
    return getProjectors(true, scanAddresses);
}

vector<string> ZlpThriftClient::getProjectors(bool scan, string scanAddresses)
{
    vector<string> projectors;

    try
    {
        // Set parameters for projector search
        SetProperty("config.projectorManager.cmdGetProjectors.scan", scan ? "1" : "0");
        SetProperty("config.projectorManager.cmdGetProjectors.scanAddresses", scanAddresses);

        // Execute search
        SetProperty("config.projectorManager.cmdGetProjectors", "1");

        string serialNumbers = this->getProperty("config.projectorManager.cmdGetProjectors.result.entries");

        if (!serialNumbers.empty())
        {
            boost::split(projectors, serialNumbers, boost::is_any_of(" "));
        }
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        printf("Property not found: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        printf("Property change failed: %s\n", e.what());
    }

    return projectors;
}

void ZlpThriftClient::activateProjector(string projectorSerial)
{
    try
    {
        std::cout << "Activate projector: " << projectorSerial << std::endl;
        SetProperty("config.projectorManager.cmdActivateProjector.serial", projectorSerial);
        SetProperty("config.projectorManager.cmdActivateProjector.active", "1");
        SetProperty("config.projectorManager.cmdActivateProjector", "1");
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        printf("Property not found: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        printf("Property change failed: %s\n", e.what());
    }
    // blocks until the projector is activated
    getProjectors();
}

void ZlpThriftClient::deactivateProjector(string projectorSerial)
{
    try
    {
        std::cout << "Deactivate projector: " << projectorSerial << std::endl;

        // Deactivate projection
        SetProperty("config.projectorManager.projectors." + projectorSerial + "." + "cmdShowProjection.show", "0");
        SetProperty("config.projectorManager.projectors." + projectorSerial + "." + "cmdShowProjection", "1");

        // Deactivate projector
        SetProperty("config.projectorManager.cmdActivateProjector.serial", projectorSerial);
        SetProperty("config.projectorManager.cmdActivateProjector.active", "0");
        SetProperty("config.projectorManager.cmdActivateProjector", "1");
    }
    catch (zlaser::thrift::PropertyNotFound& e)
    {
        printf("Property not found: %s\n", e.what());
    }
    catch (zlaser::thrift::PropertyChangeFailed& e)
    {
        printf("Property change failed: %s\n", e.what());
    }
}

string ZlpThriftClient::createFunctionModule(const string& type, const string& branchName)
{
    std::string moduleId;
    FunctionModuleCreate(moduleId, type, branchName);
    return moduleId;
}

string ZlpThriftClient::getFunctionModuleProperty(const string& moduleId, const string& propertyName)
{
    string value;
    FunctionModuleGetProperty(value, moduleId, propertyName);
    return value;
}

void ZlpThriftClient::setPropertyChangedCallback(const std::function<EventChannelInterfaceHandler::PropertChangedCallbackType>& callback)
{
    eventChannel->setPropertyChangedCallback(callback);
}

void ZlpThriftClient::setFunctionModuleStateChangedCallback(const std::function<EventChannelInterfaceHandler::FunctionModuleStateChangedCallbackType>& callback)
{
    eventChannel->setFunctionModuleStateChangedCallback(callback);
}


std::vector<GeoTreeElemId> ZlpThriftClient::getAllElements(string &branchName, GeoTreeElemTypes::type typeOfElement)
{
    std::vector<GeoTreeElemId> geoTreeElementIds;
    GetGeoTreeIds(geoTreeElementIds, branchName, GeoTreeElemTypes::type::ALL_TYPE);
    return geoTreeElementIds;
}



void ZlpThriftClient::getAllReferenceObjectatService(std::list<Referenceobject> &listReferenceObject)
{
    vector<GeoTreeElemId> geoTree;
    if( (geoTree = getGeoTreeIds("", GeoTreeElemTypes::REFOBJECT)).size() != 0)
    {
        for(unsigned int i=0; i< geoTree.size(); i++)
        {
            //! make sure element type always reference object
            if(geoTree.at(i).elemType == GeoTreeElemTypes::REFOBJECT)
            {
                listReferenceObject.push_back(getRefenceObjectFromService(geoTree.at(i).name));
            }
        }
    }
}


Referenceobject ZlpThriftClient::getRefenceObjectFromService(string refObjectname)
{
    zlaser::thrift::Referenceobject refObject;
    GetReferenceobject(refObject, refObjectname);
    return refObject;
}


