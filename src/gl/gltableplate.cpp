#include "gltableplate.h"
#include "glbody.h"

GLTablePlate::GLTablePlate(const QString& name, float radius, const GLColorRgba& color, const QString textureFile)
    : GLBody(name, radius, color, textureFile)
{
    GLBody::readBinaryModelFile(":/models/Tableplate.dat");
}

GLTablePlate::~GLTablePlate()
{
    GLBody::destroyTextureObjects();
}
