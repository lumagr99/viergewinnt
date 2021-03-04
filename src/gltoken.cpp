#include <QDebug>

#include "gltoken.h"

GLToken::GLToken(const QString &name, float radius, const QString textureFile, const GLColorRgba &color, float height)
    : GLBody(name, radius, color, textureFile),
      m_radius(radius),
      m_height(height),
      m_selected(false),
      m_rotated(false)
{
    //qDebug() << "GLToken::GLToken() called.";
    setShowFrame(true);
    setCenter(QVector3D(0.0f, 0.0f, 0.0f));
    findMinMaxCoordinates();
    GLBody::readBinaryModelFile(":/models/token.dat");
}

void GLToken::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    //qDebug() << "GLToken::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLToken::draw(GLESRenderer *renderer, bool useBuffers)
{
    GLBody::draw(renderer,useBuffers);
}

void GLToken::findMinMaxCoordinates()
{
    if(isRotated()) {
        m_minCoordinate = m_center + QVector3D(-m_radius, -m_radius, -m_height/2);
        m_maxCoordinate = m_center + QVector3D(m_radius, m_radius, m_height/2);
    } else {
        m_minCoordinate = m_center + QVector3D(-m_radius, -m_height/2, -m_radius);
        m_maxCoordinate = m_center + QVector3D(m_radius, m_height/2, m_radius);
    }

    if(m_showNormals) {
        createNormals();
    }
    if(m_showFrame) {
        createFrame();
    }
}

bool GLToken::isColliding(const GLToken *token) const
{
    if(this == token) {
        return false;
    }

    if((m_center - token->getCenter()).length() >= m_radius + token->getRadius()) {
        return false;
    } else {
        if(m_center.y() < token->getCenter().y()) {
            return token->getCenter().y() - m_center.y() <= m_height;
        } else {
            return m_center.y() - token->getCenter().y() <= token->getHeight();
        }
    }
}

void GLToken::moveToPosition(const QVector3D &position)
{
    move(position - m_center);
}

void GLToken::rotate()
{
    if(!m_rotated) {
         rotateModelPoints(m_center, v_X, 90.0f);
         m_rotated = true;
    } else {
        rotateModelPoints(m_center, v_X, -90.0f);
        m_rotated = false;
    }
    findMinMaxCoordinates();
}

float GLToken::getHeight() const
{
    return m_height;
}

float GLToken::getRadius() const
{
    return m_radius;
}

bool GLToken::isSelected() const
{
    return m_selected;
}

void GLToken::setSelected(bool selected)
{
    m_selected = selected;
}

bool GLToken::isRotated() const
{
    return m_rotated;
}

void GLToken::setRotated(bool rotated)
{
    m_rotated = rotated;
}
