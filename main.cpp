#include "mainwindow.h"
#include "iostream"
#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication application(argc, argv);
    MainWindow window;

    window.show();

    return QApplication::exec();
}
