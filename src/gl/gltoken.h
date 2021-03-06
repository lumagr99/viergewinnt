#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

class GLCourt;

class GLToken : public GLBody {

public:

    static constexpr float HEIGHT = 0.325f;
    static constexpr float RADIUS = 0.625f;

    GLToken(const QString& name, const QString& textureFile);

    bool isColliding(const GLToken* token) const;

    bool isColliding(const GLCourt* court) const;

    void moveToPosition(const QVector3D& position);

    void rotate(float angle);

    bool isRotated() const;

    void setRotated(bool rotated);

    bool isMovable() const;

    void setMovable(bool movable);

private:

    bool m_rotated;

    bool m_movable;
};

#endif // GLTOKEN_H
