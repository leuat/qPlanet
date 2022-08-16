#ifndef MESH_H
#define MESH_H

#include "entity.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct VertexData
{
    QVector3D normal;
    QVector3D position;
    QVector2D texCoord;
};

class Mesh
{
public:
    QVector<VertexData> data;
    QVector<GLushort> indices;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    void Render(QOpenGLShaderProgram *program);

    void Build();

    Mesh();
};

class MeshBox : public Mesh {
public:
    MeshBox(float scale, int n);
    void generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n);

};

#endif // MESH_H
