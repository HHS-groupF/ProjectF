#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication laadt de GUI (Graphical User Interface) modules
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}