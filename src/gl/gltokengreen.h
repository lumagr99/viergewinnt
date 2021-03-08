#ifndef GLTOKENGREEN_H
#define GLTOKENGREEN_H

#include "gltoken.h"

class GLTokenGreen : public GLToken {
public:
    GLTokenGreen(const QString& name, const QString& textureFile = ":/textures/TokenGreen.png");

private:
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
