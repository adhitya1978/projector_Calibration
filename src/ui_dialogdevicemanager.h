/********************************************************************************
** Form generated from reading UI file 'dialogdevicemanager.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGDEVICEMANAGER_H
#define UI_DIALOGDEVICEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogDeviceManager
{
public:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogDeviceManager)
    {
        if (DialogDeviceManager->objectName().isEmpty())
            DialogDeviceManager->setObjectName(QStringLiteral("DialogSave"));
        DialogDeviceManager->resize(470, 290);
        DialogDeviceManager->setMinimumSize(QSize(470, 290));
        DialogDeviceManager->setMaximumSize(QSize(470, 291));
        verticalLayout_4 = new QVBoxLayout(DialogDeviceManager);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(DialogDeviceManager);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tableWidget = new QTableWidget(groupBox);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMinimumSize(QSize(250, 200));

        horizontalLayout_3->addWidget(tableWidget);


        verticalLayout_4->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(DialogDeviceManager);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout_4->addWidget(buttonBox);


        retranslateUi(DialogDeviceManager);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogDeviceManager, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogDeviceManager, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogDeviceManager);
    } // setupUi

    void retranslateUi(QDialog *DialogDeviceManager)
    {
        DialogDeviceManager->setWindowTitle(QApplication::translate("DialogDeviceManager", "Dialog", 0));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogDeviceManager: public Ui_DialogDeviceManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGDEVICEMANAGER_H
