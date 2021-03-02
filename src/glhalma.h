#ifndef GLHALMA_H
#define GLHALMA_H


#include "glbody.h"

class GLHalma : public GLBody
{
public:
    GLHalma(const QString &name, float radius = 1.0, const GLColorRgba &color = GLColorRgba::clBlue, const QString textureFile = ":/textures/face-smile.png");

    virtual void makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer) Q_DECL_OVERRIDE;

};

#endif // GLHALMA_H
