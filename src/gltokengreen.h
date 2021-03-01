#ifndef GLTOKENGREEN_H
#define GLTOKENGREEN_H
#include "gltoken.h"

/**
 * @brief GLTokenGreen stellt den grünen Spielstein dar.
 * @authors Luca Graef, Julian Witte
 */
class GLTokenGreen : public GLToken
{
public:
    /**
     * @brief GLTokenGreen Konstruktor.
     * @param name
     */
    GLTokenGreen(const QString & name);
};

#endif // GLTOKENGREEN_H
