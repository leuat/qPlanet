#ifndef PLANETDATA_H
#define PLANETDATA_H

#include <QVector3D>



#include "source/engine/misc/SimplexNoise.h"

class PlanetData
{
public:
    PlanetData();
    SimplexNoise sn;
    double radius, scale,heightScale;
    double lacunarity=0.6, offset=0.5, gain=0.2;

    double get_surface(QVector3D v);

    QVector3D get_surface_vector(QVector3D v);


    QVector3D get_surface_normal(QVector3D v);

};

class PointLight {
public:
    QVector3D pos;
    QVector3D col;
};

#endif // PLANETDATA_H
