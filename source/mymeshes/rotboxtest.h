#ifndef ROTBOXTEST_H
#define ROTBOXTEST_H

#include "source/engine/meshes/meshinstance.h"
#include "source/engine/sdata.h"

class RotBoxTest : public MeshInstance
{
public:
    RotBoxTest();
    float time;
    void Update() override {
        QQuaternion rot = QQuaternion::fromEulerAngles(time*0.3,
                                                       time*0.4,
                                                       time*0.5);
       m_rotation = rot;
       time+=1;
    }
};

#endif // ROTBOXTEST_H
