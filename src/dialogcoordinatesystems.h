#ifndef DIALOGCOORDINATESYSTEMS_H
#define DIALOGCOORDINATESYSTEMS_H

#include <QListWidgetItem>
#include <QDialog>
#include <string>

#include "dialogfilename.h"

using namespace std;

namespace Ui {
class DialogCoordinateSystems;
}

class DialogCoordinateSystems : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCoordinateSystems(QHash<QString, QString> referenceObjectNames, QWidget *parent = 0);
    ~DialogCoordinateSystems();
    bool exist(const QString coordinatename);
    void setNewCordinateName(const QString newname);

private:
    Ui::DialogCoordinateSystems *ui;

    //! current widget item selected by user
    QListWidgetItem *m_selectitem;

    //! current file name selected by user
    QString m_current_cordinate;

    //! dialog file name
    DialogFileName *dialogFilename;

    //! new coordinate name
    QString new_filename;

    //! reference object name
    QHash<QString,QString> m_referenceobjects;


    void constructTableWidget();

    void update();
    void deleteItem();
    void openitem(const QString &coordname, const QString &refname);
signals:
    void selectitem(const QString name);
    //!
    //! \brief Signal for load or create new coordinate system
    //! \param string Referenceobject name and coordinate system name
    //!
    void signalLoadReferenceObject(const QString &coordname, const QString &referenceObjectName);
    //!
    //! \brief signal for Removing ReferenceObject
    //! \param string coordinate name
    //! \param string name referenceObject
    //!
    void signalRemoveReferenceObject(const QString &coordname, const QString &referenceObject);


public slots:
    void set_new_name_cordinatesystem(const QString &coordname, const QString &refname);
private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_del_clicked();
    void on_pushButton_new_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_cancel_clicked();
};

#endif // DIALOGCOORDINATESYSTEMS_H
