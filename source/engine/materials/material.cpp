#include "source/engine/materials/material.h"
#include "source/engine/sdata.h"
#include <QFile>

Material::Material()
{


}

void Material::setDefaults(MaterialData& d)
{
    program->setUniformValue("u_shininess", d.shininess );
    program->setUniformValue("u_specular", d.specular );
    program->setUniformValue("u_color", d.color );
    program->setUniformValue("u_light_dir",SData::sdata.s_directionalLight);
    program->setUniformValue("u_light_col",SData::sdata.s_directionalLightColor);
    program->setUniformValue("u_normalIntensity", 1-d.normalIntensity );

}

void Material::SetEye()
{
    program->setUniformValue("eye", SData::sdata.s_eye);
    program->setUniformValue("eye_dir", SData::sdata.s_eye_direction);

}

void Material::release()
{
    if (texture)
        texture->release();
    if (normal_map)
        normal_map->release();
}

void Material::setMatrices(const QMatrix4x4 &mvp, const QMatrix3x3 &rot)
{
    program->setUniformValue("mvp_matrix", mvp);
    program->setUniformValue("rot_matrix", rot);

}


MaterialFlat::MaterialFlat()
{
    program = SData::sdata.shaderPrograms["flat"].get();
//    Link(":/shaders/vert_flat.glsl",":/shaders/frag_flat.glsl");
}

void MaterialFlat::bind(QMatrix4x4 mvp, const QMatrix3x3 rot)
{
    program->bind();
    setMatrices(mvp,rot);
    setDefaults(mData);

}

QOpenGLTexture* Material::loadTexture(QString txt)
{
    if (!QFile::exists(txt))
        return nullptr;
    auto img = QImage(txt);
    auto tex = new QOpenGLTexture(img);
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::Linear);
    tex->setWrapMode(QOpenGLTexture::Repeat);
    return tex;
}

MaterialTexture::MaterialTexture(QString textureFile, QVector2D scale)
{
    uvScale = scale;
    program = SData::sdata.shaderPrograms["texture"].get();
    texture = loadTexture(textureFile);

}

void MaterialTexture::bind(QMatrix4x4 mvp, const QMatrix3x3 rot)
{
    program->bind();
    setMatrices(mvp,rot);
    setDefaults(mData);
    SetEye();
    program->setUniformValue("tex_scale", uvScale);
    if (texture!=nullptr) {
        glBindTexture(GL_TEXTURE_2D, texture->textureId());
        program->setUniformValue("tex1", 0);
    }

}

MaterialNormal::MaterialNormal(QString textureFile, QVector2D scale, QString textureNormal, QVector2D normalScale)
{
    uvScale = scale;
    uvScaleNormalMap = normalScale;

    program = SData::sdata.shaderPrograms["normal"].get();

    texture = loadTexture(textureFile);
    normal_map = loadTexture(textureNormal);

}

void MaterialNormal::bind(QMatrix4x4 mvp, const QMatrix3x3 rot)
{
    program->bind();
    setMatrices(mvp,rot);
    setDefaults(mData);
    SetEye();
    program->setUniformValue("tex_scale", uvScale);
    program->setUniformValue("uv_scale", uvScale);
    if (normal_map!=nullptr) {
  //      glBindTexture(GL_TEXTURE_2D, normal_map->textureId());
        normal_map->bind(1);
        program->setUniformValue("tex2", 1);
    }
    if (texture!=nullptr) {
        texture->bind(0);
//        glBindTexture(GL_TEXTURE_2D, texture->textureId());
        program->setUniformValue("tex1", 0);
    }

}
