#pragma once

#include "ClientEventChannel.h"
#include "interface_types.h"

class EventChannelInterfaceHandler : public zlaser::thrift::ClientEventChannelIf
{
  public:
    typedef void(PropertChangedCallbackType)(const std::string& name, const std::string& value);

    typedef void(FunctionModuleStateChangedCallbackType)(const std::string& moduleId,
                                                         zlaser::thrift::FunctionModuleStates::type oldState,
                                                         zlaser::thrift::FunctionModuleStates::type newState);

    void setPropertyChangedCallback(const std::function<PropertChangedCallbackType>& callback);

    void setFunctionModuleStateChangedCallback(const std::function<FunctionModuleStateChangedCallbackType>& callback);

    virtual void PropertyChanged(const std::string& name, const std::string& value);

    virtual void GeoTreeChanged(const int32_t changedFlags, const zlaser::thrift::GeoTreeElemId& element);

    virtual void ServiceStateChanged(const zlaser::thrift::ServiceStates::type oldState,
                                     const zlaser::thrift::ServiceStates::type newState);

    virtual void FunctionModuleStateChanged(const std::string& moduleId,
                                            const zlaser::thrift::FunctionModuleStates::type oldState,
                                            const zlaser::thrift::FunctionModuleStates::type newState);

    virtual void RemoteControlFrameReceived(const int16_t adr, const zlaser::thrift::RcCommandCodes::type cmd,
                                            const bool toggle, const int16_t projector, const int32_t timestamp);

    virtual void onReflectionStateChanged(std::string const &elementName, bool const state);

  private:
    std::function<PropertChangedCallbackType> propertyChangedCallback;
    std::function<FunctionModuleStateChangedCallbackType> functionModuleStateChangedCallback;
};
