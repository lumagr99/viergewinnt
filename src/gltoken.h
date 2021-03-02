#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

class GLToken: public GLBody
{

public:

    static constexpr float BASE_HEIGHT = 0.325;

    static constexpr float BASE_RADIUS = 0.625;

    GLToken(const QString & name, float radius = BASE_RADIUS, const QString textureFile = "",
            const GLColorRgba & color = GLColorRgba::clBlue, float height = BASE_HEIGHT);

    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    virtual void findMinMaxCoordinates() Q_DECL_OVERRIDE;

    float getHeight();

    float getRadius();

    bool isSelected();

    void setSelected(bool selected);

private:

    float m_radius;

    float m_height;

    bool m_selected;
};

#endif // GLTOKEN_H
