#include "source/engine/materials/material.h"
#include "source/engine/sdata.h"
#include <QFile>

Material::Material()
{


}

void Material::setDefaults(const QMatrix4x4& mvp, const QMatrix3x3& rot)
{
    program->setUniformValue("mvp_matrix", mvp);
    program->setUniformValue("rot_matrix", rot);
    program->setUniformValue("u_shininess", shininess );
    program->setUniformValue("u_specular", specular );
    program->setUniformValue("u_color", color );
    program->setUniformValue("u_light_dir",SData::sdata.s_directionalLight);
    program->setUniformValue("u_light_col",SData::sdata.s_directionalLightColor);

}

void Material::SetEye()
{
    program->setUniformValue("eye", SData::sdata.s_eye);
    program->setUniformValue("eye_dir", SData::sdata.s_eye_direction);

}

void Material::release()
{
}

MaterialFlat::MaterialFlat()
{
    program = SData::sdata.shaderPrograms["flat"].get();
//    Link(":/shaders/vert_flat.glsl",":/shaders/frag_flat.glsl");
}

void MaterialFlat::bind(QMatrix4x4 mvp, const QMatrix3x3 rot)
{
    program->bind();
    setDefaults(mvp,rot);

}

MaterialTexture::MaterialTexture(QString textureFile, QVector2D scale)
{
    uvScale = scale;
    program = SData::sdata.shaderPrograms["texture"].get();
    if (!QFile::exists(textureFile))
        return;
    auto img = QImage(textureFile);
    texture = new QOpenGLTexture(img);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

}

void MaterialTexture::bind(QMatrix4x4 mvp, const QMatrix3x3 rot)
{
    program->bind();
    setDefaults(mvp,rot);
    SetEye();
    program->setUniformValue("tex_scale", uvScale);
    if (texture!=nullptr) {
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
        program->setUniformValue("tex1", 0);
    }

}
