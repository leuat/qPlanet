#ifndef MESHOBJECT_H
#define MESHOBJECT_H
#include "mesh.h"
#include "source/engine/misc/objloader.h"

class MeshObject : public Mesh
{
public:
    MeshObject(QString filename, double scale, QVector3D shift, bool tangentSpace);

    void LoadGroup(QSharedPointer<ObjGroup> grp,double scale, QVector3D shift);

    MeshObject();

};

#endif // MESHOBJECT_H
