#ifndef GLMOUSERAY_H
#define GLMOUSERAY_H

#include "glbody.h"

/**
 * @brief The GLMouseRay class is a visible line for  debugging mouse clicks.
 */
class GLMouseRay : public GLBody
{
public:
    GLMouseRay();
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)Q_DECL_OVERRIDE;
    void setPoints(const QVector3D & nearPoint, const QVector3D & farPoint);
    void draw(GLESRenderer * renderer, bool useBuffers = false)Q_DECL_OVERRIDE;

private:
    QVector3D m_nearPoint;
    QVector3D m_farPoint;

};

#endif // GLMOUSERAY_H
