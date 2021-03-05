#include "gltoken.h"
#include "glcourt.h"

#include <QDebug>
#include <math.h>

GLToken::GLToken(const QString& name, const QString& textureFile)
    : GLMultipleBody(name)
    , m_radius(RADIUS)
    , m_height(HEIGHT)
    , m_selected(false)
    , m_rotated(false)
{
    // qDebug() << "GLToken::GLToken() called.";
    setCenter(v_Zero);
    setShowFrame(true);
    setTextureFile(textureFile);
    GLBody::readBinaryModelFile(":/models/token.dat");
}

bool GLToken::isColliding(const GLToken* token) const
{
    if (this == token) {
        return false;
    }

    if ((m_center - token->getCenter()).length() >= m_radius + token->getRadius()) {
        return false;
    } else {
        if (m_center.y() < token->getCenter().y()) {
            return token->getCenter().y() - m_center.y() <= m_height;
        } else {
            return m_center.y() - token->getCenter().y() <= token->getHeight();
        }
    }
}

bool GLToken::isColliding(const GLCourt* court) const
{
    float depth = court->getDepth();
    float width = court->getWidth();
    if (fabs(m_center.z()) <= depth / 2 + m_radius && fabs(m_center.x()) <= width / 2 + m_radius) {
        return true;
    }
    return false;
}

void GLToken::moveToPosition(const QVector3D& position)
{
    move(position - m_center);
}

void GLToken::rotate()
{
    if (!m_rotated) {
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

bool GLToken::isRotated() const
{
    return m_rotated;
}

void GLToken::setRotated(bool rotated)
{
    m_rotated = rotated;
}

bool GLToken::isMovable() const
{
    return m_movable;
}

void GLToken::setMovable(bool movable)
{
    m_movable = movable;
}
