#ifndef WORLD_H
#define WORLD_H

#include "source/planet.h"
#include "source/planetdata.h"
#include "source/misc/entity.h"
#include "source/misc/meshinstance.h"
#include "source/engine/camera.h"
#include <QMap>

class World
{
public:
    World();

    double time = 0;
//    QVector<QSharedPointer<Planet>> planets;
    QSharedPointer<Entity> m_root;
    QMap<QString, QSharedPointer<Entity>> m_entityList;
    QMap<QString, QSharedPointer<Mesh>> m_meshes;

    Camera m_camera;

    void Render();
    MeshInstance* AddMeshInstance(QString name, QString parent,QVector3D pos, QString mesh, Material* material);
};

#endif // WORLD_H
