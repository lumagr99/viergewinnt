#include "gltableplate.h"
#include "glbody.h"

GLTablePlate::GLTablePlate(const QString &name, float radius, const QString textureFile, const GLColorRgba &color)
    :GLBody(name, radius, color, textureFile)
{
    //qDebug() << "GLTablePlate::GLTablePlate() called.";
    GLBody::readBinaryModelFile(":/models/tableplate.dat");
    move(QVector3D(0, -0.5, 0));
}

void GLTablePlate::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    //qDebug() << "GLTablePlate::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLTablePlate::draw(GLESRenderer *renderer, bool useBuffers)
{
    // qDebug() << "GLToken::draw() called.";
    GLBody::draw(renderer,useBuffers);
}
