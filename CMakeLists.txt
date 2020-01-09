cmake_minimum_required(VERSION 3.0)
project(ZLP-CPP-SDK)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()



find_package(Boost 1.55 REQUIRED COMPONENTS system filesystem)
link_directories(${Boost_LIBRARY_DIRS})

find_package(Qt5 COMPONENTS  Core Widgets Xml Test REQUIRED)

set(APACHE_THRIFT_INCLUDE_DIR CACHE PATH "Apache Thrift include directory")
find_program(APACHE_THRIFT_COMPILER thrift DOC "Apache Thrift compiler")
find_library(APACHE_THRIFT_LIB thrift DOC "Apache Thrift library")

# Generate thrift interface for C++ ----------------------------------------------------------------------------
set(CMAKE_AUTOUIC ON)
set(SRC_OUT ${CMAKE_CURRENT_BINARY_DIR}/src-gen)
set(GENERATED_CPP_SRC
    ${SRC_OUT}/interface_constants.cpp
    ${SRC_OUT}/interface_constants.h
    ${SRC_OUT}/interface_types.cpp
    ${SRC_OUT}/interface_types.h
    ${SRC_OUT}/ServiceInterface.cpp
    ${SRC_OUT}/ServiceInterface.h
    ${SRC_OUT}/ClientEventChannel.cpp
    ${SRC_OUT}/ClientEventChannel.h)

add_custom_command(OUTPUT ${GENERATED_CPP_SRC}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SRC_OUT}
    COMMAND ${APACHE_THRIFT_COMPILER} --gen cpp --out ${SRC_OUT} ${PROJECT_SOURCE_DIR}/src/interface.thrift
    COMMENT "Generating Thrift interface for C++.")

# UI Sources
set(UI_SOURCES
    src/mainwindow.ui
    src/dialogdevicemanager.ui
    src/dialogcoordinatesystems.ui
    src/dialogfilename.ui
    src/dialogabout.ui)


QT5_WRAP_UI(UI_HEADERS ${UI_SOURCES})

# Generate example application ---------------------------------------------------------------------------------

add_executable(sdk-cpp-app
    src/main.cpp
    src/activate_projector.cpp
    src/transfer_license.cpp
    src/create_projection.cpp
    src/set_up_projector.cpp
    src/common/client.cpp
    src/common/client.h
    src/common/event_channel_interface_handler.cpp
    src/common/event_channel_interface_handler.h
    src/common/event_channel_server.cpp
    src/common/event_channel_server.h
    src/mainwindow.h
    src/mainwindow.cpp
    src/log_widget.h
    src/log_widget.cpp
    src/swith_button.h
    src/swith_button.cpp
    src/global.h
    src/ui_mainwindow.h
    src/workspaceview.h
    src/workspaceview.cpp
    src/renderarea.h
    src/renderarea.cpp
    src/dialogdevicemanager.h
    src/dialogdevicemanager.cpp
    src/ui_dialogdevicemanager.h
    src/dotsignal.h
    src/dotsignal.cpp
    src/veselectionrect.h
    src/veselectionrect.cpp
    src/toolbars.h
    src/toolbars.cpp
    src/zlp-cpp-sdk.qrc
    src/outbondpacket.h
    src/outbondpacket.cpp
    src/packetreferenceobject.h
    src/packetreferenceobject.cpp
    src/packetreferencepoint.h
    src/packetreferencepoint.cpp
    src/dialogcoordinatesystems.h
    src/dialogcoordinatesystems.cpp
    src/dialogfilename.h
    src/dialogfilename.cpp
    src/packetservice.h
    src/packetservice.cpp
    src/interfaceextexe.h
    src/interfaceextexe.cpp
    src/pathutils.h
    src/pathutils.cpp
    src/z_io.h
    src/z_io.cpp
    src/parametercross.h
    src/parametercross.cpp
    src/parameterdistance.h
    src/parameterdistance.cpp
    src/parametertracepoint.h
    src/parametertracepoint.cpp
    src/labelcorner.h
    src/labelcorner.cpp
    src/packettext.h
    src/packettext.cpp
    src/projection.h
    src/projection.cpp
    src/projections.h
    src/projections.cpp
    src/packetpolyline.h
    src/packetpolyline.cpp
    src/pointindentifier.h
    src/pointindentifier.cpp
    src/parameter.h
    src/parameter.cpp
    src/parameters.h
    src/parameters.cpp
    src/dialogabout.h
    src/dialogabout.cpp

    ${GENERATED_CPP_SRC}
    ${UI_HEADERS})




target_include_directories(sdk-cpp-app PRIVATE
    src/common
    ${SRC_OUT}
    ${APACHE_THRIFT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS})

if(UNIX)
    target_compile_options(sdk-cpp-app PRIVATE -std=c++11)
endif()
# Use the Widgets module from Qt 5
target_link_libraries(sdk-cpp-app ${APACHE_THRIFT_LIB} ${Boost_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Test)
if(UNIX)
    target_link_libraries(sdk-cpp-app pthread)
endif()
