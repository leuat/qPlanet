#include "eriswidget.h"
#include <QMouseEvent>
#include <QtImGui.h>
#include <ImGui.h>

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
 /*   // Mouse release position - mouse press position
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
    */
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
    pp.Init();
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
    if (!m_isInMenu)
    {
        if (world!=nullptr) {
            world->m_camera.RotateVertical(m_mx.y()*0.1);
            world->m_camera.RotateHorisontal(-m_mx.x()*0.1);
        }

    }
    m_mx*=0.8;
}

void ErisWidget::SetThemeEnemyMouse()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Alpha = 1.0;
//    style.WindowFillAlphaDefault = 0.83;
  //  style.ChildWindowRounding = 3;
    style.WindowRounding = 3;
    style.GrabRounding = 1;
    style.GrabMinSize = 20;
    style.FrameRounding = 3;


    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.40f);
 //   style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
//    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
//    style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
//    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
//    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
//    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
//    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
//    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
 //   style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
 //   style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
}

//! [5]
void ErisWidget::resizeGL(int width, int height)
{

    int w = width;
    int h = height;
    qreal aspect = qreal(w) / qreal(h ? h : 1)*1;
//    aspect = 2;

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.0001, zFar = 700.0, fov = 80.0;

    if (world==nullptr)
        return;
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

    glClearColor(0.0, 0.0, 0.0, 1.0); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world->Render();
//    PaintGUI();
    glFinish();
    // Calculate aspect ratio


  //  QImage fb = pp.fbo->toImage().convertToFormat(QImage::Format_RGB32);
   // fb.save("/Users/leuat/test.png");
    pp.EndFBuf();
    pp.Draw();
    Update();

   // glClearColor(0.5,0.1,0,1);
   // glClear(GL_COLOR_BUFFER_BIT);
    PaintGUI();
    update();
    //! [6]
}

void ErisWidget::keyPressEvent(QKeyEvent* e)
{
    if (m_isInMenu)
        return;
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
        world->m_camera.rotSide = speed;
    if (e->key()==Qt::Key_Q)
        world->m_camera.rotSide = -speed;


    if (e->key()==Qt::Key_Space)
        world->m_camera.moveUpdown = speed;
    if (e->key()==Qt::Key_C)
        world->m_camera.moveUpdown = -speed;


}

void ErisWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Return) {
        m_isInMenu=!m_isInMenu;
    }

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
        QPointF m = QWidget::mapFromGlobal(QCursor::pos());
        if (m_isInMenu)
            SData::sdata.s_mpos = QVector2D(2*m.x()/(float)width()-1,1- 2*m.y()/(float)height());
        else SData::sdata.s_mpos = QVector2D(-1000,-1000);
        m_mx +=(m_mousePos-m_prevPos);
/*        auto c = cursor();
        c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

        setCursor(c);
*/
 //       world->m_camera.RotateHorisontal(m_mx.x());
    }
    return false;
}
