/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "workspaceview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionSave;
    QAction *actionSaveAs;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_View;
    QHBoxLayout *horizontalLayout_2;
    WorkspaceView *workspaceView;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *layoutIP;
    QLabel *label_ip;
    QTextEdit *textEdit_ip;
    QSpacerItem *spacerPortIP;
    QVBoxLayout *layoutPort;
    QLabel *label_port;
    QTextEdit *textEdit_port;
    QPushButton *pushButton_scan;
    QHBoxLayout *layoutOnSn;
    QPushButton *pushButton_connect;
    QSpacerItem *spacerSwSn;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_Tools;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_LOG;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;


    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(700, 577);
        MainWindow->setMinimumSize(QSize(700, 400));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        groupBox_View = new QGroupBox(centralwidget);
        groupBox_View->setObjectName(QStringLiteral("groupBox_View"));
        groupBox_View->setMinimumSize(QSize(400, 400));
        groupBox_View->setMaximumSize(QSize(400, 400));
        horizontalLayout_2 = new QHBoxLayout(groupBox_View);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        workspaceView = new WorkspaceView(groupBox_View);
        workspaceView->setObjectName(QStringLiteral("workspaceView"));

        horizontalLayout_2->addWidget(workspaceView);


        horizontalLayout_3->addWidget(groupBox_View);


        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetFixedSize);
        layoutIP = new QVBoxLayout();
        layoutIP->setObjectName(QStringLiteral("layoutIP"));
        layoutIP->setSizeConstraint(QLayout::SetFixedSize);
        label_ip = new QLabel(centralwidget);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setMinimumSize(QSize(0, 10));
        label_ip->setMaximumSize(QSize(16777215, 10));

        layoutIP->addWidget(label_ip);

        textEdit_ip = new QTextEdit(centralwidget);
        textEdit_ip->setObjectName(QStringLiteral("textEdit_ip"));
        textEdit_ip->setMinimumSize(QSize(100, 20));
        textEdit_ip->setMaximumSize(QSize(200, 20));
        textEdit_ip->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit_ip->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        layoutIP->addWidget(textEdit_ip);


        horizontalLayout->addLayout(layoutIP);

        spacerPortIP = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacerPortIP);

        layoutPort = new QVBoxLayout();
        layoutPort->setObjectName(QStringLiteral("layoutPort"));
        layoutPort->setSizeConstraint(QLayout::SetFixedSize);
        label_port = new QLabel(centralwidget);
        label_port->setObjectName(QStringLiteral("label_port"));
        label_port->setMaximumSize(QSize(16777215, 10));

        layoutPort->addWidget(label_port);

        textEdit_port = new QTextEdit(centralwidget);
        textEdit_port->setObjectName(QStringLiteral("textEdit_port"));
        textEdit_port->setMinimumSize(QSize(5, 20));
        textEdit_port->setMaximumSize(QSize(200, 20));
        textEdit_port->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit_port->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        layoutPort->addWidget(textEdit_port);


        horizontalLayout->addLayout(layoutPort);


        verticalLayout->addLayout(horizontalLayout);

        pushButton_scan = new QPushButton(centralwidget);
        pushButton_scan->setObjectName(QStringLiteral("pushButton_scan"));

        verticalLayout->addWidget(pushButton_scan);

        layoutOnSn = new QHBoxLayout();
        layoutOnSn->setObjectName(QStringLiteral("layoutOnSn"));
        layoutOnSn->setSizeConstraint(QLayout::SetFixedSize);
        pushButton_connect = new QPushButton(centralwidget);
        pushButton_connect->setObjectName(QStringLiteral("pushButton_connect"));

        layoutOnSn->addWidget(pushButton_connect);

        spacerSwSn = new QSpacerItem(20, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

        layoutOnSn->addItem(spacerSwSn);

        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(150, 0));
        comboBox->setMaximumSize(QSize(100, 20));

        layoutOnSn->addWidget(comboBox);


        verticalLayout->addLayout(layoutOnSn);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(13, 70, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        groupBox_Tools = new QGroupBox(centralwidget);
        groupBox_Tools->setObjectName(QStringLiteral("groupBox_Tools"));
        groupBox_Tools->setEnabled(true);
        groupBox_Tools->setMinimumSize(QSize(0, 150));
        groupBox_Tools->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_2->addWidget(groupBox_Tools);


        horizontalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_3->addItem(verticalSpacer);

        groupBox_LOG = new QGroupBox(centralwidget);
        groupBox_LOG->setObjectName(QStringLiteral("groupBox_LOG"));
        groupBox_LOG->setMinimumSize(QSize(0, 80));
        groupBox_LOG->setMaximumSize(QSize(16777215, 80));

        verticalLayout_3->addWidget(groupBox_LOG);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuHelp->addAction(actionAbout);




        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSaveAs->setText(QApplication::translate("MainWindow", "SaveAs...", 0));
        groupBox_View->setTitle(QApplication::translate("MainWindow", "view", 0));
        label_ip->setText(QApplication::translate("MainWindow", "IP:", 0));
        label_port->setText(QApplication::translate("MainWindow", "Port:", 0));
        pushButton_scan->setText(QApplication::translate("MainWindow", "Scan", 0));
        pushButton_connect->setText(QApplication::translate("MainWindow", "...", 0));
        groupBox_Tools->setTitle(QApplication::translate("MainWindow", "Tools", 0));
        groupBox_LOG->setTitle(QApplication::translate("MainWindow", "LOG", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

class ToolBars;

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
