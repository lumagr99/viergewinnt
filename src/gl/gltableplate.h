#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"

class GLTablePlate : public GLBody {

public:
    GLTablePlate(const QString& name, float radius = 1.0f, const GLColorRgba& color = GLColorRgba::clBlue, const QString textureFile = ":/textures/Table.png");

    ~GLTablePlate() Q_DECL_OVERRIDE;
};

#endif // GLTABLEPLATE_H
