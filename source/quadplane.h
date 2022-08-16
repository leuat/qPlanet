#ifndef QUADPLANE_H
#define QUADPLANE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include "source/misc/SimplexNoise.h"
#include "source/planetdata.h"
#include "misc/mesh.h"

class QuadPlane : public Mesh
{
public:

      void generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, PlanetData& pd);






    QuadPlane();
    ~QuadPlane();

};

#endif // QUADPLANE_H
