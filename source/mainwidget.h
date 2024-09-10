
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "source/engine/world.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include "source/engine/eriswidget.h"
#include "source/engine/meshes/meshobject.h"

class MainWidget : public ErisWidget
{
    Q_OBJECT

public:
    MainWidget();


protected:

    void AddGirl();
    void AddCubes();
    void AddWalls();
    void AddChunk();
private:
    MeshInstance* water = nullptr;
    void Init() override;
    void Update() override;

    void initMeshes() override;
    void mousePressEvent(QMouseEvent *e) override;


    void PaintGUI() override;
    void InitMaterials();

    void calculateWaterShadow();



};

#endif // ERISWIDGET_H
