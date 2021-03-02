#include "gltokenred.h"

GLTokenRed::GLTokenRed(const QString & name)
    : GLToken(name, BASE_RADIUS, ":/textures/tokenred.JPG", cl_Red)
{
    qDebug() << "GLTokenRed::GLTokenRed() called.";
}
