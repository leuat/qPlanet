#ifndef SDATA_H
#define SDATA_H

#include <QVector3D>
#include <QOpenGLShader>
#include <QMap>
#include <QSharedPointer>
#include "source/engine/meshes/mesh.h"
#include "source/engine/misc/SimplexNoise.h"
#include <QMutex>
class SData
{
public:
    SData();
    QMap<QString, QSharedPointer<QOpenGLShaderProgram>> shaderPrograms;

    QMutex mutex;

    QVector3D s_directionalLight = QVector3D(1,0.7,1);
    QVector3D s_directionalLightColor = QVector3D(1,1,1);
    QVector3D *camera;
    double time = 0;
    static SData sdata;
    int noThreads = 0;
    QVector3D s_eye;
    QVector3D s_eye_direction;
    QVector2D s_mpos;
    SimplexNoise sn;
    QSharedPointer<QOpenGLShaderProgram> LinkShader(QString vert, QString frag);

    void CompileShaders();
};

#endif // SDATA_H
