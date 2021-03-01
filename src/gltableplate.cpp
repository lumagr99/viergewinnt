#include "gltableplate.h"
#include "glbody.h"

GLTablePlate::GLTablePlate(const QString &name, float radius, const QString textureFile, const GLColorRgba &color)
    :GLBody(name, radius, color, textureFile)
{
    qDebug() << "GLToken::GLToken() called.";
    GLBody::readBinaryModelFile(":/models/tableplate.dat");
}

void GLTablePlate::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    qDebug() << "GLToken::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLTablePlate::draw(GLESRenderer *renderer, bool useBuffers)
{
    qDebug() << "GLToken::draw() called.";
    GLBody::draw(renderer,useBuffers);
}
