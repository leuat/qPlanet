#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include "mesh.h"
#include "source/engine/entity.h"

class MeshInstance : public Entity
{
public:
    MeshInstance();
    Mesh* m_mesh = nullptr;
    Material* m_material = nullptr;


    void Render(QMatrix4x4 proj) override;

};

#endif // MESHINSTANCE_H
