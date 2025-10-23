#ifndef CURVESMOOTHING_H
#define CURVESMOOTHING_H

#include <QVector2D>
#include <QVector>

class CurveSmoothing
{
public:
    CurveSmoothing();

    /**
     * @brief Smooth
     * @param angle between 0 and 1, preferable 0.5
     * @param smoothing - line distance smoothing strength, 1=no smoothing, higher values = more smoothing
     * @param splineResolution number of evaluated spline points per subcurve
     */

    static void Smooth(float angle, int smoothing, int splineResolution);


};

#endif // CURVESMOOTHING_H
