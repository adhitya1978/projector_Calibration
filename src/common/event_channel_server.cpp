#include "event_channel_server.h"

#include <thrift/transport/TBufferTransports.h>

EventChannelServer::EventChannelServer(boost::shared_ptr<EventChannelInterfaceHandler> handler) :
    state(State::undefined),
    interfaceHandler(handler)
{}

EventChannelServer::~EventChannelServer()
{
    std::lock_guard<std::recursive_mutex> lock(stateMutex);
    if (state == State::running)
    {
        stop();
    }
}

void EventChannelServer::start()
{
    using namespace apache::thrift;

    std::lock_guard<std::recursive_mutex> lock(stateMutex);
    assert(state != State::running);

    boost::shared_ptr<TProcessor> processor(new zlaser::thrift::ClientEventChannelProcessor(interfaceHandler));
    socket.reset(new transport::TServerSocket(0)); // "0" select available port automatically
    boost::shared_ptr<server::TServerTransport> serverTransport(socket);
    boost::shared_ptr<server::TTransportFactory> transportFactory(new transport::TBufferedTransportFactory());
    boost::shared_ptr<server::TProtocolFactory> protocolFactory(new server::TBinaryProtocolFactory());

    thriftServer = boost::shared_ptr<server::TSimpleServer>(new server::TSimpleServer(processor, serverTransport, transportFactory, protocolFactory));

    thriftServerThread = std::thread(&EventChannelServer::runServer, this);
}

void EventChannelServer::stop()
{
    std::lock_guard<std::recursive_mutex> lock(stateMutex);

    if (thriftServerThread.joinable())
    {
        thriftServer->stop();
        thriftServerThread.join();
    }

    state = State::stopped;
}

void EventChannelServer::runServer()
{
    try
    {
        state = State::running;
        thriftServer->serve(); // blocks until stop() is called
        state = State::stopped;
    }
    catch(const apache::thrift::TException&)
    {
        state = State::error;
    }
    catch(...)
    {
        assert(!"Unexpected Thrift server error");
        state = State::error;
    }
}

EventChannelServer::State EventChannelServer::getState() const
{
    std::lock_guard<std::recursive_mutex> lock(stateMutex);
    return state;
}

int EventChannelServer::getPort() const
{
    return socket->getPort();
}

void EventChannelServer::setPropertyChangedCallback(const std::function<EventChannelInterfaceHandler::PropertChangedCallbackType> &callback)
{
    interfaceHandler->setPropertyChangedCallback(callback);
}

void EventChannelServer::setFunctionModuleStateChangedCallback(const std::function<EventChannelInterfaceHandler::FunctionModuleStateChangedCallbackType> &callback)
{
    interfaceHandler->setFunctionModuleStateChangedCallback(callback);
}
