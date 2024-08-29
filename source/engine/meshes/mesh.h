#ifndef MESH_H
#define MESH_H

#include "source/engine/entity.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

struct VertexData
{
    QVector3D normal;
    QVector3D position;
    QVector2D texCoord;
    QVector3D binormal;
    QVector3D tangent;
    QVector3D light = QVector3D(1,1,1);

};


class Mesh
{
public:
    QVector<VertexData> data;
    QVector<GLushort> indices;
    QVector<VertexData> workData;
    QVector<GLushort> workIndices;
    QOpenGLVertexArrayObject vao;
    QString name;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    bool isBuilt = false;

    QVector<QSharedPointer<Mesh>> children;

    bool usesTangent = false;

    void Render(QOpenGLShaderProgram *program);

    void BuildTangentSpace();
    void BuildNormals(bool inverted=false);

    void Build();

    Mesh();
};

class MeshBox : public Mesh {
public:
    MeshBox(float r, int n, bool build=true);
    MeshBox(float r, int n, bool f1, bool f2, bool f3, bool f4, bool f5, bool f6, QVector3D shift);
    void generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n);

};

class MeshRoom : public Mesh {
public:
    MeshRoom(int n, double scale, int walls);
    void Wall(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, float h);

};

#endif // MESH_H
