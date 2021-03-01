#include "glcube.h"

GLCube::GLCube(const QString &name, float radius, const GLColorRgba &color, const QString textureFile, const QVector3D &lbb, const QVector3D &rtf)
    : GLBody(name, radius, color, textureFile){

    m_lbb = lbb;
    m_rtf = rtf;

}

//void GLCube::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer){
//    qDebug() << "GLCube::makeSurface()";
//    GLBody::makeSurface(pointContainer, indexContainer);

//    /**
//   Corner vertices of the cube

//   ltb_____________rtb
//     /            /|
//    /            / |
//ltf/____________/m_rtf
//   |   |        |  |
//   |   |        |  |
//   |m_lbb|________|__|rbb
//   |  /         | /
//   | /          | /
//lbf|/___________|/rbf

//   */

//    //Vorlesung
//    QVector3D normal = v_X;
//    QVector3D texCoord = v_X;
//    GLColorRgba color = GLColorRgba::clRed;

//    QVector3D ltb = QVector3D(m_lbb.x(), m_rtf.y(), m_lbb.z());
//    QVector3D rtb = QVector3D(m_rtf.x(), m_rtf.y(), m_lbb.z());
//    //QVector3D m_lbb = v_Zero * size;
//    QVector3D rbb = QVector3D(m_rtf.x(), m_lbb.y(), m_lbb.z());

//    QVector3D lbf = QVector3D(m_lbb.x(), m_lbb.y(), m_rtf.z());
//    QVector3D rbf = QVector3D(m_rtf.x(), m_lbb.y(), m_rtf.z());
//    QVector3D ltf = QVector3D(m_lbb.x(), m_rtf.y(), m_rtf.z());
//    //QVector3D m_rtf = v_XYZ * size;


//    m_firstPoint = m_points->size();
//    //Back
//    //Flächennormale steht senkrecht auf der Fläche, daher n = 0 0 -1
//    normal = -v_Z;

//    m_points->append(GLPoint(m_lbb, normal, texCoord, color));
//    m_points->append(GLPoint(rbb, normal, texCoord, color));
//    m_points->append(GLPoint(ltb, normal, texCoord, color));

//    m_points->append(GLPoint(rbb, normal, texCoord, color));
//    m_points->append(GLPoint(rtb, normal, texCoord, color));
//    m_points->append(GLPoint(ltb, normal, texCoord, color));

//    //Top
//    //Flächennormale steht senkrecht auf der Fläche, daher n = 0 0 -1
//    color = GLColorRgba::clGreen;
//    normal = v_Y;
//    m_points->append(GLPoint(ltf, normal, texCoord, color));
//    m_points->append(GLPoint(m_rtf, normal, texCoord, color));
//    m_points->append(GLPoint(ltb, normal, texCoord, color));

//    m_points->append(GLPoint(ltb, normal, texCoord, color));
//    m_points->append(GLPoint(rtb, normal, texCoord, color));
//    m_points->append(GLPoint(m_rtf, normal, texCoord, color));

//    //Unten
//    normal =-v_Y;
//    texCoord = v_Z;
//    color = GLColorRgba::clMagenta;

//    m_points->append(GLPoint(lbf, normal, texCoord, color));
//    m_points->append(GLPoint(rbf, normal, texCoord, color));
//    m_points->append(GLPoint(rbb, normal, texCoord, color));

//    m_points->append(GLPoint(rbb, normal, texCoord, color));
//    m_points->append(GLPoint(m_lbb, normal, texCoord, color));
//    m_points->append(GLPoint(lbf, normal, texCoord, color));

//    //Links
//    normal = -v_X;
//    texCoord = v_Z;
//    color = GLColorRgba::clGray;

//    m_points->append(GLPoint(ltb, normal, texCoord, color));
//    m_points->append(GLPoint(ltf, normal, texCoord, color));
//    m_points->append(GLPoint(lbf, normal, texCoord, color));

//    //GLPoint(punkt_ecke, normal, text, color);
//    //GLPoint(punkt_normal+punkt_ecke, normal, text, color);

//    m_points->append(GLPoint(lbf, normal, texCoord, color));
//    m_points->append(GLPoint(m_lbb, normal, texCoord, color));
//    m_points->append(GLPoint(ltb, normal, texCoord, color));

//    //Rechts
//    normal = v_X;
//    texCoord = v_Z;
//    color = GLColorRgba::clYellow;

//    m_points->append(GLPoint(rtb, normal, texCoord, color));
//    m_points->append(GLPoint(m_rtf, normal, texCoord, color));
//    m_points->append(GLPoint(rbf, normal, texCoord, color));

//    m_points->append(GLPoint(rbf, normal, texCoord, color));
//    m_points->append(GLPoint(rbb, normal, texCoord, color));
//    m_points->append(GLPoint(rtb, normal, texCoord, color));

//    //Vorne
//    normal = v_Z;
//    texCoord = v_Z;
//    color = GLColorRgba::clCyan;

//    m_points->append(GLPoint(ltf, normal, texCoord, color));
//    m_points->append(GLPoint(m_rtf, normal, texCoord, color));
//    m_points->append(GLPoint(lbf, normal, texCoord, color));

//    m_points->append(GLPoint(lbf, normal, texCoord, color));
//    m_points->append(GLPoint(rbf, normal, texCoord, color));
//    m_points->append(GLPoint(m_rtf, normal, texCoord, color));

//    m_nextPoint = m_points->size();
//}

//Würfel. Texturkoordinaten sind gesetzt. Funktioniert mit cube.jpg
void GLCube::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    //m_drawingMode = GL_POINTS; //Für Punkte
    //m_drawingMode = GL_TRIANGLES; //Für Dreiecke
    m_drawingMode = GL_QUADS;
    m_firstPoint = m_points->size();
    float faktorGroesse = m_rtf.x() - m_lbb.x();

    QVector3D vX = QVector3D(1.0,0.0,0.0);
    QVector3D vY = QVector3D(0.0,1.0,0.0);
    QVector3D vZ = QVector3D(0.0,0.0,1.0);

    QVector3D lbb = m_lbb;
    QVector3D rtf = m_rtf;
    QVector3D ltb = lbb + faktorGroesse * vY;
    QVector3D rtb = rtf - faktorGroesse * vZ;
    QVector3D rbb = lbb + faktorGroesse * vX;
    QVector3D rbf = rtf - faktorGroesse * vY;
    QVector3D lbf = lbb + faktorGroesse * vZ;
    QVector3D ltf = rtf - faktorGroesse * vX;

    //Vorne //Eins
    m_points->append(GLPoint(ltf, vZ, QVector3D(0.0, 1.0, 0.0), GLColorRgba::clRed));
    m_points->append(GLPoint(lbf, vZ, QVector3D(0.0, 0.0, 0.0), GLColorRgba::clRed));
    m_points->append(GLPoint(rbf, vZ, QVector3D(0.125, 0.0, 0.0), GLColorRgba::clRed));
    m_points->append(GLPoint(rtf, vZ, QVector3D(0.125, 1.0, 0.0), GLColorRgba::clRed));

    //Hinten //Sechs
    m_points->append(GLPoint(rtb, -vZ, QVector3D(0.5, 1.0, 0.0), GLColorRgba::clCyan));
    m_points->append(GLPoint(rbb, -vZ, QVector3D(0.5, 0.0, 0.0), GLColorRgba::clCyan));
    m_points->append(GLPoint(lbb, -vZ, QVector3D(0.625, 0.0, 0.0), GLColorRgba::clCyan));
    m_points->append(GLPoint(ltb, -vZ, QVector3D(0.625, 1.0, 0.0), GLColorRgba::clCyan));

    //Rechts //Drei
    m_points->append(GLPoint(rtf, vX, QVector3D(0.125, 1.0, 0.0), GLColorRgba::clBlue));
    m_points->append(GLPoint(rbf, vX, QVector3D(0.125, 0.0, 0.0), GLColorRgba::clBlue));
    m_points->append(GLPoint(rbb, vX, QVector3D(0.25, 0.0, 0.0), GLColorRgba::clBlue));
    m_points->append(GLPoint(rtb, vX, QVector3D(0.25, 1.0, 0.0), GLColorRgba::clBlue));

    //Links //Vier
    m_points->append(GLPoint(ltb, -vX, QVector3D(0.375, 1.0, 0.0), GLColorRgba::clYellow));
    m_points->append(GLPoint(lbb, -vX, QVector3D(0.375, 0.0, 0.0), GLColorRgba::clYellow));
    m_points->append(GLPoint(lbf, -vX, QVector3D(0.5, 0.0, 0.0), GLColorRgba::clYellow));
    m_points->append(GLPoint(ltf, -vX, QVector3D(0.5, 1.0, 0.0), GLColorRgba::clYellow));

    //Oben //Fünf
    m_points->append(GLPoint(ltb, vY, QVector3D(0.25, 1.0, 0.0), GLColorRgba::clGreen));
    m_points->append(GLPoint(ltf, vY, QVector3D(0.25, 0.0, 0.0), GLColorRgba::clGreen));
    m_points->append(GLPoint(rtf, vY, QVector3D(0.375, 0.0, 0.0), GLColorRgba::clGreen));
    m_points->append(GLPoint(rtb, vY, QVector3D(0.375, 1.0, 0.0), GLColorRgba::clGreen));

    //Unten //Zwei
    m_points->append(GLPoint(lbf, -vY, QVector3D(0.625, 1.0, 0.0), GLColorRgba::clWhite));
    m_points->append(GLPoint(lbb, -vY, QVector3D(0.625, 0.0, 0.0), GLColorRgba::clWhite));
    m_points->append(GLPoint(rbb, -vY, QVector3D(0.75, 0.0, 0.0), GLColorRgba::clWhite));
    m_points->append(GLPoint(rbf, -vY, QVector3D(0.75, 1.0, 0.0), GLColorRgba::clWhite));

    m_nextPoint = m_points->size();
}
