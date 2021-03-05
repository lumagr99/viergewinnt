#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"

class GLTablePlate : public GLBody {

public:
    GLTablePlate(const QString& name, float radius = 1.0f, const QString textureFile = ":/textures/table.JPG", const GLColorRgba& color = GLColorRgba::clGray);

    virtual void makeSurface(QVector<GLPoint>* pointContainer, QVector<IndexType>* indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer* renderer, bool useBuffers = false) Q_DECL_OVERRIDE;
};

#endif // GLTABLEPLATE_H
