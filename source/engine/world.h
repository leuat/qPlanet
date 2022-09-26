#ifndef WORLD_H
#define WORLD_H

#include "source/engine/meshes/planet.h"
#include "source/engine/meshes/planetdata.h"
#include "source/engine/entity.h"
#include "source/engine/meshes/meshinstance.h"
#include "source/engine/camera.h"
#include <QMap>

class World
{
public:
    World();

    double time = 0;
//    QVector<QSharedPointer<Planet>> planets;
    QSharedPointer<Entity> m_root;
    QMap<QString, Entity*> m_entityList;
    QMap<QString, QSharedPointer<Mesh>> m_meshes;

    Camera m_camera;

    void Render();
    MeshInstance* AddMeshInstance(MeshInstance* mi,QString name, QString parent,QVector3D pos, QString mesh, Material* material);
    void Update();
};

#endif // WORLD_H
