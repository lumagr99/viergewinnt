#include "glhalma.h"

GLHalma::GLHalma(const QString &name, float radius, const GLColorRgba &color, const QString textureFile)
    : GLBody(name, radius, color, textureFile){
    qDebug() << "GLHalma::GLHalma() called.";
    GLBody::setShowFrame(true);
    GLBody::readBinaryModelFile(":/textures/court.dat");
    m_minCoordinate = QVector3D(-1.5, 4, -1.5);
    GLBody::createFrame();
}

void GLHalma::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    qDebug() << "GLHalma::GLHalma() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}

//Spielfigur mit ModelViewMatrix bewegen
