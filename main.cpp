#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion"); // looks good with dark color scheme

    MainWindow w;
    w.show();
    return a.exec();
}
