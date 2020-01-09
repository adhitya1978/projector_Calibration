#pragma once

#include <string>
#include <QStringList>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

#include <ServiceInterface.h>


#include "event_channel_interface_handler.h"

using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::transport::TBufferedTransport;
using apache::thrift::transport::TSocket;

class EventChannelServer;


class ZlpThriftClient : public zlaser::thrift::ServiceInterfaceClient
{
public:

    struct Exception : std::runtime_error
    {
        Exception(const std::string& what) : std::runtime_error(what) {}
    };

    /// Creates the module to get connected with the service, this replaces the standard constructor
    static ZlpThriftClient* create();

    ~ZlpThriftClient();

    /// Establish connection to Server and also the event channel from service to client
    void connect(int zlpServicePort, std::string zlpServiceIp);
    void disconnect();

    void setPropertyChangedCallback(const std::function<EventChannelInterfaceHandler::PropertChangedCallbackType>& callback);
    void setFunctionModuleStateChangedCallback(const std::function<EventChannelInterfaceHandler::FunctionModuleStateChangedCallbackType>& callback);

    //!
    //! get all element
    std::vector<zlaser::thrift::GeoTreeElemId> getAllElements(std::string &branchName, zlaser::thrift::GeoTreeElemTypes::type typeOfElement);

    bool removeGeoTreeElement(std::string elemName, zlaser::thrift::GeoTreeElemTypes::type typeOfElement);

    void getAllReferenceObjectatService(std::list<zlaser::thrift::Referenceobject> &listReferenceObject);

    zlaser::thrift::Referenceobject getRefenceObjectFromService(std::string refObjectname);

    std::vector<std::string> getCoordinateSystems();
    std::vector<zlaser::thrift::GeoTreeElemId> getGeoTreeIds(const std::string& branchName = "", zlaser::thrift::GeoTreeElemTypes::type typeOfElement = zlaser::thrift::GeoTreeElemTypes::ALL_TYPE);
    zlaser::thrift::Polyline getPolyline(const std::string& name, bool onlyAttributes = false);

    std::string createFunctionModule(const std::string& type, const std::string& branchName);
    std::string getFunctionModuleProperty(const std::string& functionModuleId, const std::string& propertyName);

    std::string getProperty(const std::string& name);
    std::vector<std::string> getPropertyChildren(const std::string& name);

    void transferFile(const std::string& localPath, const std::string& remoteFilename, bool overwrite = false);

    std::vector<std::string> scanProjectors(std::string scanAddresses = "");
    std::vector<std::string> getProjectors(bool scan = false, std::string scanAddresses = "");
    void activateProjector(std::string projectorSerial);
    void deactivateProjector(std::string projectorSerial);

private:

    /// Constructor, not for extern use, use ThriftClient::Create() for constructing an new instance
    ZlpThriftClient(boost::shared_ptr<TSocket> socket, boost::shared_ptr<TBufferedTransport> transport, boost::shared_ptr<TBinaryProtocol> protocol);

    boost::shared_ptr<TSocket> socket;
    boost::shared_ptr<TBufferedTransport> transport;
    boost::shared_ptr<TBinaryProtocol> protocol;
    boost::shared_ptr<EventChannelServer> eventChannel;
};
