
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

#include "shape.h"

class MainWidget : public ErisWidget
{
    Q_OBJECT

public:
    MainWidget();


    Shape sUser, sEstimate,sOrg, sGraph;
    Shape sResult;
    bool mouseDown = false;
    bool displayResult = false;
    QVector2D center;
    float delta;
protected:

    void AddGirl();
    void AddCubes();
    void AddWalls();
    void AddChunk();
    void InitShapes();

private:
    MeshInstance* water = nullptr;
    void Init() override;
    void Update() override;

    void initMeshes() override;
    void mousePressEvent(QMouseEvent *e) override;


    void PaintGUI() override;
    void InitMaterials();

    void calculateWaterShadow();
    void RenderSplines();
    void RenderShapes();

    void drawLine(int x0, int y0, int x1, int y1,  QColor col, int t);


};

#endif // ERISWIDGET_H
