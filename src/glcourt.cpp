#include "glcourt.h"
#include <math.h>

GLCourt::GLCourt(const QString &name, float radius, const QString textureFile, const GLColorRgba &color)
    :GLBody(name, radius, color, textureFile),
      m_rows(ROWS),
      m_columns(COLUMNS),
      m_width(WIDTH),
      m_height(HEIGHT),
      m_depth(DEPTH)
{
    //qDebug() << "GLCourt::GLCourt() called.";
    //setShowFrame(true);
    setCenter(QVector3D(0, 0, 0));
    setMinMaxCoordinates(QVector3D(-m_width/2, 0, -m_depth/2), QVector3D(m_width/2, m_height, m_depth/2));
    GLBody::readBinaryModelFile(":/models/court.dat");
}

void GLCourt::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
    //qDebug() << "GLCourt::makeSurface() called.";
    GLBody::makeSurface(pointContainer, indexContainer);
}


void GLCourt::draw(GLESRenderer *renderer, bool useBuffers)
{
    // qDebug() << "GLCourt::draw() called.";
    GLBody::draw(renderer,useBuffers);
}

void GLCourt::findMinMaxCoordinates()
{
    m_minCoordinate = m_center + QVector3D(-m_width/2, -m_height/2, -m_depth/2);
    m_maxCoordinate = m_center + QVector3D(m_width/2, m_height/2, m_depth/2);

    if(m_showNormals) {
        createNormals();
    }
    if(m_showFrame) {
        createFrame();
    }
}

bool GLCourt::isColliding(const GLToken *token) const
{
    QVector3D v = token->getCenter();
    if (fabs(v.z()) <= m_depth/2 + token->getRadius() && fabs(v.x()) <= m_width/2 + token->getRadius()) {
        return true;
    }
    return false;
}

QVector3D GLCourt::fieldToPosition(const QPoint &field) const
{
    QVector3D vx = v_X * (m_width / m_columns);
    QVector3D vy = v_Y * (m_height / m_rows);

    QVector3D result = QVector3D(-m_width/2, m_height, 0.0f) + 0.5 * (vx - vy);
    result += vx * field.x();
    result -= vy * field.y();

    return result;
}

int GLCourt::getColumnByPosition(const QVector3D &position) const
{
    return (position.x() + m_width / 2) / (m_width / m_columns);
}

int GLCourt::getRows() const
{
    return m_rows;
}

int GLCourt::getColumns() const
{
    return m_columns;
}

float GLCourt::getWidth() const
{
    return m_width;
}

float GLCourt::getHeight() const
{
    return m_height;
}

float GLCourt::getDepth() const
{
    return m_depth;
}
