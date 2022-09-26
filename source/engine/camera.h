#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include "source/engine/entity.h"

class Camera : public Entity
{
public:

    QMatrix4x4 m_matrix;
    QMatrix4x4 m_viewMatrix, m_viewMatrixInv, m_projection,  m_rotMatrix, m_invVP;
    float m_fov = 80;
    float m_aspect = 1.8;


    Camera();
    void TranslateXY(QVector3D& delta);

    void ZoomXY(float delta);

    void RotateUp(float r);


    QMatrix4x4 GetRotationMatrix();

    void RotateVertical(float angle);
    void RotateHorisontal(float angle);


    void setupViewmatrix();

    QVector3D coord2ray(float x, float y, float width, float height);
    QVector3D fisheye(float x, float y, float width, float height);


    void setRotation(QVector3D& v);

};

#endif // CAMERA_H
