#include "world.h"
#include "source/mesh/meshfactory.h"


World::World()
{
    m_root = QSharedPointer<Entity>(new Entity());
    m_entityList["root"] = m_root;
    m_camera.m_position = QVector3D(0,0,-5);
    m_camera.m_target = QVector3D(0,0,0);
}


void World::Render()
{
    time+=0.5;
    //program->setAttributeValue(program->attributeLocation("a_light1"), sun.pos.x(), sun.pos.y(),sun.pos.z());

/*    for (auto& p: planets) {
        p->rotation = QQuaternion::fromEulerAngles(QVector3D(time,time,time));
        p->Render(projection, program);
    }*/
    m_camera.setupViewmatrix();
    m_root->Render(m_camera.m_projection*m_camera.m_viewMatrix);
    m_camera.Update();
    m_camera.Move(0.1);
}

MeshInstance* World::AddMeshInstance(QString name, QString parent, QVector3D pos, QString mesh, Material *material)
{
    if (!m_entityList.contains(parent)) {
        qDebug() << "Error in World::AddMeshInstance: cannot find parent "+parent;
        return nullptr;
    }
    if (!m_meshes.contains(mesh)) {
        qDebug() << "Error in World::AddMeshInstance: cannot find mesh "+mesh;
        return nullptr;
    }
    auto p = m_entityList[parent];
    auto mi = MeshInstanceFactory::Create(name,pos,m_meshes[mesh].get(),material);

    m_entityList[parent]->m_children.append(QSharedPointer<MeshInstance>(mi));
    m_entityList[name] = QSharedPointer<MeshInstance>(mi);
    return mi;
}
