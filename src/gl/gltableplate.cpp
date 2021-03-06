#include "gltableplate.h"
#include "glbody.h"

GLTablePlate::GLTablePlate(const QString& name, float radius, const QString textureFile, const GLColorRgba& color)
    : GLBody(name, radius, color, textureFile)
{
    GLBody::readBinaryModelFile(":/models/tableplate.dat");
    move(QVector3D(0, -0.2, 0));
}
