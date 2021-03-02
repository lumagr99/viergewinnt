#include "glcourt.h"

GLCourt::GLCourt(const QString &name, float radius, const QString textureFile, const GLColorRgba &color)
    :GLBody(name, radius, color, textureFile)
{
    qDebug() << "GLCourt::GLCourt() called.";
    GLBody::readBinaryModelFile(":/models/court.dat");
}

void GLCourt::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    qDebug() << "GLCourt::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLCourt::draw(GLESRenderer *renderer, bool useBuffers)
{
    // qDebug() << "GLCourt::draw() called.";
    GLBody::draw(renderer,useBuffers);
}
