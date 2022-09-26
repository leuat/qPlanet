#include "meshobject.h"
#include "source/engine/misc/objloader.h"


MeshObject::MeshObject(QString filename, double scale, QVector3D shift)
{
    ObjLoader o(filename);
    o.Parse();

    QVector<QVector3D> norms;

    for (int i=0;i<o.m_vertices.count();i++) {
        VertexData vd;
        vd.position = o.m_vertices[i]*scale + shift;
        if (i<o.m_normals.count())
            vd.normal = o.m_normals[i].normalized();//o.m_normals[i];
        if (i<o.m_tx.count()) {
            vd.texCoord = o.m_tx[i];//o.m_normals[i];
        }
        norms.append(o.m_normals[i]);
       // if (i<o..count())
         //   vd.normal = o.m_normals[i];
        // no texcoords yet
        data.append(vd);
    }

    for (int i=0;i<o.m_faces.count();i++) {
        indices.append(o.m_faces[i].v1);
        indices.append(o.m_faces[i].v2);
        indices.append(o.m_faces[i].v3);
        data[o.m_faces[i].v1].normal = norms[o.m_faces[i].f1];
        data[o.m_faces[i].v2].normal = norms[o.m_faces[i].f2];
        data[o.m_faces[i].v3].normal = norms[o.m_faces[i].f3];

    }
    Build();

}
