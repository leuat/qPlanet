#include "quadplane.h"
#include <omp.h>

void QuadPlane::generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, PlanetData &pd) {
    data.clear();
    indices.clear();
    QVector3D dx = (p2-p1)/(n-1);
    QVector3D dy = (p4-p1)/(n-1);
    //var vloc = PoolVector3Array()
    int nt = omp_get_num_threads();
    data.resize(n*n);
#pragma omp parallel for
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            auto v = p1 + dx*i + dy*j;
            v = v.normalized();
            v = pd.get_surface_vector(v);
            VertexData vd;
            vd.normal = pd.get_surface_normal(v);
//            vd.normal = v.normalized();

            vd.position = v;
            vd.texCoord = QVector2D(i,j);
//            data.append(vd);
            data[i*n + j] = vd;
        }
    }
    int vcount = 0;
    for (int i=0;i<n-0;i++) {

        for (int j=0;j<n-0;j++) {
            indices.append(vcount+i*n+j);
            indices.append(vcount+i*n+j+1);
            indices.append(vcount+(i+1)*n+j);

            indices.append(vcount+(i+1)*n+j);
            indices.append(vcount+i*n+j+1);
            indices.append(vcount+(i+1)*n+(j+1));
        }
    }
    Bind();
}

void QuadPlane::Render(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();


    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));


    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 5, 2, sizeof(VertexData));

    int normalLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLoc);
    program->setAttributeBuffer(normalLoc, GL_FLOAT, 4, 3, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, indices.count(), GL_UNSIGNED_SHORT, nullptr);
}

void QuadPlane::Bind()
{
    arrayBuf.bind();

    arrayBuf.allocate(&data[0], data.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.count() * sizeof(GLushort));

}

QuadPlane::QuadPlane()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

}

QuadPlane::~QuadPlane() {
    arrayBuf.destroy();
    indexBuf.destroy();

}
