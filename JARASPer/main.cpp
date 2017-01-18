#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w, *wp;
    wp = &w;
    w.cl.bindToWindow(wp);
  w.show();

    return a.exec();
}
