#include <QApplication>
#include "global.h"
#include "renderarea.h"
#include "packetreferenceobject.h"
#include "packetreferencepoint.h"

RenderArea *RenderArea::instance = NULL;

RenderArea *RenderArea::getInstance()
{
    if(instance == 0)
    {
        instance = new RenderArea;
    }
    return instance;
}

RenderArea::RenderArea(QObject *parent) :
    QGraphicsScene(parent),
    m_leftMouseButtonPressed(false)
{
}

RenderArea::~RenderArea()
{
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 200);
}

void RenderArea::SlotSetLabelType(int _type, bool leftbuttonpressed)
{
    m_LabelType = (LabelCorner::LABEL_TYPE)_type;
    m_leftMouseButtonPressed = leftbuttonpressed;
}

VESelectionRect::TYPE_POLYGON RenderArea::type_polygon() const
{
    return type;
}

void RenderArea::setType_polygon(const VESelectionRect::TYPE_POLYGON &type_polygon)
{
    type = type_polygon;
}

void RenderArea::setPreviousPointIndentifier(PointIndentifier *PreviousPointIndentifier)
{
    m_PreviousPointIndentifier = PreviousPointIndentifier;
}

//! =====================================================================
//! event block

void RenderArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // scene quadrant ++
    m_CurrentPosition = event->scenePos();
    qreal dx, dy;
    if (m_leftMouseButtonPressed){
        switch(m_LabelType){
        case LabelCorner::LABEL_TYPE::ONE:{
            m_PreviousPointIndentifier[0].m_name = "T1";
            m_PreviousPosition = QPointF(m_PreviousPointIndentifier[0].m_point.x,m_PreviousPointIndentifier[0].m_point.y);

            if(m_CurrentPosition.x() <= 0) dx = 0;
            if(m_CurrentPosition.y() <= 0) dy = 0;

            if(m_CurrentPosition.x() >= m_PreviousPosition.x()) dx = m_CurrentPosition.x();
            if(m_CurrentPosition.y() >= m_PreviousPosition.y()) dy = m_CurrentPosition.y();

            m_PreviousPointIndentifier[0].m_point = ElementUtils::CreatePointXYZ(dx, dy, 0.0);
            qDebug()<< QString("Point T1 move position: (%1,%2)").arg(m_PreviousPointIndentifier[0].m_point.x).arg(m_PreviousPointIndentifier[0].m_point.y);

        }
            break;// top left
        case LabelCorner::LABEL_TYPE::TWO:{
            m_PreviousPointIndentifier[1].m_name = "T2";
            m_PreviousPosition = QPointF(m_PreviousPointIndentifier[1].m_point.x,m_PreviousPointIndentifier[1].m_point.y);

            if(type== POLY_LANDSCAPE){
                if(m_CurrentPosition.x() <= 0)  dx = 0;
                if(m_CurrentPosition.y() <= 0)  dy = 210;
            }
            if(type == POLY_PORTRAIT){
                if(m_CurrentPosition.x() <= 0)  dx = 0;
                if(m_CurrentPosition.y() <= 0)  dy = 297;
            }
            if(m_CurrentPosition.y() >= m_PreviousPosition.y()) dy = m_CurrentPosition.y();
            if(m_CurrentPosition.x() >= m_PreviousPosition.x()) dx = m_CurrentPosition.x();

            m_PreviousPointIndentifier[1].m_point = ElementUtils::CreatePointXYZ(dx, dy, 0.0);
            qDebug()<< QString("Point T2 move position: (%1,%2)").arg(m_PreviousPointIndentifier[1].m_point.x).arg(m_PreviousPointIndentifier[1].m_point.y);
        }
            break;
        case LabelCorner::LABEL_TYPE::THREE:{// top right
            m_PreviousPointIndentifier[2].m_name = "T3";
            m_PreviousPosition = QPointF(m_PreviousPointIndentifier[2].m_point.x,m_PreviousPointIndentifier[2].m_point.y);

            if(type== POLY_LANDSCAPE){
                if(m_CurrentPosition.x() <= 0)  dx = 297;
                if(m_CurrentPosition.y() <= 0)  dy = 210;
            }
            if(type == POLY_PORTRAIT){
                if(m_CurrentPosition.x() <= 0)  dx = 210;
                if(m_CurrentPosition.y() <= 0)  dy = 297;
            }

            if(m_CurrentPosition.x() >= m_PreviousPosition.x()) dx = m_CurrentPosition.x();
            if(m_CurrentPosition.y() >= m_PreviousPosition.y()) dy = m_CurrentPosition.y();

            m_PreviousPointIndentifier[2].m_point = ElementUtils::CreatePointXYZ(dx, dy, 0.0);
            qDebug()<< QString("Point T3 move position: (%1,%2)").arg(m_PreviousPointIndentifier[2].m_point.x).arg(m_PreviousPointIndentifier[2].m_point.y);
        }
            break;
        case LabelCorner::LABEL_TYPE::FOUR:{// bottom right
            m_PreviousPointIndentifier[3].m_name = "T4";
            m_PreviousPosition = QPointF(m_PreviousPointIndentifier[3].m_point.x,m_PreviousPointIndentifier[3].m_point.y);

            if(type== POLY_LANDSCAPE){
                if(m_CurrentPosition.x() <= 0)  dx = 297;
                if(m_CurrentPosition.y() <= 0)  dy = 0;
            }
            if(type == POLY_PORTRAIT){
                if(m_CurrentPosition.x() <= 0)  dx = 210;
                if(m_CurrentPosition.y() <= 0)  dy = 0;
            }
            if(m_CurrentPosition.x() >= m_PreviousPosition.x()) dx = m_CurrentPosition.x();
            if(m_CurrentPosition.y() >= m_PreviousPosition.y()) dy = m_CurrentPosition.y();

            m_PreviousPointIndentifier[3].m_point = ElementUtils::CreatePointXYZ(dx, dy, 0.0);
            qDebug()<< QString("Point T4 move position: (%1,%2)").arg(m_PreviousPointIndentifier[3].m_point.x).arg(m_PreviousPointIndentifier[3].m_point.y);
        }
            break;
        default:QGraphicsScene::mousePressEvent(event);
            break;
        }
        emit signalUpdatePointIndentifier(m_PreviousPointIndentifier);
    }

}

void RenderArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void RenderArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}
