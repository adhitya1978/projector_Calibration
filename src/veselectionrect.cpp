#include "veselectionrect.h"
#include <QPen>
#include <QBrush>
#include <QPolygon>
#include <QDebug>

VESelectionRect::VESelectionRect(QObject *parent) :
    QObject(parent),
    QGraphicsItem()
{
    //setBrush(QBrush(QColor(158,182,255,96)));
    //setPen(QPen(QColor(158,182,255,200),1));
    this->setRotation(180);
}

VESelectionRect::~VESelectionRect()
{
}

int VESelectionRect::type() const
{
    return Type;
}

void VESelectionRect::setType(const TYPE_POLYGON &type)
{
    _type = type;
}


QRectF VESelectionRect::boundingRect() const
{
    return QRectF(-150, -150, 300, 300);
}

void VESelectionRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<QPoint> Points_for_ui;
    if(_type == TYPE_POLYGON::LANDSCAPE){
        QPoint TopLeft(-147.5, 105);
        QPoint BottomRight(147.5, -105);


        Points_for_ui.append(TopLeft);
        Points_for_ui.append(BottomRight);
    }
    if(_type == TYPE_POLYGON::PORTRAIT){
        QPoint TopLeft(-105, 147.5);
        QPoint BottomRight(105 , -147.5);


        Points_for_ui.append(TopLeft);
        Points_for_ui.append(BottomRight);
    }
    //! construct topleft
    //! construct bottom right

    //! construct rectangle
    QRect rectangle(Points_for_ui.at(0), Points_for_ui.at(1)); //! for triger position label
    QPolygon polygon(rectangle, true);// Using Polygon class, to draw the rectangle

    emit SignalLabelBottomLeft("T1",    rectangle.bottomLeft());
    emit SignalLabelTopLeft("T2",       rectangle.topLeft());
    emit SignalLabelTopRight("T3",      rectangle.topRight());
    emit SignalLabelBottomRight("T4",   rectangle.bottomRight());


    painter->setBrush(QBrush(QColor(158,182,255,96)));     // We set the brush, which will render the object
    painter->drawPolygon(polygon);  // Draw a triangle on a polygonal model

    Q_UNUSED(option);
    Q_UNUSED(widget);

}

