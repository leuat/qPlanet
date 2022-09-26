#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShader>

class PostProcess: protected QOpenGLFunctions
{
public:

    void InitFramebuffer(int w, int h);
    QVector3D CD = QVector3D(0.15, 0.3,0);
    // Barrel distortion shrinks the display area a bit, this will allow us to counteract that.
    QVector3D barrelScale = (QVector3D(1,1,1) - (0.20 * CD));
    float chromatic = 0.015*0;
    // Curvature distortion
    float lsca = 482.12*0;
    float lamp = 0.4*0;
    float saturation = 1.5;
    float gamma = 1.2;
    float time = 0;

    bool isEnabled = true;
    QOpenGLShaderProgram shaderScreen;

    QOpenGLFramebufferObject* fbo = nullptr;
    PostProcess();
    void StartFBuf();
    void EndFBuf();
    void Draw();
    bool InitShaders();
};

#endif // POSTPROCESS_H
