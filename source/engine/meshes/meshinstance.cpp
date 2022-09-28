#include "meshinstance.h"
#include "source/engine/misc/util.h"

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
//    qDebug() <<m_mesh->children.count();
    for (auto& c:m_mesh->children) {
        if (extraMats.contains(c->name))
            m_material->setDefaults(extraMats[c->name]);

        c->Render(m_material->program);
        if (extraMats.contains(c->name))
            m_material->setDefaults(m_material->mData);

    }

    m_material->release();

    Entity::Render(getMV());

}
