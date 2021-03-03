#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

class GLToken: public GLBody
{

public:

    GLToken(const QString & name, float radius = RADIUS, const QString textureFile = "",
            const GLColorRgba & color = GLColorRgba::clBlue, float height = HEIGHT);

    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    virtual void findMinMaxCoordinates() Q_DECL_OVERRIDE;

    bool isColliding(const GLToken *token) const;

    float getHeight() const;

    float getRadius() const;

    bool isSelected() const;

    void setSelected(bool selected);

    static constexpr float HEIGHT = 0.325;

    static constexpr float RADIUS = 0.625;

private:

    float m_radius;

    float m_height;

    bool m_selected;
};

#endif // GLTOKEN_H
