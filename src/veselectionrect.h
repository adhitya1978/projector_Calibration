#ifndef SELECTIONRECT_H
#define SELECTIONRECT_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QPainter>

#include "labelcorner.h"

#define POLY_PORTRAIT VESelectionRect::TYPE_POLYGON::PORTRAIT
#define POLY_LANDSCAPE VESelectionRect::TYPE_POLYGON::LANDSCAPE

class VESelectionRect : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    enum TYPE_POLYGON{
        PORTRAIT    = 0,
        LANDSCAPE   = 1,
        UNKNOWN     = 2
    };
    explicit VESelectionRect(QObject *parent = nullptr);
    ~VESelectionRect();
    enum {
        Type = UserType + 1
    };
    int type() const override;

    void setType(const TYPE_POLYGON &type);

private:
    TYPE_POLYGON _type;

signals:

    void SignalLabelBottomLeft(QString name,QPoint);
    void SignalLabelTopLeft(QString name, QPoint);
    void SignalLabelBottomRight(QString name, QPoint);
    void SignalLabelTopRight(QString name, QPoint);

public slots:

protected:
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SELECTIONRECT_H
