#include <QtDebug>
#include <QThread>

#include "glmouseray.h"

GLMouseRay::GLMouseRay(const QString &name,  const GLColorRgba &color)
    : GLBody(name, 1.0, color)
{
}

void GLMouseRay::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    Q_UNUSED(pointContainer);
    Q_UNUSED(indexContainer);

    GLBody::makeSurface(nullptr, nullptr);

    m_firstPoint = m_points->size();
    m_points->append(GLPoint(m_nearPoint, v_Z, v_Zero, m_color));
    m_points->append(GLPoint(m_farPoint, v_Z, v_Zero, m_color));
    m_nextPoint = m_points->size();
}


void GLMouseRay::draw(GLESRenderer *renderer, bool useBuffers)
{
    glPointSize(3.0);
    setDrawingMode(GL_POINTS);
    GLBody::draw(renderer, useBuffers);
    glPointSize(1.0);

    glLineWidth(3.0);
    setDrawingMode(GL_LINES);
    GLBody::draw(renderer, useBuffers);
    glLineWidth(1.0);
}

void GLMouseRay::setPoints(const QVector3D &nearPoint, const QVector3D &farPoint)
{
    //qDebug() << "GLMouseRay::setPoints running on thread: " << QThread::currentThreadId();
    m_nearPoint = nearPoint;
    m_farPoint = farPoint;

    (*m_points)[m_firstPoint] = GLPoint(m_nearPoint, v_X, v_Zero, m_color);
    (*m_points)[m_nextPoint -1] = GLPoint(m_farPoint, v_X, v_Zero, m_color);
}

