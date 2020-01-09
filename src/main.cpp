#include <QApplication>
#include <QStyleFactory>
#include <iostream>
#include <string>
#include "mainwindow.h"


using namespace std;

void searchAndActivateProjector(const string ip, const int port);
void transferLicense(const string ip, const int port, const string license);
void registerProjector(const string ip, const int port, const bool doPointSearch);
void createProjection(const string ip, const int port);
;

void printProgramUsage()
{
    printf(
        "Usage: zlp-cpp-sdk [ip of zlp-service|localhost] [port of zlp-service|9090] <example number> <license file "
        "for example 2>\n");
    printf("Examples:\n");
    printf("1: Search and activate projector\n");
    printf("2: Transfer license file and obtain license information\n");
    printf("3: Create custom coordinate system without reference point search\n");
    printf("4: Create custom coordinate system with reference point search\n");
    printf("5: Create projection\n");
}

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    MainWindow m_window ;
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    m_window.show();


    return a.exec();
}
