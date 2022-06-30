#ifndef WORLD_H
#define WORLD_H

#include "source/planet.h"
#include "source/planetdata.h"
class World
{
public:
    World();

    double time = 0;
    PointLight sun;
    QVector<QSharedPointer<Planet>> planets;
    void Init();
    void Render(QMatrix4x4 projection,QOpenGLShaderProgram *program);

};

#endif // WORLD_H
