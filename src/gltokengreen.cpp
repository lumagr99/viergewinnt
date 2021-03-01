#include "gltokengreen.h"
GLTokenGreen::GLTokenGreen(const QString & name) : GLToken(name, 1.0f, ":/textures/tokengreen.JPG", cl_Green)
{
    qDebug() << "GLTokenGreen::GLTokenGreen() called.";
}
