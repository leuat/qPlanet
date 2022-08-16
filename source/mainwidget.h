
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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
#include "source/misc/entity.h"
#include "source/misc/postprocess.h"
#include "source/misc/sdata.h"
#include "source/misc/util.h"

class World;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    MainWidget();
    ~MainWidget();


protected:

    QPoint m_mousePos;
    QPoint m_prevPos;
    QPoint m_mx;
    bool m_isStart = true;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

    void initShaders();
    void initTextures();
    void initMeshes();
    void initWorld();
    Entity player;
    PostProcess pp;
    float time = 0;



private:
    QBasicTimer timer;
    World *world = nullptr;

    void UpdateWorld();




    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
};

#endif // MAINWIDGET_H
