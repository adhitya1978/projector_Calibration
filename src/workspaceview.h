#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QWidget>
#include <QGraphicsView>

class WorkspaceView: public QGraphicsView
{
    Q_OBJECT

public:
   explicit WorkspaceView(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

public slots:


};

#endif // WORKSPACEVIEW_H
