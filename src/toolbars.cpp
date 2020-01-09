#include "toolbars.h"




ToolBars::ToolBars(QMainWindow *parent) : QToolBar(parent)
{
    //! construct QAction
    this->m_new     = new QAction(icon("New_32x32"), text("New"), parent);
    this->m_save    = new QAction(icon("Save_32x32"), text("Save"), this);
    this->m_help    = new QAction(icon("Index_32x32"), text("help"), this);
    this->m_about   = new QAction(icon("Feature_32x32"), text("About"), this);
    this->m_devicemanager = new QAction(icon("IDE_32x32"), text("Device Manager"), this);

    if(m_state_view == VIEW_MODE_OBJECT)
        this->m_trace_object = new QAction(icon("Object_32x32"), text("Object"), this);
    if(m_state_view == VIEW_MODE_TRACE)
        this->m_trace_object = new QAction(icon("trace_32x32"), text("Trace"), this);

    this->m_zoomin  = new QAction(icon("ZoomIn_32x32"), text("Zoom +"), this);
    this->m_zoomout = new QAction(icon("ZoomOut_32x32"), text("Zoom -"), this);

    //! space tools
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum, QSizePolicy::CheckBox);
    QWidget *space = new QWidget(this);
    space->setSizePolicy(sizePolicy);

    //! action
    this->addAction(this->m_new);
    this->addAction(this->m_save);
    this->addSeparator();
    this->addAction(this->m_zoomin);
    this->addAction(this->m_zoomout);
    this->addSeparator();
    this->addAction(this->m_trace_object);
    this->addAction(this->m_devicemanager);
    this->addAction(addWidget(space));
    this->addSeparator();
    this->addAction(this->m_help);
    this->addAction(this->m_about);

    /*
     * @event handler
     */
    this->connect(this->m_new, SIGNAL(triggered()), this, SLOT(SlotNew()));
    this->connect(this->m_save, SIGNAL(triggered()), this, SLOT(SlotSave()));
    this->connect(this->m_trace_object, SIGNAL(triggered()), this, SLOT(SlotTraceObject()));
    this->connect(this->m_zoomin, SIGNAL(triggered(bool)), this, SLOT(SlotZoomIn()));
    this->connect(this->m_zoomout, SIGNAL(triggered()), this, SLOT(SlotZoomOut()));
    this->connect(this->m_devicemanager, SIGNAL(triggered()), this, SLOT(SlotDeviceMaager()));
    this->connect(this->m_help, SIGNAL(triggered()), this, SLOT(SlotHelp()));
    this->connect(this->m_about,SIGNAL(triggered()), this, SLOT(SlotAbout()));

    setObjectName("CustomToolBar");
    this->setAcceptDrops(false);
    this->setMovable(false);
    setGeometry(0,0,500,30);
    setFloatable(true);
}

void ToolBars::disable(const int action, const bool block)
{
    QAction *pointer = this->get(action);
    if (pointer != 0) {
        if (block)
            this->blockSignals(true);
        pointer->setEnabled(false);
        if (block)
            this->blockSignals(false);
    }

}

void ToolBars::enable(const int action, const bool blok)
{
    QAction *act = this->get(action);
    if(act != 0)
    {
        if(blok)
            this->blockSignals(true);
        act->setEnabled(true);
        if(blok)
            this->blockSignals(false);
    }

}

QAction *ToolBars::get(const int action)
{
    switch (action) {
    case NEW:           return this->m_new;
    case SAVE:          return this->m_save;
    case HELP:          return this->m_help;
    case ABOUT:         return this->m_about;
    case TRACE_OBJECT:       return this->m_trace_object;
    case DEVICEMANAGER: return this->m_devicemanager;
    case ZOOMIN:        return this->m_zoomin;
    case ZOOMOUT:       return this->m_zoomout;
    default:            return 0;
    }
}
