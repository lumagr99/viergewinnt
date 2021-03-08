#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

class GLCourt;

class GLToken : public GLBody {

public:
    static constexpr float HEIGHT = 0.325f;
    static constexpr float RADIUS = 0.625f;

    GLToken(const QString& name, const QString& textureFile);

    ~GLToken() Q_DECL_OVERRIDE;

    bool isColliding(const GLToken* token) const;

    bool isColliding(const GLCourt* court) const;

    void moveToPosition(const QVector3D& position);

    void rotate(float angle);

    bool isRotated() const
    {
        return m_rotated;
    }

    void setRotated(bool rotated)
    {
        m_rotated = rotated;
    }

    bool isMovable() const
    {
        return m_movable;
    }

    void setMovable(bool movable)
    {
        m_movable = movable;
    }

private:
    bool m_rotated;

    bool m_movable;
};

#endif // GLTOKEN_H
