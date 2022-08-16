#include "sdata.h"

SData SData::sdata;

SData::SData()
{

}

QSharedPointer<QOpenGLShaderProgram> SData::LinkShader(QString vert, QString frag)
{
    auto *program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vert);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, frag);

    // Link shader pipeline
    if (!program->link())
        qDebug() << "Program linking shaders " + vert + " or " + frag;

    return QSharedPointer<QOpenGLShaderProgram>(program);

}

void SData::CompileShaders()
{
    shaderPrograms["flat"] = LinkShader(":/shaders/vert_flat.glsl",":/shaders/frag_flat.glsl");
}
