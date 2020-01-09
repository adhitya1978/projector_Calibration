#ifndef VECTOR3DEXT_H
#define VECTOR3DEXT_H

#include <QObject>

#include "common/client.h"
using namespace zlaser::thrift;

class Vector3DExt : public Vector3D
{
    Q_OBJECT
public:
    explicit Vector3DExt( double x = 0, double y = 0, double z = 0);


private:
    double pointX;
    double pointY;
    double pointZ;
signals:

public slots:
};

#endif // VECTOR3DEXT_H
