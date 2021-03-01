#include "gltokenred.h"

GLTokenRed::GLTokenRed(const QString & name) : GLToken(name, 1.0f, ":/textures/tokenred.JPG", cl_Red)
{
    qDebug() << "GLTokenRed::GLTokenRed() called.";
}
