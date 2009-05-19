
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Bullestock");
    QCoreApplication::setOrganizationDomain("bullestock.net");
    QCoreApplication::setApplicationName("NXT Remote");

    MainWindow window;
    window.show();
    return app.exec();
}
