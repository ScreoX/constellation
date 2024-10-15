#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), plotWidget(new PlotWidget(this)) {
    setCentralWidget(plotWidget);
    setWindowTitle("Созвездие");
    resize(800, 800);
}

void MainWindow::readFiles(const QString &inputXmlFile, const QString &inputBinFile) {
    plotWidget->readXmlFile(inputXmlFile);
    plotWidget->readBinaryFile(inputBinFile);
}

MainWindow::~MainWindow() = default;
