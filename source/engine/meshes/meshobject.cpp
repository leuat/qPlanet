#include "meshobject.h"
#include "source/engine/misc/objloader.h"


MeshObject::MeshObject(QString filename, double scale, QVector3D shift, bool tangentSpace)
{
    ObjLoader o(filename);
    o.Parse();
    usesTangent = tangentSpace;

    for (auto& s:o.groups.keys()) {
        auto m = QSharedPointer<MeshObject>(new MeshObject());
        m->usesTangent = usesTangent;
        if (s!="default") {
            o.groups[s]->m_vertices = o.groups["default"]->m_vertices;
            o.groups[s]->m_normals = o.groups["default"]->m_normals;
            o.groups[s]->m_tx = o.groups["default"]->m_tx;
        }
        qDebug() << s << o.groups[s]->m_vertices.count();

        m->LoadGroup(o.groups[s],scale,shift);
        m->name = s;

     //   if (s=="base_body")
    //    if (!s.contains("hair"))
        children.append(m);
    }



}

void  MeshObject::LoadGroup(QSharedPointer<ObjGroup> g,double scale, QVector3D shift) {

    /*    qDebug() << o.m_vertices.count();
    qDebug() << o.m_tx.count();
    qDebug() << o.m_faces.count()/3;
    qDebug() << " ";*/
    for (int i=0;i<g->m_vertices.count();i++) {
        VertexData vd;
        vd.position = g->m_vertices[i]*scale + shift;
        data.append(vd);
    }

    for (int i=0;i<g->m_faces.count();i++) {

        Face f = g->m_faces[i];
        indices.append(f.v1);
        indices.append(f.v2);
        indices.append(f.v3);
        //        qDebug() << f.f3 << o.m_normals.count();
        if (g->hasNormals) {
            data[f.v1].normal = g->m_normals[f.n1];
            data[f.v2].normal = g->m_normals[f.n2];
            data[f.v3].normal = g->m_normals[f.n3];
        }
        //        qDebug() << o.m_tx.count() << f.vt1 ;
        if (g->hasUV) {
//            qDebug() << "HERE";
            data[f.v1].texCoord = g->m_tx[f.vt1];
            data[f.v2].texCoord = g->m_tx[f.vt2];
            data[f.v3].texCoord = g->m_tx[f.vt3];
        }

    }
    if (!g->hasNormals)
        BuildNormals();

    if (usesTangent)
        BuildTangentSpace();

    Build();

}

MeshObject::MeshObject::MeshObject()
{

}
