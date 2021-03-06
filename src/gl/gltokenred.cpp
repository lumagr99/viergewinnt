#include "gltokenred.h"

GLTokenRed::GLTokenRed(const QString& name, const QString& textureFile)
    : GLToken(name, textureFile)
    , m_textureFile(textureFile)
{
    m_color = GLColorRgba::clRed;
}
