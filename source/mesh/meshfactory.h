#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "source/misc/meshinstance.h"



class MeshInstanceFactory
{
public:
    MeshInstanceFactory();
    enum MeshType {Box};
    enum MaterialType{Flat};
    static MeshInstance* Create(QString name, QVector3D pos, Mesh* mesh, Material* material);
};

#endif // MESHFACTORY_H
