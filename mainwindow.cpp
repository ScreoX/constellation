#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_plotWidget(new PlotWidget(this)) {
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_buttonForXml = new QPushButton("Выбрать XML файл", this);

    m_layout = new QVBoxLayout(m_centralWidget);
    m_layout->addWidget(m_buttonForXml);
    m_layout->addWidget(m_plotWidget);

    connect(m_buttonForXml, &QPushButton::clicked, this, &MainWindow::selectXmlFile);

    m_buttonForBin = new QPushButton("Выбрать Binary файл", this);
    m_buttonForBin->hide();
    m_layout->addWidget(m_buttonForBin);

    connect(m_buttonForBin, &QPushButton::clicked, this, &MainWindow::selectBinaryFile);

    setWindowTitle("Constellation");
    resize(800, 800);
}

MainWindow::~MainWindow() = default;

void MainWindow::readXmlFile(const QString &inputXmlFile) {
    m_plotWidget->readXmlFile(inputXmlFile);
}

void MainWindow::readBinaryFile(const QString &inputBinFile) {
    m_plotWidget->readBinaryFile(inputBinFile);
}

void MainWindow::selectBinaryFile() {
    QString BinFile = QFileDialog::getOpenFileName(this, tr("Выберите binary файл"), "", tr("Bin FilesWithInputCoordinates (*.bin)"));
    if (!BinFile.isEmpty()) {
        readBinaryFile(BinFile);
        m_buttonForBin->hide();
    }
}

void MainWindow::selectXmlFile() {
    QString XmlFile = QFileDialog::getOpenFileName(this, tr("Выберите xml файл"), "", tr("XML FilesWithInputCoordinates (*.xml)"));
    if (!XmlFile.isEmpty()) {
        readXmlFile(XmlFile);
        m_buttonForXml->hide();
        m_buttonForBin->show();
    }
}
