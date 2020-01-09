#include "event_channel_interface_handler.h"

using namespace std;
using namespace zlaser::thrift;

void EventChannelInterfaceHandler::setPropertyChangedCallback(
    const std::function<EventChannelInterfaceHandler::PropertChangedCallbackType>& callback)
{
    propertyChangedCallback = callback;
}

void EventChannelInterfaceHandler::setFunctionModuleStateChangedCallback(
    const std::function<EventChannelInterfaceHandler::FunctionModuleStateChangedCallbackType>& callback)
{
    functionModuleStateChangedCallback = callback;
}

void EventChannelInterfaceHandler::PropertyChanged(const string& name, const string& value)
{
    if (propertyChangedCallback) {
        propertyChangedCallback(name, value);
    }
}

void EventChannelInterfaceHandler::GeoTreeChanged(const int32_t, const zlaser::thrift::GeoTreeElemId&)
{
    // not used
}

void EventChannelInterfaceHandler::ServiceStateChanged(const ServiceStates::type, const ServiceStates::type)
{
    // not used
}

void EventChannelInterfaceHandler::FunctionModuleStateChanged(const std::string& moduleId,
                                                              const FunctionModuleStates::type oldState,
                                                              const FunctionModuleStates::type newState)
{
    if (functionModuleStateChangedCallback) {
        functionModuleStateChangedCallback(moduleId, oldState, newState);
    }
}

void EventChannelInterfaceHandler::RemoteControlFrameReceived(const int16_t adr,
                                                              const zlaser::thrift::RcCommandCodes::type cmd,
                                                              const bool toggle, const int16_t projector,
                                                              const int32_t timestamp)
{
    (void)adr;
    (void)cmd;
    (void)toggle;
    (void)projector;
    (void)timestamp;
    // not used
}

void EventChannelInterfaceHandler::onReflectionStateChanged(const std::string &elementName, const bool state)
{
    (void)elementName;
    (void)state;
}
