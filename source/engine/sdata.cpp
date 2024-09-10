#include "sdata.h"
#include "misc/util.h"

SData SData::sdata;

SData::SData()
{

}

QSharedPointer<QOpenGLShaderProgram> SData::LinkShader(QString vert, QString frag, QString includeFrag = "")
{
    auto *program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vert);
    if (includeFrag!="") {
        QString code = Util::loadTextFile(includeFrag);
        code += Util::loadTextFile(frag);

        program->addShaderFromSourceCode(QOpenGLShader::Fragment, code);
    }
    else
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, frag);

    if (!program->link()) {
        SData::fatalError("Program linking shaders " + vert + " or " + frag);
    }


    return QSharedPointer<QOpenGLShaderProgram>(program);

}

void SData::CompileShaders()
{
    shaderPrograms["flat"] = LinkShader(":/shaders/vert_flat.glsl",":/shaders/frag_flat.glsl");
    shaderPrograms["atmosphere"] = LinkShader(":/shaders/atmosphere.vert",":/shaders/atmosphere.frag",":/shaders/common.frag");
//    shaderPrograms["flat"] = LinkShader(":/shaders/vert_flat_110.glsl",":/shaders/frag_flat_110.glsl");
//    shaderPrograms["texture"] = LinkShader(":/shaders/vert_texture.glsl",":/shaders/frag_texture.glsl");
    shaderPrograms["normal"] = LinkShader(":/shaders/vert_normal.glsl",":/shaders/frag_normal.glsl");
    shaderPrograms["block"] = LinkShader(":/shaders/block.vert",":/shaders/block.frag",":/shaders/common.frag");
    shaderPrograms["textured_block"] = LinkShader(":/shaders/block_texture.vert",":/shaders/block_texture.frag",":/shaders/common.frag");
    shaderPrograms["water"] = LinkShader(":/shaders/water.vert",":/shaders/water.frag",":/shaders/common.frag");
    shaderPrograms["clouds"] = LinkShader(":/shaders/clouds.vert",":/shaders/clouds.frag",":/shaders/common.frag");

}

void SData::fatalError(QString s)
{
    qDebug() << s;
    exit(1);
}
