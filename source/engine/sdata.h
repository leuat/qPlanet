#ifndef SDATA_H
#define SDATA_H

#include <QVector3D>
#include <QOpenGLShader>
#include <QMap>
#include <QSharedPointer>
#include "source/engine/meshes/mesh.h"
#include "source/engine/misc/SimplexNoise.h"

class SData
{
public:
    SData();
    QMap<QString, QSharedPointer<QOpenGLShaderProgram>> shaderPrograms;

    QVector3D s_directionalLight = QVector3D(1,-1,-1);
    QVector3D s_directionalLightColor = QVector3D(1,1,1);
    double time = 0;
    static SData sdata;
    QVector3D s_eye;
    QVector3D s_eye_direction;
    QVector2D s_mpos;
    SimplexNoise sn;
    QSharedPointer<QOpenGLShaderProgram> LinkShader(QString vert, QString frag);

    void CompileShaders();
};

#endif // SDATA_H
