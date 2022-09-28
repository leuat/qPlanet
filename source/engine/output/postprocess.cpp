#include "postprocess.h"

void PostProcess::InitFramebuffer(int w, int h)
{
    //    int w = width()*2;
    //  int h = height()*2;
    if (!fbo || fbo->width() != w || fbo->height() != h)
    {
        if (fbo)
            delete fbo;

        fbo = new QOpenGLFramebufferObject(w, h, QOpenGLFramebufferObject::CombinedDepthStencil);
    }


}

PostProcess::PostProcess()
{

}


void PostProcess::StartFBuf()
{
    if (fbo && isEnabled)
        fbo->bind();
}

void PostProcess::EndFBuf()
{
    if (fbo && isEnabled)
        fbo->release();

}

void PostProcess::Draw()
{
    if (!isEnabled)
        return;

    shaderScreen.bind();


    glBindTexture(GL_TEXTURE_2D, fbo->texture());

    //    fbo->bind();
    //sbuf->bind(b);
    //    fbuf->release();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    //    qDebug() << "IIIH" << fbuf->textureId();
    shaderScreen.setUniformValue("screen", 0 );
    shaderScreen.setUniformValue("CD", CD.x(), CD.y());
    shaderScreen.setUniformValue("barrelScale", barrelScale.x(), barrelScale.y());
    shaderScreen.setUniformValue("chromatic",chromatic);
    shaderScreen.setUniformValue("lsca",lsca);
    shaderScreen.setUniformValue("lamp",lamp);
    shaderScreen.setUniformValue("saturation",saturation);
    shaderScreen.setUniformValue("gamma",gamma);
    shaderScreen.setUniformValue("time",time);
    vao.bind();
    arrayBuf.bind();
    indexBuf.bind();
    shaderScreen.enableAttributeArray(0);
    shaderScreen.setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(GLfloat)*2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    shaderScreen.disableAttributeArray(0);
    // release the shader
    shaderScreen.release();
    indexBuf.release();
    arrayBuf.release();
    vao.release();
    time++;

}

bool PostProcess::InitShaders()
{

    initializeOpenGLFunctions();

    // Compile vertex shader
    if (!shaderScreen.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/screen.vert"))
        return false;

    // Compile fragment shader
    if (!shaderScreen.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/screen.frag"))
        return false;

    // Link shader pipeline
    shaderScreen.bindAttributeLocation("vPosition", 0);
    if (!shaderScreen.link())
        return false;


}

void PostProcess::Init()
{
    float s = 1.0;
    GLfloat vertices[]{ -1.0f*s, -1.0f*s,
                1.0f*s, -1.0f*s,
                -1.0f*s,  1.0f*s,
                1.0f*s, 1.0f*s };

    GLushort ind[] {0,1,2, 2,1,3};

    if (vao.create())
        vao.bind();

    arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBuf.create();
    arrayBuf.bind();

    arrayBuf.allocate(&vertices, 8 * sizeof(GLfloat));
    arrayBuf.release();

    indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(ind, 6 * sizeof(GLushort));

    indexBuf.release();
    vao.release();

}
