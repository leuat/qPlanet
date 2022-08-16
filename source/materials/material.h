#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShader>
#include <QOpenGLTexture>

class Material
{
public:

    QOpenGLShaderProgram* program = nullptr;

    QOpenGLTexture *texture = nullptr;
    QOpenGLTexture *normal_map = nullptr;
    QOpenGLTexture *parallax_map = nullptr;


    float uvScale = 1.0;
    float normalScale = 1.0;
    float specular = 1.0;
    float shininess = 1.0;
    QVector3D color = QVector3D(1,1,1);
    void Link(QString vert, QString frag);

    Material();



    virtual void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) = 0;
    void release();


};

class MaterialFlat : public Material {
public:
    MaterialFlat();
    void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) override;
};


#endif // MATERIAL_H
