#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <QObject>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QApplication>
#include <QDebug>

#define PATH_ICONS       ":/icons/"

QT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

namespace Ui {
class ToolBars;
}
QT_END_NAMESPACE

#define VIEW_MODE_OBJECT    ToolBars::STATE_ICO::OBJECT
#define VIEW_MODE_TRACE     ToolBars::STATE_ICO::TRACE

class MainWindow;

class ToolBars : public QToolBar
{
    Q_OBJECT
public:
    enum ToolBarAction {
        NEW,
        SAVE,
        DEVICEMANAGER,
        ZOOMIN,
        ZOOMOUT,
        TRACE_OBJECT,
        HELP,
        ABOUT
    };

    enum STATE_ICO{
        TRACE,
        OBJECT
    };
    explicit ToolBars(QMainWindow *parent = nullptr);
    void disable(const int action, const bool block);
    void enable(const int action, const bool blok);
    void set_mode(STATE_ICO state){ m_state_view = state; }

    void reset();
    QAction *get(const int action);
    void toggle(const int action, const bool check, const bool block);

private:

    STATE_ICO m_state_view = VIEW_MODE_OBJECT;

    QAction *m_new;
    QAction *m_save;
    QAction *m_help;
    QAction *m_about;
    QAction *m_devicemanager;
    QAction *m_trace_object;
    QAction *m_zoomin;
    QAction *m_zoomout;

    static QIcon icon(const char *context, const char *name)
    {
        QString path(PATH_ICONS);
        Q_UNUSED(context);
        path.append(context);
        path.append('/');
        path.append(name);
        path.append(".png");
        return QIcon(path);
    }

    static QIcon icon(const char *name)
    {
        return icon("Toolbars", name);
    }

    static QString text(const char *context, const char *key)
    {
        return QApplication::translate(context, key);
    }

    static QString text(const char *key)
    {
        return text("Toolbars", key);
    }


signals:
    void action(int);

public slots:
    void SlotNew()
    {
        emit action(NEW);
    }
    void SlotSave()
    {
        emit action(SAVE);
    }
    void SlotHelp()
    {
        emit action(HELP);
    }
    void SlotAbout()
    {
        emit action(ABOUT);
    }
    void SlotDeviceMaager()
    {
        emit action(DEVICEMANAGER);
    }
    void SlotTraceObject()
    {
        emit action(TRACE_OBJECT);
    }
    void SlotZoomIn()
    {
        emit action(ZOOMIN);
    }
    void SlotZoomOut()
    {
        emit action(ZOOMOUT);
    }


};

#endif // TOOLBARS_H
