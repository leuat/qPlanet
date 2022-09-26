#ifndef MESHOBJECT_H
#define MESHOBJECT_H
#include "mesh.h"

class MeshObject : public Mesh
{
public:
    MeshObject(QString filename, double scale, QVector3D shift);
};

#endif // MESHOBJECT_H
