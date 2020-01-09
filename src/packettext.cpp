#include "packettext.h"
#include <QDebug>

PacketText::PacketText(const QString text,
                       Point3D position,
                       const QString groupname,
                       const QString elementname, QObject *parent) :
    m_Type(TEXT),
    m_name(text),
    m_Position(position),
    m_groupname(groupname),
    m_elementname(elementname),
    QObject(parent)
{
    ConstructText();
}


PacketText::~PacketText()
{
    delete m_Text;
}

zlaser::thrift::TextElement *PacketText::Text() const
{
    return m_Text;
}

void PacketText::setText(TextElement text)
{
    if(m_Text)
        delete m_Text;
    m_Text = &text;
}

bool PacketText::isActive()
{
    return m_Text->activated;
}

void PacketText::setPen(Qt::PenStyle style)
{
    m_Text->__set_pen(style);
}

void PacketText::setFont(const QString fontname)
{
    m_Text->__set_font(fontname.toStdString());
}

void PacketText::setHeight(int size)
{
    m_Text->__set_height(size);
}

void PacketText::setAngle(int anglesize)
{
    m_Text->__set_angle(anglesize);
}

void PacketText::setPosition(const Vector3D position)
{
    m_Text->__set_position(position);
}

void PacketText::setLineSpacing(const double val)
{
    m_Text->__set_lineSpacing(val);
}

void PacketText::setCharSpacing(const double val)
{
    m_Text->__set_charSpacing(val);
}

bool PacketText::setCoordinateSystem(const QString coordname)
{
    std::vector<std::string> _coordinatesystems;
    //! assign new one
    _coordinatesystems.push_back(coordname.toStdString());
    m_Text->__set_coordinateSystemList(_coordinatesystems);
    return true;
}


QList<QString> PacketText::getCoordinateSystem()
{
    QList<QString> _result;
    std::vector<std::string> _coordinatesystems = m_Text->coordinateSystemList;
    for(std::string _coordinatesystem : _coordinatesystems){
        _result.append(QString::fromStdString(_coordinatesystem));
    }
    return _result;
}



void PacketText::ConstructText()
{
    m_Text = new zlaser::thrift::TextElement();
    m_Text->__set_font("Helvetica [Cronyx]");
    m_Text->__set_pen(Qt::SolidLine);
    m_Text->__set_position(m_Position);
    m_Text->__set_text(m_name.toStdString());
    m_Text->__set_charSpacing(10);
    m_Text->__set_height(30);
    m_Text->__set_angle(25);
    m_groupname.append("/");
    m_groupname.append(m_elementname);
    //mygroup/my_element
    m_Text->__set_name(m_groupname.toStdString());
}

QString PacketText::get_projection_name()
{
    return m_elementname;
}

bool PacketText::get_activated()
{
    return m_Text->activated;
}

QList<QString> PacketText::get_coordinateSystemList()
{
    QList<QString> _result;
    for(std::string coordname : m_Text->coordinateSystemList){
        _result.append(QString::fromStdString(coordname));
    }
    return _result;
}

PROJECTION_TYPE PacketText::type()
{
    return m_Type;
}



