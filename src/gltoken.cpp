#include <QDebug>

#include "gltoken.h"

GLToken::GLToken(const QString &name, float radius, const QString textureFile, const GLColorRgba &color, float height)
    : GLBody(name, radius, color, textureFile),
      m_radius(radius),
      m_height(height)
{
    qDebug() << "GLToken::GLToken() called.";
    setShowFrame(true);
    setCenter(QVector3D(0, m_height / 2, 0));
    setMinMaxCoordinates(QVector3D(-m_radius, 0, -m_radius), QVector3D(m_radius, m_height, m_radius));
    GLBody::readBinaryModelFile(":/models/token.dat");
}

void GLToken::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    qDebug() << "GLToken::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLToken::draw(GLESRenderer *renderer, bool useBuffers)
{
    GLBody::draw(renderer,useBuffers);
}

void GLToken::findMinMaxCoordinates()
{
    m_minCoordinate = m_center + QVector3D(-m_radius, -m_height/2, -m_radius);
    m_maxCoordinate = m_center + QVector3D(m_radius, m_height/2, m_radius);

    if(m_showNormals) {
        createNormals();
    }
    if(m_showFrame) {
        createFrame();
    }
}

float GLToken::getHeight()
{
    return m_height;
}

float GLToken::getRadius()
{
    return m_radius;
}

bool GLToken::isSelected()
{
    return m_selected;
}

void GLToken::setSelected(bool selected)
{
    m_selected = selected;
}
