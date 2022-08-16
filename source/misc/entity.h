#ifndef ENTITY_H
#define ENTITY_H

#include <QVector3D>
#include <QMatrix4x4>
#include "source/misc/mesh.h"
#include "source/materials/material.h"
#include <QSharedPointer>
class Entity
{
public:
    QString m_name="";
    double m_mass = 1.0;
    QVector3D m_position = QVector3D(0,0,0);
    QVector3D m_target = QVector3D(0,0,0);
    QVector3D m_up = QVector3D(0,1,0);
    QQuaternion m_rotation;

    QVector3D m_acc = QVector3D(0,0,0);
    QVector3D m_vel = QVector3D(0,0,0);
    float speedDampening = 0.9;
    float moveForward = 0;
    float moveSide = 0;
    float moveUpdown = 0;


    void Update() {
        auto dir = (m_target-m_position).normalized();
        auto side = QVector3D::crossProduct(m_up,dir).normalized();
        m_acc = moveForward*dir + moveSide*side + moveUpdown*m_up;
    }



    void Move(double dt) {
        m_position+=m_vel*dt;
        m_target+=m_vel*dt;
        m_vel +=m_acc;
        m_vel*=speedDampening;
        m_acc = QVector3D(0,0,0);

    }

    void Accelerate(QVector3D dir) {
        m_acc+=dir/m_mass;
    }



//    void Render();
    QVector<QSharedPointer<Entity>> m_children;
    Entity();

    QMatrix4x4 getMV() {
        QMatrix4x4 matrix;
        matrix.setToIdentity();
        matrix.translate(m_position);
        matrix.rotate(m_rotation);
        return matrix;
    }

    virtual void Render(QMatrix4x4 proj);

};

#endif // ENTITY_H
