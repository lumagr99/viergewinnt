#include "glpoint.h"

GLPoint::GLPoint(const QVector3D   & vertex, const QVector3D & normal, const QVector3D & texCoord,
                 const GLColorRgba & color ) {
    m_vertex   = vertex;
    m_normal   = normal;
    m_texCoord = texCoord;
    m_color    = color ;
} /* ----- end of constructor ----- */

GLPoint::~GLPoint() {
    // Es gibt momentan keinen Speicherplatz der freigegebene werden muss
}

int GLPoint::vertexOffset()
{
    return 0;
}

int GLPoint::normalOffset()
{
    return sizeof (QVector3D);
}

int GLPoint::texCoordOffset()
{
    return sizeof (QVector3D) * 2;
}

int GLPoint::colorOffset()
{
    return sizeof (QVector3D) * 3;
}

void GLPoint::moveTo(QVector3D vMove)
{
    m_vertex = vMove;
}

void GLPoint::transform(const QMatrix4x4 &vertexMatrix, const QMatrix4x4 &normalMatrix)
{
   m_vertex = vertexMatrix * m_vertex;
   m_normal = normalMatrix * m_normal;
}
