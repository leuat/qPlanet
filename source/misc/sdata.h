#ifndef SDATA_H
#define SDATA_H

#include <QVector3D>
#include <QOpenGLShader>
#include <QMap>
#include <QSharedPointer>
#include "mesh.h"

class SData
{
public:
    SData();
    QMap<QString, QSharedPointer<QOpenGLShaderProgram>> shaderPrograms;

    QVector3D s_directionalLight = QVector3D(1,1,1);
    QVector3D s_directionalLightColor = QVector3D(1,1,1);

    static SData sdata;

    QSharedPointer<QOpenGLShaderProgram> LinkShader(QString vert, QString frag);

    void CompileShaders();
};

#endif // SDATA_H
