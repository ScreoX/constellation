#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), plotWidget(new PlotWidget(this)) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    buttonForXml = new QPushButton("Выбрать XML файл", this);

    layout = new QVBoxLayout(centralWidget);
    layout->addWidget(buttonForXml);
    layout->addWidget(plotWidget);

    connect(buttonForXml, &QPushButton::clicked, this, &MainWindow::selectXmlFile);

    buttonForBin = new QPushButton("Выбрать Binary файл", this);
    buttonForBin->hide();
    layout->addWidget(buttonForBin);

    connect(buttonForBin, &QPushButton::clicked, this, &MainWindow::selectBinaryFile);

    setWindowTitle("Constellation");
    resize(800, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::readXmlFile(const QString &inputXmlFile) {
    plotWidget->readXmlFile(inputXmlFile);
}

void MainWindow::readBinaryFile(const QString &inputBinFile) {
    plotWidget->readBinaryFile(inputBinFile);
}

void MainWindow::selectBinaryFile() {
    QString BinFile = QFileDialog::getOpenFileName(this, tr("Выберите binary файл"), "", tr("Bin Files (*.bin)"));
    if (!BinFile.isEmpty()) {
        readBinaryFile(BinFile);
        buttonForBin->hide();
    }
}

void MainWindow::selectXmlFile() {
    QString XmlFile = QFileDialog::getOpenFileName(this, tr("Выберите xml файл"), "", tr("XML Files (*.xml)"));
    if (!XmlFile.isEmpty()) {
        readXmlFile(XmlFile);
        buttonForXml->hide();
        buttonForBin->show();
    }
}
