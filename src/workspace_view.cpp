#include "workspace_view.h"


workspace_view::workspace_view( QWidget *parent):
    QWidget(parent),
    renderArea(new RenderArea)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(renderArea);
    this->setLayout(mainLayout);
    qRegisterMetaType<RenderArea::Shape>();
}

RenderArea *workspace_view::getRenderArea() const
{
    return renderArea;
}
