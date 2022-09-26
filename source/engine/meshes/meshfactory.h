#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "source/engine/meshes/meshinstance.h"



class MeshInstanceFactory
{
public:
    MeshInstanceFactory();
    enum MeshType {Box};
    enum MaterialType{Flat};
    static MeshInstance* Create(MeshInstance* mi,QString name, QVector3D pos, Mesh* mesh, Material* material);
};

#endif // MESHFACTORY_H
