#include "graph.h"
#include <QPainter>
#include <QFile>
#include <QDataStream>
#include <QtConcurrent/QtConcurrent>
#include <iostream>
#include <QMessageBox>
#include <QMutexLocker>
#include <cmath>
#include <QThread>

PlotWidget::PlotWidget(QWidget *parent)
        : QWidget(parent), MAX_SIGNAL_POINTS(16000), m_maxXmlCord(0.0), m_stopReading(false), m_errorReported(false){

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &PlotWidget::updateGraph);
    m_timer->start(100);

    connect(this, &PlotWidget::errorOccurred, this, [&](const QString &message){
        QMessageBox::critical(this, "Error", message);
    });
}


void PlotWidget::readXmlFile(const QString &inputXmlFile) {

    QFile file(inputXmlFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "File opening error";
        emit errorOccurred("File opening error");
        return;
    }

    QXmlStreamReader streamForXmlData(&file);
    m_markers.clear();

    while (!streamForXmlData.atEnd() && !streamForXmlData.hasError()) {
        streamForXmlData.readNext();
        if (streamForXmlData.isStartElement() && streamForXmlData.name() == "Point") {
            double x = streamForXmlData.attributes().value("X").toDouble();
            double y = streamForXmlData.attributes().value("Y").toDouble();

            m_markers.append(QPointF(x, y));

            m_maxXmlCord = std::max(abs(std::max(abs(x),abs(y))), m_maxXmlCord);
        }
    }

    if (streamForXmlData.hasError()) {
        std::cerr << "Fail with parsing XML";
        emit errorOccurred(QString("Fail with parsing XML:"));
    }
    file.close();

}

void PlotWidget::readBinaryFile(const QString &inputBinFile) {
    QtConcurrent::run([this, inputBinFile]() {

        QFile file(inputBinFile);

        if (!file.open(QIODevice::ReadOnly)) {
            std::cerr << "Binary file opening error\n";
            emit errorOccurred("Binary file opening error");
            return;
        }

        QDataStream streamForBinData(&file);
        streamForBinData.setByteOrder(QDataStream::LittleEndian);

        while (!m_stopReading) {
            QVector<QPointF> new_signalPoints;
            int16_t x, y;

            for (int i = 0; i < 1000; ++i) {
                if (!streamForBinData.readRawData(reinterpret_cast<char *>(&x), sizeof(x)) ||
                    !streamForBinData.readRawData(reinterpret_cast<char *>(&y), sizeof(y))) {

                    if (!file.seek(0)) {
                        if (!m_errorReported) {
                            std::cerr << "Failed to reset file position\n";
                            emit errorOccurred("Failed to reset file position");
                            m_errorReported = true;
                            m_stopReading = true;
                        }
                        break;
                    }
                    streamForBinData.device()->reset();
                    break;
                }
                new_signalPoints.append(QPointF(
                        x / (AVERAGE_MAXIMUM_BINARY_VALUE / m_maxXmlCord),
                        y / (AVERAGE_MAXIMUM_BINARY_VALUE / m_maxXmlCord)
                ));
            }

            {
                std::lock_guard<std::mutex> lock(m_data_mutex);
                m_signalPoints += new_signalPoints;
                if (m_signalPoints.size() > MAX_SIGNAL_POINTS) {
                    m_signalPoints = m_signalPoints.mid(m_signalPoints.size() - MAX_SIGNAL_POINTS);
                }
            }

            if (new_signalPoints.size() == 1000) {
                m_errorReported = false;
            }

            QThread::msleep(100);
        }

        file.close();
    });
}

void PlotWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal scaleFactorX = width() / (m_maxXmlCord * 2.5);
    qreal scaleFactorY = height() / (m_maxXmlCord * 2.5);
    qreal scaleFactor = qMin(scaleFactorX, scaleFactorY);

    painter.translate(width() / 2.0, height() / 2.0);
    painter.scale(scaleFactor, scaleFactor);

    painter.setPen(QPen(Qt::black, 20 / scaleFactor));

    {
        std::lock_guard<std::mutex> lock(m_data_mutex);


        for (const auto &point: m_signalPoints) {
            painter.drawPoint(point);
        }

    }

    painter.setPen(QPen(Qt::red, 1 / scaleFactor));
    for (const auto &marker : m_markers) {
        myDrawLine(painter, marker.x() - 0.1, marker.y() - 0.1, marker.x() + 0.1, marker.y() + 0.1);
        myDrawLine(painter, marker.x() - 0.1, marker.y() + 0.1, marker.x() + 0.1, marker.y() - 0.1);
    }
}

void PlotWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

void PlotWidget::updateGraph() {
    update();
}

void PlotWidget::myDrawLine(QPainter &painter, double x1, double y1, double x2, double y2) {
    QLineF line(x1, y1, x2, y2);
    painter.drawLine(line);
}
