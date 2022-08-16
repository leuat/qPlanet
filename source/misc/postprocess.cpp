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
    //sbuf->bind(b);
//    fbuf->release();
    shaderScreen.bind();
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
    float s = 1.0;
    GLfloat vertices[]{ -1.0f*s, -1.0f*s,
                        1.0f*s, -1.0f*s,
                        -1.0f*s,  1.0f*s,
                        1.0f*s, 1.0f*s };

    shaderScreen.enableAttributeArray(0);
    shaderScreen.setAttributeArray(0, GL_FLOAT, vertices, 2);
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    shaderScreen.disableAttributeArray(0);

    // release the shader
    shaderScreen.release();
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
