#include "gltokengreen.h"

GLTokenGreen::GLTokenGreen(const QString & name)
    : GLToken(name, RADIUS, ":/textures/tokengreen.JPG", GLColorRgba::clGreen)
{
    //qDebug() << "GLTokenGreen::GLTokenGreen() called.";
}
