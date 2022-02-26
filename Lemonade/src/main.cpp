#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("Lemonade");

    QSurfaceFormat format;
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;
    window.show();
    return a.exec();
}
