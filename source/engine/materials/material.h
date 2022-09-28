#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLShader>
#include <QOpenGLTexture>


class MaterialData {
public:
    float normalScale = 1.0;
    float specular = 1.0;
    float shininess = 25.0;
    float normalIntensity = 1.0;

    QVector3D color = QVector3D(1,1,1);

};

class Material
{
public:

    QOpenGLShaderProgram* program = nullptr;

    QOpenGLTexture *texture = nullptr;
    QOpenGLTexture *normal_map = nullptr;
    QOpenGLTexture *parallax_map = nullptr;

    QVector2D uvScale = QVector2D(1,1);
    QVector2D uvScaleNormalMap = QVector2D(1,1);
    MaterialData mData;

    void Link(QString vert, QString frag);

    Material();


    void release();
    void setMatrices(const QMatrix4x4& mvp, const QMatrix3x3& rot);
    void setDefaults(MaterialData& d);
    void SetEye();


    virtual void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) = 0;

    QOpenGLTexture* loadTexture(QString txt);



};

class MaterialFlat : public Material {
public:
    MaterialFlat();
    void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) override;
};

class MaterialTexture : public Material {
public:
    MaterialTexture(QString textureFile, QVector2D scale);
    void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) override;
};

class MaterialNormal : public Material {
public:
    MaterialNormal(QString textureFile, QVector2D scale,QString textureNormal, QVector2D normalScale);
    void bind(QMatrix4x4 mvp, const QMatrix3x3 rot) override;
};



#endif // MATERIAL_H
