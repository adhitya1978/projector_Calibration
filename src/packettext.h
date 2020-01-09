#ifndef PACKETTEXT_H
#define PACKETTEXT_H
/*
 * class for manage Object text
**/
#include <QObject>
#include <QVector3D>
#include <QString>

//! SDK
#include "interface_types.h"
#include "common/client.h"

#include "projection.h"
#include "pointindentifier.h"

using namespace std;
using namespace zlaser::thrift;

class PacketText : public QObject, public Projection
{
    Q_OBJECT
public:
    explicit PacketText(const QString name,
                        Point3D position,
                        const QString groupname,
                        const QString elementname,
                        QObject *parent = nullptr);
    ~PacketText();

    //! get current text
    zlaser::thrift::TextElement *Text() const;
    //! set model Text if user need to modify one
    void setText(zlaser::thrift::TextElement text);
    //! check is there active text
    bool isActive();
    //! set Pen model
    void setPen(Qt::PenStyle);
    //! set Font model
    void setFont(const QString fontname);
    //! set height
    void setHeight(int size);
    //! set angle
    void setAngle(int anglesize);
    //! set position text
    void setPosition(const Vector3D position);
    //! set spacing
    void setLineSpacing(const double val);
    //! set char spacing
    void setCharSpacing(const double val);
    //! get element text
    zlaser::thrift::TextElement get(){ return *m_Text; }
    //! set coordinate system for current text
    bool setCoordinateSystem(const QString coordname);
    //! get coordinate system which already sign in to current text
    QList<QString> getCoordinateSystem();
    // Projection interface
    QString get_projection_name();
    bool get_activated();
    QList<QString> get_coordinateSystemList();
    PROJECTION_TYPE type();

private:
    QString m_name;
    Point3D m_Position;
    //! coordinate systems
    QList<QString> m_CoordinateSystems;
    //! main Text
    zlaser::thrift::TextElement *m_Text;
    //! group name
    QString m_groupname;
    //! element name
    QString m_elementname;

    void ConstructText();
    //! type an object
    PROJECTION_TYPE m_Type;




};

#endif // PACKETTEXT_H
