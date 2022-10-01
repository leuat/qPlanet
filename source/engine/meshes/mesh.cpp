#include "mesh.h"
#include <QOpenGLVertexArrayObject>
#include "source/engine/misc/SimplexNoise.h"
#include "source/engine/sdata.h"
Mesh::Mesh(): indexBuf(QOpenGLBuffer::IndexBuffer)

{

}

void Mesh::Render(QOpenGLShaderProgram *program)
{

    // Tell OpenGL which VBOs to use
    if (!isBuilt)
        return;
  /*  if (!arrayBuf.bind()) {
        qDebug() << "Could not bind buffer";
        exit(1);
    }*/
    vao.bind();
    arrayBuf.bind();
    if (!indexBuf.bind()) {
        qDebug() << "Could not bind index buffer";
        exit(1);
    }
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


//    qDebug() << vertexLocation<<normalLoc<<texcoordLocation;
  //  program->setAttributeBuffer("position", GL_FLOAT,0,3);
  //  program->enableAttributeArray("position");

//    program->setAttributeBuffer(0, GL_FLOAT, Q_OFFSETOF(VertexData, position), 3, sizeof(VertexData));

    if (usesTangent) {
        int tanLoc = program->attributeLocation("a_tangent");
        program->enableAttributeArray(tanLoc);
        program->setAttributeBuffer(tanLoc, GL_FLOAT, Q_OFFSETOF(VertexData, tangent), 3, sizeof(VertexData));

        int biLoc = program->attributeLocation("a_binormal");
        program->enableAttributeArray(biLoc);
        program->setAttributeBuffer(biLoc, GL_FLOAT, Q_OFFSETOF(VertexData, binormal), 3, sizeof(VertexData));
    }



    glDrawElements(GL_TRIANGLES, indices.count(), GL_UNSIGNED_SHORT, 0);
//    glDrawArrays(GL_TRIANGLES, indices.count(), GL_UNSIGNED_SHORT);
//    qDebug() << indices.count();
    arrayBuf.release();
    indexBuf.release();
    vao.release();
}


void Mesh::BuildTangentSpace()
{
    for (int i=0;i<indices.count();i+=3) {
        auto v1 = data[indices[i+0]].position;
        auto v2 = data[indices[i+1]].position;
        auto v3 = data[indices[i+2]].position;

        auto uv1 = data[indices[i+0]].texCoord;
        auto uv2 = data[indices[i+1]].texCoord;
        auto uv3 = data[indices[i+2]].texCoord;

        auto duv1 = (uv2-uv1).normalized();
        auto duv2 = (uv3-uv1).normalized();

        auto dp1 = (v2-v1.normalized());
        auto dp2 = (v3-v1.normalized());


        float r = 1.0f / (duv1.x() * duv2.y() - duv1.y() * duv2.x());
        auto tangent = (dp1 * duv2.y()   - dp2 * duv1.y())*r;
        auto bitangent = (dp2 * duv1.x()   - dp1 * duv2.x())*r;


        tangent.normalize();
        bitangent.normalize();

        data[indices[i+0]].tangent = tangent;
        data[indices[i+1]].tangent = tangent;
        data[indices[i+2]].tangent = tangent;

        data[indices[i+0]].binormal = bitangent;
        data[indices[i+1]].binormal = bitangent;
        data[indices[i+2]].binormal = bitangent;


    }
}

void Mesh::BuildNormals(bool inverted)
{
    for (auto& d:data)
        d.normal = QVector3D(0,0,0);

    for (int i=0;i<indices.count();i+=3) {
        auto v1 = data[indices[i+0]].position;
        auto v2 = data[indices[i+1]].position;
        auto v3 = data[indices[i+2]].position;

        auto n = QVector3D::crossProduct(v2-v1,v3-v1).normalized();
        if (inverted) n*=-1;

        data[indices[i+0]].normal+=n;
        data[indices[i+1]].normal+=n;
        data[indices[i+2]].normal+=n;

    }
    for (auto& d:data)
        d.normal.normalize();

}




void Mesh::Build()
{
    if (data.size()==0)
        return;
    if (indices.count()==0)
        return;

        if (vao.create())
            vao.bind();

    arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBuf.create();
    arrayBuf.bind();

    arrayBuf.allocate(&data[0], data.size() * sizeof(VertexData));
    arrayBuf.release();

/*
    QVector<QVector3D> vo;
    for (auto& d: data)
        vo.append(d.position);

    vbuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbuf.create();
    vbuf.bind();

    vbuf.allocate(&vo[0], vo.size() * sizeof(QVector3D));
    vbuf.release();
*/
    // Transfer index data to VBO 1
    indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(&indices[0], indices.count() * sizeof(GLushort));

    indexBuf.release();
    isBuilt = true;
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

void MeshRoom::Wall(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, float h)
{
    QVector3D dx = (p2-p1)/(n-1);
    QVector3D dy = (p4-p1)/(n-1);
    //var vloc = PoolVector3Array()
    //   int nt = omp_get_num_threads();
   // data.resize(n*n);
    int vcount = data.size();

    QVector<double> grid;

    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            auto v = i + j*n;
//            float h = rand(
        }
    }



    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            auto v = p1 + dx*i + dy*j;
            VertexData vd;
            vd.normal = QVector3D::crossProduct(dx,dy).normalized();
            auto s = v*0.9;
            vd.position = v + vd.normal*SData::sdata.sn.noise(s.x(),s.y(),s.z())*0.2;
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


MeshRoom::MeshRoom(int n, double scale, int walls)
{
    float r = scale*-1;
    auto p1 = QVector3D(-r,-r,-r);
    auto p2 = QVector3D(r,-r,-r);
    auto p3 = QVector3D(r,r,-r);
    auto p4 = QVector3D(-r,r,-r);
    auto p5 = QVector3D(-r,-r,r);
    auto p6 = QVector3D(r,-r,r);
    auto p7 = QVector3D(r,r,r);
    auto p8 = QVector3D(-r,r,r);
    float h= 0;
    Wall(p4,p3,p2,p1,n,h);
    Wall(p5,p6,p7,p8,n,h);
    Wall(p8,p7,p3,p4,n,h);
    Wall(p6,p5,p1,p2,n,h);

    Wall(p4,p1,p5,p8,n,h);
    Wall(p2,p3,p7,p6,n,h);

    BuildNormals();
    BuildTangentSpace();
    Build();

}
