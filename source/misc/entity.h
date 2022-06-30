#ifndef ENTITY_H
#define ENTITY_H

#include <QVector3D>

class Entity
{
public:
    QVector3D camera, target;
    QVector3D up = QVector3D(0,1,0);


    Entity();
};

#endif // ENTITY_H
