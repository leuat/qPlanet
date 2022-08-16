#include "meshinstance.h"

MeshInstance::MeshInstance()
{

}

void MeshInstance::Render(QMatrix4x4 projection)
{

    // Set modelview-projection matrix
    if (m_material==nullptr)
        return;
    if (m_mesh==nullptr)
        return;

    m_material->bind(projection * getMV(),m_rotation.normalized().toRotationMatrix());
//    qDebug() << m_rotation;
    m_mesh->Render(m_material->program);

    m_material->release();

    Entity::Render(getMV());

}
