#include "world.h"

World::World()
{

}

void World::Init()
{
    auto p = QSharedPointer<Planet>(new Planet);
    p->initPlanetGeometry(1.5,0.1,10);
    planets.append(p);
    sun.pos = QVector3D(1,1,1);
}

void World::Render(QMatrix4x4 projection, QOpenGLShaderProgram *program)
{
    time+=0.5;
    program->setAttributeValue(program->attributeLocation("a_light1"), sun.pos.x(), sun.pos.y(),sun.pos.z());

    for (auto& p: planets) {
        p->rotation = QQuaternion::fromEulerAngles(QVector3D(time,time,time));
        p->Render(projection, program);
    }
}
