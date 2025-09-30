#include "convexhull.h"
#include <algorithm>
#include <QMap>

bool operator<(const QPoint& a, const QPoint& b)
{
    if (a.x() != b.x()) {
        return a.x() < b.x();
    }
    return a.y() < b.y();
}

double ConvexHull::orientation(const QPointF &p, const QPointF &q, const QPointF &r)
{
    return (q.x() - p.x()) * (r.y() - p.y()) - (q.y() - p.y()) * (r.x() - p.x());
}

HullResult ConvexHull::slowConvexHull(const QVector<QPoint>& points)
{
    HullResult result;
    result.iterations = 0;
    int n = points.size();
    if (n < 3) return result;

    QVector<QPair<QPoint, QPoint>> hullEdges;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            const QPoint& p = points[i];
            const QPoint& q = points[j];
            bool valid = true;
            for (int k = 0; k < n; ++k) {
                if (k == i || k == j) continue;

                result.iterations++;

                const QPoint& r = points[k];
                if (orientation(p, q, r) < 0) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                hullEdges.append({p, q});
            }
        }
    }

    if (hullEdges.isEmpty()) return result;

    QMap<QPoint, QPoint> edgeMap;
    for (const auto& edge : hullEdges) {
        edgeMap[edge.first] = edge.second;
    }

    QPoint startPoint = hullEdges[0].first;
    QPoint currentPoint = startPoint;

    for(int i = 0; i < hullEdges.size() && !edgeMap.isEmpty(); ++i) {
        result.hull.append(currentPoint);
        if(!edgeMap.contains(currentPoint)) break;
        currentPoint = edgeMap.take(currentPoint);
        if(currentPoint == startPoint) break;
    }
    return result;
}

HullResult ConvexHull::fastConvexHull(const QVector<QPoint>& points)
{
    HullResult result;
    result.iterations = 0;
    int n = points.size();
    if (n < 3) return result;

    QVector<QPoint> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const QPoint &a, const QPoint &b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });

    QVector<QPoint> upperHull;
    upperHull.append(sortedPoints[0]);
    upperHull.append(sortedPoints[1]);

    for (int i = 2; i < n; ++i) {
        upperHull.append(sortedPoints[i]);
        while (upperHull.size() > 2 && orientation(upperHull[upperHull.size()-3], upperHull[upperHull.size()-2], upperHull.back()) >= 0) {
            result.iterations++;
            upperHull.remove(upperHull.size() - 2);
        }
    }

    QVector<QPoint> lowerHull;
    lowerHull.append(sortedPoints[n-1]);
    lowerHull.append(sortedPoints[n-2]);
    for (int i = n - 3; i >= 0; --i) {
        lowerHull.append(sortedPoints[i]);
        while (lowerHull.size() > 2 && orientation(lowerHull[lowerHull.size()-3], lowerHull[lowerHull.size()-2], lowerHull.back()) >= 0) {
            result.iterations++;
            lowerHull.remove(lowerHull.size() - 2);
        }
    }

    for (int i = 0; i < upperHull.size(); ++i) {
        result.hull.append(upperHull[i]);
    }
    for (int i = 1; i < lowerHull.size() - 1; ++i) {
        result.hull.append(lowerHull[i]);
    }

    return result;
}

