#ifndef QUADPLANE_H
#define QUADPLANE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include "source/misc/SimplexNoise.h"
#include "source/planetdata.h"

struct VertexData
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

class QuadPlane : protected QOpenGLFunctions
{
private:
      QVector<VertexData> data;
      QVector<GLushort> indices;

      QOpenGLBuffer arrayBuf;
      QOpenGLBuffer indexBuf;
public:

      void generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, PlanetData& pd);


      //! [2]
      void Render(QOpenGLShaderProgram *program);

      void Bind();



    QuadPlane();
    ~QuadPlane();

};

#endif // QUADPLANE_H
