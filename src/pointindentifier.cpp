#include "pointindentifier.h"

PointIndentifier::PointIndentifier(const QString name, const Vector3D point):
    m_name(name),
    m_point(point)
{

}


Vector2D ElementUtils::CreatePointXY(double X, double y)
{
    Vector2D point;
    point.__set_x(X);
    point.__set_y(y);
    return point;
}

Vector3D ElementUtils::CreatePointXYZ(double X, double Y, double Z)
{
    Vector3D point;
    point.__set_x(X);
    point.__set_y(Y);
    point.__set_z(Z);
    return point;
}

std::vector<Vector3D> ElementUtils::CreateLine(const Vector3D &pointA, const Vector3D &pointB)
{
    std::vector<Vector3D> linestring;
    linestring.push_back(pointA);
    linestring.push_back(pointB);

    return linestring;
}

