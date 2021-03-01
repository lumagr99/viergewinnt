#include "glmouseray.h"
#include <QThread>
#include <QtDebug>

GLMouseRay::GLMouseRay():
    GLBody("MouseRay")
{
  m_drawingMode = GL_LINES;
  m_color = cl_Red;
}

void GLMouseRay::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    Q_UNUSED(pointContainer)
    Q_UNUSED(indexContainer)
    GLBody::makeSurface(nullptr, nullptr); //force creation of own containers
    m_firstPoint = pointsSize();
    m_indices->append(m_firstPoint);
    m_points->append(GLPoint(v_Zero));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(v_Zero));
    m_nextPoint = pointsSize();
    m_nextIndex = m_nextPoint;
}

void GLMouseRay::setPoints(const QVector3D &nearPoint, const QVector3D &farPoint)
{
    qDebug() << "GLMouseRay::setPoints running on thread: " << QThread::currentThreadId();
    m_nearPoint = nearPoint;
    //we need to shift near point into visible volume
    m_nearPoint += (m_farPoint - m_nearPoint) * 0.001f;
    (*m_points)[m_firstPoint] = GLPoint(m_nearPoint,  v_X, v_Zero, m_color);
    m_farPoint = farPoint;
    (*m_points)[m_nextPoint - 1] = GLPoint(m_farPoint,  v_X, v_Zero, m_color);
}

void GLMouseRay::draw(GLESRenderer *renderer, bool useBuffers)
{
    Q_UNUSED(useBuffers)
    float oldPointSize;
    glGetFloatv(GL_POINT_SIZE, &oldPointSize);
    m_drawingMode = GL_POINTS;
    glPointSize(3.0f);
    GLBody::draw(renderer, false);
    m_drawingMode = GL_LINES;
    float oldLineWidth = 1.0;
    glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);
    glLineWidth(5.0);
    GLBody::draw(renderer, false);
    //clean up
    glLineWidth(oldLineWidth);
    glPointSize(oldPointSize);
}
