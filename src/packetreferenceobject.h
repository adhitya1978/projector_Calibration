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
*   Class  for manage Reference Object
*
*/

#ifndef PACKETREFERENCEOBJECT_H
#define PACKETREFERENCEOBJECT_H

#include <QObject>

#include "interface_types.h"
#include "common/client.h"
#include "packetreferencepoint.h"

using namespace std;
using namespace zlaser::thrift;

class PacketReferenceObject
{
public:
    explicit PacketReferenceObject(vector<Referencepoint> referencelist,
                                   const QString projectorid,
                                   const QString coordinatename,
                                   bool isactive = false);
    ~PacketReferenceObject();

    //! change Referencepoint
    void changereferencepoint(const Referencepoint &point);
    //! add new Referencepoint
    void addreferencepoint(const Referencepoint &point);
    //! remove Referencepoint
    void removereverencepoint(const Referencepoint &point);
    //! clear Referencepoint
    void clear();
    //! set coordinate system
    void setCoordinateSystem(QString coordname);
    //! set projector id, if user want to projection in other
    void setProjectorId(QString projectorid);
    //! set Referencepoint List
    void setReferencepoints(QList<Referencepoint> referencepoints);
    //! set active coordinate system to register at service
    void setActive(bool isactive);
    //! set transformation matrix
    void setTransformationMatrix(const zlaser::thrift::Matrix4x4 matrix);
    //! get Referenceobject
    Referenceobject getPacketReferenceobject();
    //! setnmae Referenceobject
    void setname(const QString name);
    //! set new Referenceobject if we want to modify the old one
    void setReferenceobject(Referenceobject referenceObject);
    //! get actve
    bool isActive();
    //! is ReferenceObject in the list of service
    bool checkAvailableRefernceObject(const QString &coordname, const QString &refobjname);

    //! get list Referencepoints in object
    vector<Referencepoint> getReferenceList();

    //! get Polyline from reference object
    zlaser::thrift::Polyline ConstructPolyLinesFromObject();

private:
    QString m_projectorid;
    QString m_coordinatename;

    //! main referenceobject
    Referenceobject *m_referenceobject;

    //! construct default referencepoin


signals:

public slots:


};

#endif // PAKETREFERENCEOBJECT_H
