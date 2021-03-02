#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

class GLToken: public GLBody
{

public:

    GLToken(const QString & name, float radius = 1.0f, const QString textureFile = "", const GLColorRgba & color = GLColorRgba::clBlue);

    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

};

#endif // GLTOKEN_H
