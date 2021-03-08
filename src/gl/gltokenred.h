#ifndef GLTOKENRED_H
#define GLTOKENRED_H

#include "gltoken.h"

class GLTokenRed : public GLToken {
public:
    GLTokenRed(const QString& name, const QString& textureFile = ":/textures/TokenRed.png");

private:
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
