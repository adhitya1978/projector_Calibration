#ifndef LOG_WIDGET_H
#define LOG_WIDGET_H

#include <QObject>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QString>
#include <QDateTime>

#define DPRINT(x,y) log_widget::getInstance()->insert_text(x,y)

class log_widget : public QObject
{
   Q_OBJECT

public:
    explicit log_widget(QObject *parent = nullptr);

    void create_log_widget(QGroupBox *parent, QHBoxLayout *layout);

    void widget_log_clear();
    void insert_text(int level_color ,const QString & message );



    ///properties
    static log_widget* getInstance();

private:
    static log_widget *instance;
};

#endif // LOG_WIDGET_H
