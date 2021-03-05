#include "gltokenred.h"

GLTokenRed::GLTokenRed(const QString& name, const QString& textureFile)
    : GLToken(name, textureFile)
    , m_textureFile(textureFile)
{
    qDebug() << "GLTokenRed::GLTokenRed() called.";
    setColor(GLColorRgba::clRed);
    m_copies.append(this);
}
