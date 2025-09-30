#include "drawingcanvas.h"
#include "convexhull.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>


DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    setMinimumSize(400, 300);
}

void DrawingCanvas::clearCanvas()
{
    points.clear();
    slowHull.clear();
    fastHull.clear();
    slowHullIterations = 0;
    fastHullIterations = 0;
    hullsComputed = false;
    update();
}

void DrawingCanvas::runAlgorithms()
{
    if (points.size() < 3) {
        return;
    }

    slowHull.clear();
    fastHull.clear();

    HullResult slowResult = ConvexHull::slowConvexHull(points);
    slowHull = slowResult.hull;
    slowHullIterations = slowResult.iterations;

    HullResult fastResult = ConvexHull::fastConvexHull(points);
    fastHull = fastResult.hull;
    fastHullIterations = fastResult.iterations;

    hullsComputed = true;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pointPen(Qt::red);
    pointPen.setWidth(5);
    painter.setPen(pointPen);
    for (const QPoint &point : points)
    {
        painter.drawPoint(point);
    }

    if (hullsComputed) {
        QPen slowPen(Qt::blue, 2, Qt::DashLine);
        painter.setPen(slowPen);
        painter.drawPolygon(slowHull);

        QPen fastPen(Qt::darkGreen, 3, Qt::SolidLine);
        painter.setPen(fastPen);
        painter.drawPolygon(fastHull);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10));
        QString slowText = QString("Slow Algorithm Iterations: %1").arg(slowHullIterations);
        QString fastText = QString("Fast Algorithm Iterations: %1").arg(fastHullIterations);
        painter.drawText(10, 20, slowText);
        painter.drawText(10, 40, fastText);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        points.append(event->pos());

        if (hullsComputed) {
            hullsComputed = false;
        }
        update();
    }
}
