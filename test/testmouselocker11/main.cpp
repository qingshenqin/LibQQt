#include "mainwindow.h"
#include <QQtApplication>

int main ( int argc, char* argv[] )
{
    QQtApplication::setHighDpiScaling();
    QQtApplication a ( argc, argv );
    MainWindow w;
    w.show();

    return a.exec();
}
