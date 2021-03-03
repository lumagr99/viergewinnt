#ifndef GLCOURT_H
#define GLCOURT_H

#include "glbody.h"
#include "gltoken.h"

class GLCourt : public GLBody
{

public:

    GLCourt(const QString & name, float radius = 1.0f, const QString textureFile = "", const GLColorRgba & color = GLColorRgba::clBlue);

    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    virtual void findMinMaxCoordinates() Q_DECL_OVERRIDE;

    bool isColliding(const GLToken *token) const;

    static constexpr float WIDTH = 10.0f;

    static constexpr float HEIGHT = 8.5f;

    static constexpr float DEPTH = 0.65f;

private:
    float m_width;

    float m_height;

    float m_depth;

};

#endif // GLCOURT_H
