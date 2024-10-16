#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <QXmlStreamReader>
#include <QFile>

class PlotWidget : public QWidget {

public:
    explicit PlotWidget(QWidget *parent = nullptr);

    void readXmlFile(const QString &fileName);
    void readBinaryFile(const QString &fileName);

protected:
    QVector<QPointF> markers;
    QVector<QPointF> signalPoints;
    QMutex mutex;
    QTimer *timer;

    int maxSignalPoints;
    double maxXmlCord;
    const double averageMaximumBinaryValue = 9000.0;
    const QString binaryFile;

private:
    void updateGraph();
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    static void myDrawLine(QPainter& painter, double x1, double y1, double x2, double y2);
};

#endif
