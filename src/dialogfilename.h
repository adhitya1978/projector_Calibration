#ifndef DIALOGFILENAME_H
#define DIALOGFILENAME_H

#include <QDialog>

namespace Ui {
class DialogFileName;
}

class DialogFileName : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFileName(QWidget *parent = 0);
    ~DialogFileName();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void set_new_name_cordinatesystem(const QString coordname, const QString refname);

private:
    Ui::DialogFileName *ui;
};

#endif // DIALOGFILENAME_H
