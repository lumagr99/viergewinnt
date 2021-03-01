#include "glcolorrgba.h"

GLColorRgba GLColorRgba::clBlack = GLColorRgba(0.0,0.0,0.0,1.0);
GLColorRgba GLColorRgba::clRed = GLColorRgba(1.0,0.0,0.0,1.0);
GLColorRgba GLColorRgba::clYellow = GLColorRgba(1.0,1.0,0.0,1.0);
GLColorRgba GLColorRgba::clGreen = GLColorRgba(0.0,1.0,0.0,1.0);
GLColorRgba GLColorRgba::clCyan = GLColorRgba(0.0,1.0,1.0,1.0);
GLColorRgba GLColorRgba::clBlue = GLColorRgba(0.0,0.0,1.0,1.0);
GLColorRgba GLColorRgba::clLightBlue = GLColorRgba(0.2,0.2,1.0,1.0);
GLColorRgba GLColorRgba::clMagenta = GLColorRgba(1.0,0.0,1.0,1.0);
GLColorRgba GLColorRgba::clGray = GLColorRgba(0.5,0.5,0.5,1.0);
GLColorRgba GLColorRgba::clWhite = GLColorRgba(1.0,1.0,1.0,1.0);

GLColorRgba::GLColorRgba(float r, float g, float b, float a)
    :QVector4D(r,g,b,a)
{
    //nothing else to do
}

GLColorRgba::GLColorRgba(const GLColorRgba &other)
    :QVector4D(other.red(),other.green(),other.blue(),other.alpha())
{
}

GLColorRgba::GLColorRgba(const QVector4D &other)
    :QVector4D(other.x(),other.y(),other.z(),other.w())
{
}

/**
  * Returns the inverted color with a= 1.0
  */
GLColorRgba GLColorRgba::inverted()
{
    return GLColorRgba(1.0f - x(), 1.0f - y(), 1.0f - z(), 1.0f);
}

GLColorRgba GLColorRgba::operator *(float factor)
{
    return GLColorRgba(red()*factor,
                       green() *factor,
                       blue() * factor,
                       alpha());
}
