#include "workspaceview.h"

WorkspaceView::WorkspaceView(QWidget *parent) :
    QGraphicsView(parent)
{
    //this->rotate(90);
}

void WorkspaceView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}

void WorkspaceView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}

