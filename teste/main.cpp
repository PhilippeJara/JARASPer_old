#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w, *wp;
    wp = &w;
    w.cl.bindToWindow(wp);
  w.show();
  return a.exec();
}
