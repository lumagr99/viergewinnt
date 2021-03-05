#include "gltokengreen.h"

GLTokenGreen::GLTokenGreen(const QString& name, const QString& textureFile)
    : GLToken(name, textureFile)
    , m_textureFile(textureFile)
{
    qDebug() << "GLTokenRed::GLTokenGreen() called.";
    setColor(GLColorRgba::clGreen);
    m_copies.append(this);
}
