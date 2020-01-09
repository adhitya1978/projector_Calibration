#ifndef LABELCORNER_H
#define LABELCORNER_H

#include <QObject>
#include <QGraphicsTextItem>
#include <QPointF>

class LabelCorner: public QGraphicsTextItem
{
    Q_OBJECT
public:
    enum POSITION_HINT{
        BOTTOM_LEFT = 0,
        BOTTOM_RIGHT = 1,
        TOP_LEFT = 2,
        TOP_RIGHT = 3
    };

    enum LABEL_TYPE{
        ONE, TWO, THREE, FOUR
    };

    LabelCorner(const QString labelname);

    void setPosition_hint(const POSITION_HINT &value);

private:

    //! indicator label name is select
    LABEL_TYPE m_SelectLabel;
    //! inidicator left MouseButton long Press
    bool m_leftMouseButtonPressed;
    POSITION_HINT position_area;


signals:
    void SignalSetLabelType(int _type, bool leftbuttonpressed);


protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
};

#endif // LABELCORNER_H
