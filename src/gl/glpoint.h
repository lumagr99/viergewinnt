#ifndef GLPOINT_H
#define GLPOINT_H
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QVector3D>

#include "glcolorrgba.h"
// ----------------------------------------------------------------------------
/**
 * @brief The GLPoint class A Point containing data for a surface point. This class is used in binary read
 * and write functions. DO NOT add virtual methods, DO NOT change order of declarations.
 * xxxPointer methods return the addresses needed by OpenGL attribute functions.
 */
class GLPoint {
public:
    GLPoint(const QVector3D& vertex = QVector3D(0.0, 0.0, 0.0),
        const QVector3D& normal = QVector3D(0.0, 1.0, 0.0),
        const QVector3D& texCoord = QVector3D(0.0, 0.0, 0.0),
        const GLColorRgba& color = GLColorRgba(1.0, 1.0, 1.0, 1.0));
    ~GLPoint();

    //pointers to be used with QOpenGLShaderProgram::setAttributeArray
    QVector3D* vertexPointer(); // Returns address of vertex coordinate
    QVector3D* normalPointer(); // Returns address of normal coordinate
    QVector3D* texCoordPointer(); // Returns address of texture coordinate
    GLColorRgba* colorPointer(); // Returns address of color

    //offset values to be used with QOpenGLShaderProgram::setAttributeBuffer
    static int vertexOffset();
    static int normalOffset();
    static int texCoordOffset();
    static int colorOffset();

    void setVertex(const QVector3D& newVal) { m_vertex = newVal; }
    void setNormal(const QVector3D& newVal) { m_normal = newVal; }
    void setTexCoord(const QVector3D& newVal) { m_texCoord = newVal; }
    void setColor(const GLColorRgba& newVal) { m_color = newVal; }

    const QVector3D& vertex() const { return m_vertex; }
    const QVector3D& normal() const { return m_normal; }
    const QVector3D& texCoord() const { return m_texCoord; }
    const GLColorRgba& color() const { return m_color; }

    void move(QVector3D vMove);
    void moveTo(QVector3D vMove);
    /**
     * @brief transform Multiply vertex with vertexMatrix and normal with normalMatrix.
     * @param vertexMatrix
     * @param normalMatrix
     */
    void transform(const QMatrix4x4& vertexMatrix, const QMatrix4x4& normalMatrix);

private:
    QVector3D m_vertex; //DO NOT change order of declarations! Binary compatibility required!
    QVector3D m_normal;
    QVector3D m_texCoord;
    GLColorRgba m_color;
}; /* ----- end of class GLPoint ----- */

inline QVector3D* GLPoint::vertexPointer()
{
    return &m_vertex;
} /* ----- end of method vertexPointer ----- */

inline QVector3D* GLPoint::normalPointer()
{
    return &m_normal;
} /* ----- end of method normalPointer ----- */

inline QVector3D* GLPoint::texCoordPointer()
{
    return &m_texCoord;
} /* ----- end of method texCoordPointer ----- */

inline GLColorRgba* GLPoint::colorPointer()
{
    return &m_color;
} /* ----- end of method colorPointer ----- */

inline void GLPoint::move(QVector3D vMove)
{
    m_vertex = m_vertex + vMove;
} /* ----- end of method move ----- */
#endif // GLPOINT_H
