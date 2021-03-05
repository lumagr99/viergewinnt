#ifndef GLMOUSERAY_H
#define GLMOUSERAY_H

#include "glbody.h"

class GLMouseRay : public GLBody {
public:
    GLMouseRay(const QString& name, const GLColorRgba& color);

    virtual void makeSurface(QVector<GLPoint>* pointContainer, QVector<IndexType>* indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer* renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    void setPoints(const QVector3D& nearPoint, const QVector3D& farPoint);

private:
    QVector3D m_nearPoint;

    QVector3D m_farPoint;
};

#endif // GLMOUSERAY_H
