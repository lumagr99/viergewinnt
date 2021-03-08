#include "gltoken.h"
#include "glcourt.h"

#include <QDebug>
#include <math.h>

GLToken::GLToken(const QString& name, const QString& textureFile)
    : GLBody(name)
    , m_rotated(false)
    , m_movable(true)
{
    m_radius = RADIUS;
    m_center = v_Zero;
    setTextureFile(textureFile);
    GLBody::readBinaryModelFile(":/models/Token.dat");
}

GLToken::~GLToken()
{
    GLBody::destroyTextureObjects();
}

bool GLToken::isColliding(const GLToken* token) const
{
    if (this == token) {
        return false;
    }

    if ((m_center - token->getCenter()).length() >= 2 * RADIUS) {
        return false;
    } else {
        if (m_center.y() < token->getCenter().y()) {
            return token->getCenter().y() - m_center.y() <= HEIGHT;
        } else {
            return m_center.y() - token->getCenter().y() <= HEIGHT;
        }
    }
}

bool GLToken::isColliding(const GLCourt* court) const
{
    QVector3D center = court->getCenter();
    if (fabs(m_center.z()) <= center.z() + GLCourt::DEPTH / 2 + m_radius
            && fabs(m_center.x()) <= center.x() + GLCourt::WIDTH / 2 + m_radius) {
        return true;
    }
    return false;
}

void GLToken::moveToPosition(const QVector3D& position)
{
    move(position - m_center);
}

void GLToken::rotate(float angle)
{
    rotateModelPoints(m_center, v_X, angle);
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
