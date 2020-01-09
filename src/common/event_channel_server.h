#pragma once

#include <mutex>
#include <thread>

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>

#include <ClientEventChannel.h>

#include "event_channel_interface_handler.h"

class EventChannelServer
{
public:

    enum struct State {undefined, init, running, stopped, error};

    EventChannelServer(boost::shared_ptr<EventChannelInterfaceHandler> interfaceHandler);
    virtual ~EventChannelServer();

    void start();
    void stop();

    State getState() const;
    int getPort() const; ///< Returns the automatically chosen port

    void setPropertyChangedCallback(const std::function<EventChannelInterfaceHandler::PropertChangedCallbackType>& callback);
    void setFunctionModuleStateChangedCallback(const std::function<EventChannelInterfaceHandler::FunctionModuleStateChangedCallbackType>& callback);

private:

    void runServer();

    mutable std::recursive_mutex stateMutex;
    State state;

    boost::shared_ptr<apache::thrift::server::TSimpleServer> thriftServer;
    std::thread thriftServerThread;

    boost::shared_ptr<EventChannelInterfaceHandler> interfaceHandler;
    boost::shared_ptr<apache::thrift::transport::TServerSocket> socket;
};
