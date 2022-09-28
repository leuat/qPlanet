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

};


class Mesh
{
public:
    QVector<VertexData> data;
    QVector<GLushort> indices;
    QOpenGLVertexArrayObject vao;
    QString name;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    bool isBuilt = false;

    QVector<QSharedPointer<Mesh>> children;

    bool usesTangent = false;

    void Render(QOpenGLShaderProgram *program);

    void BuildTangentSpace();
    void BuildNormals();

    void Build();

    Mesh();
};

class MeshBox : public Mesh {
public:
    MeshBox(float scale, int n);
    void generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n);

};

#endif // MESH_H
