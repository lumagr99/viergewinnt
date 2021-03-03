#include "gltokenred.h"

GLTokenRed::GLTokenRed(const QString & name)
    : GLToken(name, RADIUS, ":/textures/tokenred.JPG", GLColorRgba::clRed)
{
    //qDebug() << "GLTokenRed::GLTokenRed() called.";
}
