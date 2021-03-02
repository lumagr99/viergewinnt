#include "gltokengreen.h"

GLTokenGreen::GLTokenGreen(const QString & name)
    : GLToken(name, BASE_RADIUS, ":/textures/tokengreen.JPG", cl_Green)
{
    qDebug() << "GLTokenGreen::GLTokenGreen() called.";
}
