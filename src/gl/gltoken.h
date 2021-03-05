#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glmultiplebody.h"

class GLCourt;

class GLToken : public GLMultipleBody {

    static constexpr float HEIGHT = 0.325f;
    static constexpr float RADIUS = 0.625f;

public:
    GLToken(const QString& name, const QString& textureFile);

    bool isColliding(const GLToken* token) const;

    bool isColliding(const GLCourt* court) const;

    void moveToPosition(const QVector3D& position);

    void rotate();

    float getHeight() const;

    float getRadius() const;

    bool isRotated() const;

    void setRotated(bool rotated);

    bool isMovable() const;

    void setMovable(bool movable);

private:
    float m_radius;

    float m_height;

    bool m_selected;

    bool m_rotated;

    bool m_movable;
};

#endif // GLTOKEN_H
