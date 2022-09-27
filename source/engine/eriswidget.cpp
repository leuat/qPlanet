#include "eriswidget.h"

#include <QMouseEvent>


ErisWidget::ErisWidget() {
    installEventFilter(this);
    setMouseTracking(true);
}

ErisWidget::~ErisWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete world;
    doneCurrent();
}

//! [0]
void ErisWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->position());
}

void ErisWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->position()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]
void ErisWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;
    SData::sdata.time+=1;
    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        //rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
//! [1]

void ErisWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();
    SData::sdata.CompileShaders();
//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    world = new World;

    initMeshes();

    Init();

}

//! [3]
void ErisWidget::initShaders()
{
/*    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vert_planet.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag_planet.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

*/


    if (!pp.InitShaders())
        return;


}
//! [3]

//! [4]
void ErisWidget::initTextures()
{
/*    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
    */
}

void ErisWidget::initMeshes()
{
    world->m_meshes["box"] = QSharedPointer<MeshBox>(new MeshBox(1,2));
}

void ErisWidget::Update() {
    m_mx*=0.9;

}

//! [5]
void ErisWidget::resizeGL(int width, int height)
{

    int w = width;
    int h = height;
    qreal aspect = qreal(w) / qreal(h ? h : 1)*1;
//    aspect = 2;

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.01, zFar = 7000.0, fov = 80.0;


    world->m_camera.m_projection.setToIdentity();
    world->m_camera.m_projection.perspective(fov, aspect, zNear, zFar);

    // Set perspective projection
}

void ErisWidget::paintGL()
{
    // Clear color and depth buffer
    // Bind shader pipeline for use
    //initFramebuffer();

    pp.InitFramebuffer(width()*2, height()*2);

    pp.StartFBuf();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world->Render();
    glFinish();
    // Calculate aspect ratio


  //  QImage fb = fbo->toImage().convertToFormat(QImage::Format_RGB32);
//    fb.save("/Users/leuat/test.png");
    pp.EndFBuf();
    pp.Draw();
    Update();
    world->m_camera.RotateVertical(-m_mx.y()*0.1);
    world->m_camera.RotateHorisontal(-m_mx.x()*0.1);

    update();
    //! [6]
}

void ErisWidget::keyPressEvent(QKeyEvent* e)
{
    double speed = 0.4;
    if (e->key()==Qt::Key_W)
        world->m_camera.moveForward = speed;
    if (e->key()==Qt::Key_S)
        world->m_camera.moveForward = -speed;
    if (e->key()==Qt::Key_D)
        world->m_camera.moveSide = -speed;
    if (e->key()==Qt::Key_A)
        world->m_camera.moveSide = speed;

    if (e->key()==Qt::Key_E)
        world->m_camera.rotSide = -speed;
    if (e->key()==Qt::Key_Q)
        world->m_camera.rotSide = speed;


    if (e->key()==Qt::Key_Space)
        world->m_camera.moveUpdown = -speed;
    if (e->key()==Qt::Key_C)
        world->m_camera.moveUpdown = speed;


}

void ErisWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_W || e->key()==Qt::Key_S)
        world->m_camera.moveForward = 0;
    if (e->key()==Qt::Key_D || e->key()==Qt::Key_A)
      world->m_camera.moveSide = 0;
    if (e->key()==Qt::Key_E || e->key()==Qt::Key_Q) {
        world->m_camera.rotSide = 0;
    }
    if (e->key()==Qt::Key_C || e->key()==Qt::Key_Space) {
        world->m_camera.moveUpdown = 0;
    }


}

bool ErisWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        m_prevPos = m_mousePos;

        m_mousePos = mouseEvent->pos();
        if (m_isStart) {
            m_prevPos = m_mousePos;
            m_isStart = false;
        }
        m_mx +=(m_mousePos-m_prevPos);
 //       world->m_camera.RotateHorisontal(m_mx.x());
    }
    return false;
}
