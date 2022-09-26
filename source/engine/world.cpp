#include "world.h"
#include "source/engine/meshes/meshfactory.h"


World::World()
{
    m_root = QSharedPointer<Entity>(new Entity());
    m_entityList["root"] = m_root.get();
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
    m_camera.UpdatePhysics();
    m_camera.MovePhysics(0.1);
}

MeshInstance* World::AddMeshInstance(MeshInstance* mi_org, QString name, QString parent, QVector3D pos, QString mesh, Material *material)
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
    auto mi = MeshInstanceFactory::Create(mi_org, name,pos,m_meshes[mesh].get(),material);

    m_entityList[parent]->m_children.append(QSharedPointer<MeshInstance>(mi));
    m_entityList[name] = mi;
    return mi;
}

void World::Update()
{
    for (auto& n:m_entityList.keys()) {
        m_entityList[n]->Update();
    }
}
