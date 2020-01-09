#include "log_widget.h"

#include <QTextEdit>
#include <QFont>
#include <QPalette>

static QTextEdit *front_end = nullptr;

log_widget* log_widget::instance =nullptr;

void log_widget::create_log_widget(QGroupBox *parent, QHBoxLayout *layout)
{
  // create widget text box
  front_end = new QTextEdit(parent);
  // user font
  QFont font;
  font.setFamily("Helvetica [cronyx]");
  font.setFixedPitch(true);
  font.setPointSizeF(8);
  font.setStyleHint(QFont::TypeWriter);
  QFontMetrics metrics(font);
  // palette text black
  QPalette palette;
  palette.setColor(QPalette::Active, QPalette::Base, QColor(Qt::white));
  palette.setColor(QPalette::Inactive, QPalette::Base, QColor(Qt::white));

  front_end->setPalette(palette);
  front_end->setReadOnly(true);
  front_end->setFrameStyle(QFrame::NoFrame);
  front_end->setTabStopWidth(4 * metrics.width('8'));
  front_end->setWordWrapMode(QTextOption::NoWrap);
  layout->addWidget(front_end);

}

log_widget::log_widget(QObject *parent):
    QObject(parent)
{

}

void log_widget::widget_log_clear()
{
    front_end->clear();
}

void log_widget::insert_text(int level_color, const QString &message)
{
     QString line = QString("%1] %2")
             .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
             .arg(message);
     switch(level_color){
     case 0:
         front_end->setTextColor(Qt::red);
         front_end->append(line.trimmed());
         break;
     case 1:
         front_end->setTextColor(Qt::yellow);
         front_end->append(line.trimmed());
         break;
     default:
         front_end->setTextColor(Qt::black);
         front_end->append(line.trimmed());
         break;
     }
     front_end->repaint();
}

log_widget *log_widget::getInstance()
{

    if(instance == NULL)
        instance = new log_widget;
    return instance;
}
