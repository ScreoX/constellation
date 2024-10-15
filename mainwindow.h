#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"

class MainWindow : public QMainWindow {

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void readFiles(const QString &inputXmlFile, const QString &inputBinFile);

private:
    PlotWidget *plotWidget;
};

#endif
