#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "graph.h"

class MainWindow : public QMainWindow {

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void readXmlFile(const QString &inputXmlFile);
    void readBinaryFile(const QString &inputBinFile);

private:
    void selectXmlFile();
    void selectBinaryFile();

    PlotWidget *m_plotWidget;
    QVBoxLayout *m_layout;
    QWidget *m_centralWidget;

    QPushButton *m_buttonForXml;
    QPushButton *m_buttonForBin;
};

#endif
