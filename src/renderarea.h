#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QMetaEnum>
#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>

#include "veselectionrect.h"
#include "labelcorner.h"
#include "pointindentifier.h"

#define POLY RenderArea::getInstance()->type_polygon();

class LabelCorner;

class RenderArea : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit RenderArea(QObject *parent = nullptr);
    ~RenderArea();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    //! set new PointIndentifier form MainWindow
    void setPreviousPointIndentifier(PointIndentifier *PreviousPointIndentifier);

    void setType_polygon(const VESelectionRect::TYPE_POLYGON &type_polygon);

    VESelectionRect::TYPE_POLYGON type_polygon() const;

    static RenderArea *getInstance();

public slots:
    //! slot which Point will be change not multiple move
    void SlotSetLabelType(int _type, bool leftbuttonpressed);

signals:
    //! position corner change
    //! signal position or Point changed
    void signalUpdatePointIndentifier(PointIndentifier *PreviousPointIndentifier);

private:
    int m_currentSelectPoint;

    static RenderArea *instance;

    LabelCorner::LABEL_TYPE m_LabelType;

    QPointF m_CurrentPosition;

    QPointF m_PreviousPosition;

    bool m_leftMouseButtonPressed;

    VESelectionRect::TYPE_POLYGON type;

    //! structure potition of corner will use when mouse moved
    PointIndentifier *m_PreviousPointIndentifier;


protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};


#endif // RENDERAREA_H
