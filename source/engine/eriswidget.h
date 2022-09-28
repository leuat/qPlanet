
#ifndef ERISWIDGET_H
#define ERISWIDGET_H

#include "world.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include "source/engine/entity.h"
#include "source/engine/output/postprocess.h"
#include "source/engine/sdata.h"
#include "source/engine/misc/util.h"

class World;


class ErisWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    ErisWidget();
    ~ErisWidget();


protected:

    QPoint m_mousePos;
    QPoint m_prevPos;
    QPointF m_mx;
    bool m_isStart = true;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    virtual void PaintGUI() {}

    bool eventFilter(QObject *obj, QEvent *event) override;

    void initShaders();
    void initTextures();
    virtual void initMeshes();
    virtual void Init() {
        world->Init();
    }
    virtual void Update();
    Entity player;
    PostProcess pp;



protected:
    QBasicTimer timer;
    World *world = nullptr;





    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
};

#endif // ERISWIDGET_H
