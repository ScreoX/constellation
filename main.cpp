#include "mainwindow.h"
#include "iostream"
#include <QApplication>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Invalid number of arguments";
        return 1;
    }

    QApplication application(argc, argv);
    MainWindow window;

    window.readFiles(argv[1], argv[2]);

    window.show();

    return QApplication::exec();
}
