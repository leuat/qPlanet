#include "meshfactory.h"

MeshInstanceFactory::MeshInstanceFactory()
{

}

MeshInstance *MeshInstanceFactory::Create(QString name, QVector3D pos, Mesh* mesh, Material* material)
{
    auto* mi = new MeshInstance();
    mi->m_mesh = mesh;
    mi->m_material = material;
    mi->m_position = pos;
    mi->m_name = name;
    return mi;
}
