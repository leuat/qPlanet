#include "material.h"
#include "source/misc/sdata.h"



Material::Material()
{


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

    program->setUniformValue("mvp_matrix", mvp);
    program->setUniformValue("rot_matrix", rot);
    program->setUniformValue("u_shininess", shininess );
    program->setUniformValue("u_specular", specular );
    program->setUniformValue("u_color", color );
    program->setUniformValue("u_light_dir",SData::sdata.s_directionalLight);
    program->setUniformValue("u_light_col",SData::sdata.s_directionalLightColor);

}
