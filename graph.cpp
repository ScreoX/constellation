#include "graph.h"
#include <QPainter>
#include <QFile>
#include <QDataStream>
#include <QtConcurrent/QtConcurrent>
#include <iostream>

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent), maxSignalPoints(16000) {
    maxXmlCord = 0.0;
    
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlotWidget::updateGraph);
    timer->start(100);
}

void PlotWidget::readXmlFile(const QString &inputXmlFile) {

    QFile file(inputXmlFile);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "File opening error";
        return;
    }

    QXmlStreamReader streamForXmlData(&file);
    markers.clear();

    while (!streamForXmlData.atEnd() && !streamForXmlData.hasError()) {
        streamForXmlData.readNext();
        if (streamForXmlData.isStartElement() && streamForXmlData.name() == "Point") {
            double x = streamForXmlData.attributes().value("X").toDouble();
            double y = streamForXmlData.attributes().value("Y").toDouble();

            markers.append(QPointF(x, y));

            maxXmlCord = std::max(abs(std::max(abs(x),abs(y))), maxXmlCord);
        }
    }
    file.close();

}

void PlotWidget::readBinaryFile(const QString &inputBinFile) {

    QtConcurrent::run([this, inputBinFile]() {

        QFile file(inputBinFile);

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Binary file opening error";
            return;
        }

        QDataStream streamForBinData(&file);
        streamForBinData.setByteOrder(QDataStream::LittleEndian);

        while (!streamForBinData.atEnd()) {

            QVector<QPointF> newSignalPoints;
            int16_t x, y;

            for (int i = 0; i != 1000 && !streamForBinData.atEnd(); i++) {
                if (!streamForBinData.readRawData(reinterpret_cast<char *>(&x), sizeof(x))
                || !streamForBinData.readRawData(reinterpret_cast<char *>(&y), sizeof(y))) {
                    std::cerr << "Data reading error";
                    break;
                }
                newSignalPoints.append(QPointF(x / (averageMaximumBinaryValue / maxXmlCord), y / (averageMaximumBinaryValue / maxXmlCord)));
            }

            mutex.lock();

            signalPoints += newSignalPoints;
            if (signalPoints.size() > maxSignalPoints) {
                signalPoints = signalPoints.mid(signalPoints.size() - maxSignalPoints);
            }

            mutex.unlock();
        }
    });
}

void PlotWidget::paintEvent(QPaintEvent *event) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal scaleFactorX = width() / (maxXmlCord * 2.5);
    qreal scaleFactorY = height() / (maxXmlCord * 2.5);
    qreal scaleFactor = qMin(scaleFactorX, scaleFactorY);

    painter.translate(width() / 2.0, height() / 2.0);
    painter.scale(scaleFactor, scaleFactor);

    painter.setPen(QPen(Qt::black, 20 / scaleFactor));
    mutex.lock();

    for (const auto &point : signalPoints) {
        painter.drawPoint(point);
    }

    mutex.unlock();

    painter.setPen(QPen(Qt::red, 1 / scaleFactor));
    for (const auto &marker : markers) {
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
