#ifndef WORKSPACE_VIEW_H
#define WORKSPACE_VIEW_H

#include <QWidget>
#include <QHBoxLayout>


#include "renderarea.h"

class workspace_view : public QWidget
{
    Q_OBJECT
public:
    workspace_view(QWidget *parent = nullptr);


    RenderArea *getRenderArea() const;

private:
    RenderArea *renderArea;


};

#endif // WORKSPACE_VIEW_H
