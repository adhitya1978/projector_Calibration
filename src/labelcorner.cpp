#include "labelcorner.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

LabelCorner::LabelCorner(const QString labelname):
    QGraphicsTextItem(labelname)
{
    //this->setRotation(180);
}

void LabelCorner::setPosition_hint(const POSITION_HINT &value)
{
    position_area = value;
}

QRectF LabelCorner::boundingRect() const
{
    return QRectF(-150, -150, 300, 300);
}

void LabelCorner::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);
    QGraphicsTextItem::paint(painter, option, widget);
}

void LabelCorner::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(this->toPlainText() == "T1")m_SelectLabel = LABEL_TYPE::ONE;
        if(this->toPlainText() == "T2")m_SelectLabel = LABEL_TYPE::TWO;
        if(this->toPlainText() == "T3")m_SelectLabel = LABEL_TYPE::THREE;
        if(this->toPlainText() == "T4")m_SelectLabel = LABEL_TYPE::FOUR;
        m_leftMouseButtonPressed = true;
        emit SignalSetLabelType(m_SelectLabel, m_leftMouseButtonPressed);
    }
}


void LabelCorner::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = false;
        emit SignalSetLabelType(-1, m_leftMouseButtonPressed);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
