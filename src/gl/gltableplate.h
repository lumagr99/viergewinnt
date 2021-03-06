#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"

class GLTablePlate : public GLBody {

public:
    GLTablePlate(const QString& name, float radius = 1.0f, const QString textureFile = ":/textures/table.JPG", const GLColorRgba& color = GLColorRgba::clGray);
};

#endif // GLTABLEPLATE_H
