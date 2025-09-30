#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <QVector>
#include <QPoint>
#include <QPolygonF>

struct HullResult {
    QPolygonF hull;
    long long iterations = 0;
};

class ConvexHull
{
public:
    static HullResult slowConvexHull(const QVector<QPoint>& points);
    static HullResult fastConvexHull(const QVector<QPoint>& points);

private:
    // Helper function to determine the orientation of an ordered triplet (p, q, r).
    static double orientation(const QPointF &p, const QPointF &q, const QPointF &r);
};

#endif // CONVEXHULL_H
