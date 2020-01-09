
/* Copyright (c) 2019 Laser Solution Asia

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*
*   Class OutbondPacket or Bridge projector
*
*/

#ifndef OUTBOUNDPACKET_H
#define OUTBOUNDPACKET_H

#include <string>

#include <QObject>
#include <QList>
#include <QTest>
#include <QHash>

#include "projections.h"
#include "projection.h"
#include "packettext.h"
#include "packetpolyline.h"
#include "parameters.h"

#include "interface_types.h"
#include "common/client.h"

using namespace std;
using namespace zlaser::thrift;

//! constanta module
#define MODULE_TYPE                "zFunctModRegister3d"
#define MODULE_BRANCH              "3DReg"
#define PROPERTY_NAME              "referenceData"
#define PROPERTY_VALUE             "RefObject"

#define ELEMENT_POLYLINE_NAME      "My_Polyline"
#define ELEMENT_TEXT_NAME          "My_Text"

//! constanta Command
#define CMD_CONFIG_PROJECTOR       "config.projectorManager.projectors."
#define CMD_PROJECTOR_SERIAL       "config.projectorManager.cmdActivateProjector.serial"
#define CMD_PROJECTOR_ACTIV        "config.projectorManager.cmdActivateProjector.active"
#define CMD_PROJECTOR_ACTIVATED    "config.projectorManager.cmdActivateProjector"

//! state projector

#define PROJECTOR_RUNNING   FunctionModuleStates::type::RUNNING
#define PROJECTOR_STOP      FunctionModuleStates::type::IDLE

//!
//! parameter area point scanning
//!
typedef struct SCANNING_POINT{
    QString     name;
    Point2D     tracepoint; //! real area will used
    double      distance;
    bool        activated;
    Vector2D    crosssize;
    Point3D     refpoint; //! limit area to be scanning
}*struct_scan_point;

class OutbondPacket : public QObject
{
    Q_OBJECT
public:

    enum PROJECTION_MODE{
        TRACEPOINT = 0,
        OBJECT = 1
    };

    OutbondPacket(ZlpThriftClient *client = nullptr, QObject *parent = nullptr);
    ~OutbondPacket();

    //! set trace point Parameter
    void setParameterTracePoint(const QList<SCANNING_POINT*> positions);

    //!
    Referenceobject getCoordSys(QString coordname, QString refobjname);

    //! get projector state
    FunctionModuleStates::type getProjectorState();

    //! set Referenceobject
    void setReferenceObject(Referenceobject referenceObject);

    //! set IP address and Port
    void setAddress(const QString IP, int port);


    //! get list name cordinate and referenceobject
    QHash<QString, QString> getAllNameReferenceObjectAndCoordSystem();

    QString createGroupTreePolyline(QString groupname);

    QList<string> printAvailableCoordinateSystems();

    //! list available projector in local networks
    QList<string> *printAvailableProjector();

    //! run projector
    bool runProjector();
    //! stop projector
    void stopProjector();
    void setElementPolyline(zlaser::thrift::Polyline polyline);
    void setElementText(const zlaser::thrift::TextElement &text);
    bool checkGeoTreeElement(string elementName);
    bool removeGeoTreeElement(string elementName, zlaser::thrift::GeoTreeElemTypes::type typeOfElement);
    bool getIsactiv() const;
    void RegisterCoordinateSystem(QString refobjname);



private:
    int                         m_traceX;
    int                         m_traceY;
    double                      m_distance;
    bool                        m_ProjectorActive;
    QString                     m_IP;
    int                         m_port;
    bool                        checkTracePoint;
    bool                        checkPoint;
    QString                     m_projectorsn;
    zlaser::thrift::Polyline    m_polyline_element;
    zlaser::thrift::TextElement m_text_element;
    PROJECTION_MODE             m_projection_mode;
    //! posisi reflector yang akan di scan
    Referenceobject             m_referenceObject;
    //! interface to common client
    ZlpThriftClient             *m_clientinterface;
    //! tracepoint model
    Vector2D                    m_crossize;
    //! current state projector
    FunctionModuleStates::type  m_oldstate;
    //! status projector
    bool                        m_isactiv;
    //! module id
    string                      m_module_id;

    //! conversion state device from string to FunctionModuleStates
    QHash<QString, FunctionModuleStates::type> *DeviceState;

    QHash<QString, bool> *CMDTOBOOL;

    //! transform polylines
    void transformPolylinesfromRefObject();


signals:
    void checkPacketTracePoint(const Referencepoint &point, bool _return);

public slots:
    //! change projector state
    void changeprojectorstate(int state);
    //! do trace point
    void startcalibration(const QString &m_projectorsn);
    //! upload object into projector
    void readyforupload(int mode);
    void Uploading(Projections *projection, int mode);
    //! switch on a projector
    void switchonprojector(const QString m_projectorsn, const QString IP, int port);
    //! switch off
    void switchoffprojector(const QString m_projectorsn);
    //! load new Referenceobject
    void LoadReferenceObject(const QString coordname, const QString referenceObjectName);
    //! remove a Referenceobject
    void removeReferenceObject(const QString coordname, const QString referenceObjectName);

    void slotSetMode(int mode);

};

#endif // OUTBOUNDPACKET_H
