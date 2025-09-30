#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPolygonF>

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void clearCanvas();
    void runAlgorithms();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> points;
    QPolygonF slowHull;
    QPolygonF fastHull;
    long long slowHullIterations = 0;
    long long fastHullIterations = 0;
    bool hullsComputed = false;
};

#endif
