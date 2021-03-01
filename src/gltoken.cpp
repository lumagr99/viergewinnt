#include "gltoken.h"

GLToken::GLToken(const QString &name, float radius, const QString textureFile, const GLColorRgba &color)
    :GLBody(name, radius, color, textureFile)
{
    qDebug() << "GLToken::GLToken() called.";
    GLBody::readBinaryModelFile(":/models/token.dat");
}

void GLToken::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    qDebug() << "GLToken::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLToken::draw(GLESRenderer *renderer, bool useBuffers)
{
    qDebug() << "GLToken::draw() called.";
    GLBody::draw(renderer,useBuffers);
}
