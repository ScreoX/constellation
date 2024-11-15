#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <QXmlStreamReader>
#include <QFile>
#include <QPainter>
#include <mutex>

class PlotWidget : public QWidget {
Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr);

    void readXmlFile(const QString &fileName);
    void readBinaryFile(const QString &fileName);

protected:

    const int MAX_SIGNAL_POINTS;
    const double AVERAGE_MAXIMUM_BINARY_VALUE = 9000.0;

    QVector<QPointF> m_markers;
    QVector<QPointF> m_signalPoints;
    QTimer *m_timer;

    std::mutex m_data_mutex;
    bool m_stopReading;
    bool m_errorReported;
    double m_maxXmlCord;

private slots:
    void updateGraph();

signals:
    void errorOccurred(const QString &errorMessage);

private:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    static void myDrawLine(QPainter& painter, double x1, double y1, double x2, double y2);
};

#endif
