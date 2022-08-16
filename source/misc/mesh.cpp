#include "mesh.h"

Mesh::Mesh(): indexBuf(QOpenGLBuffer::IndexBuffer)

{

}

void Mesh::Render(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();
    if (arrayBuf.size()==-1 || indexBuf.size()==-1) {
        qDebug() << "Mesh::Render indexbuf / arraybuf size -1";
        indexBuf.release();
        arrayBuf.release();
        return;
    }


    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, Q_OFFSETOF(VertexData, position), 3, sizeof(VertexData));


    int normalLoc = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLoc);
    program->setAttributeBuffer(normalLoc, GL_FLOAT, Q_OFFSETOF(VertexData, normal), 3, sizeof(VertexData));

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, Q_OFFSETOF(VertexData,texCoord), 2, sizeof(VertexData));


    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_SHORT, nullptr);
//    qDebug() << indices.count();
    arrayBuf.release();
    indexBuf.release();
}

void Mesh::Build()
{
    arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBuf.create();
    arrayBuf.bind();

    arrayBuf.allocate(&data[0], data.size() * sizeof(VertexData));
    arrayBuf.release();

    // Transfer index data to VBO 1
    indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.count() * sizeof(GLushort));

    indexBuf.release();

}

MeshBox::MeshBox(float r, int n)
{

    auto p1 = QVector3D(-r,-r,-r);
    auto p2 = QVector3D(r,-r,-r);
    auto p3 = QVector3D(r,r,-r);
    auto p4 = QVector3D(-r,r,-r);
    auto p5 = QVector3D(-r,-r,r);
    auto p6 = QVector3D(r,-r,r);
    auto p7 = QVector3D(r,r,r);
    auto p8 = QVector3D(-r,r,r);

    generatePlane(p4,p3,p2,p1,n);
    generatePlane(p5,p6,p7,p8,n);
    generatePlane(p8,p7,p3,p4,n);
    generatePlane(p6,p5,p1,p2,n);

    generatePlane(p4,p1,p5,p8,n);
    generatePlane(p2,p3,p7,p6,n);

    Build();

}

void MeshBox::generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n) {
    QVector3D dx = (p2-p1)/(n-1);
    QVector3D dy = (p4-p1)/(n-1);
    //var vloc = PoolVector3Array()
    //   int nt = omp_get_num_threads();
   // data.resize(n*n);
    int vcount = data.size();
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            auto v = p1 + dx*i + dy*j;
            VertexData vd;
            vd.normal = QVector3D::crossProduct(dx,dy).normalized();
            vd.position = v;
            vd.texCoord = QVector2D(i,j);
            data.append(vd);
        }
    }
    for (int i=0;i<n-1;i++) {
        for (int j=0;j<n-1;j++) {
            indices.append(vcount+i*n+j);
            indices.append(vcount+(i+1)*n+j);
            indices.append(vcount+i*n+j+1);


            indices.append(vcount+i*n+j+1);
            indices.append(vcount+(i+1)*n+j);
            indices.append(vcount+(i+1)*n+(j+1));
        }
    }
//    qDebug() << indices.count()/6 << data.size();
}
