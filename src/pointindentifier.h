#ifndef POINTINDENTIFIER_H
#define POINTINDENTIFIER_H

#include <string>
#include <QString>
#include "interface_types.h"

using namespace std;
using namespace zlaser::thrift;

class ElementUtils{
public:
    static Vector2D CreatePointXY(double X, double y);
    static Vector3D CreatePointXYZ(double X, double Y, double Z);
    static std::vector<Vector3D> CreateLine(const Vector3D &pointA, const Vector3D &pointB);
};


class PointIndentifier
{
public:
    QString m_name;
    Vector3D m_point;
    PointIndentifier(const QString name, const Vector3D point);
};




#endif // POINTINDENTIFIER_H
