#ifndef GLTOKENGREEN_H
#define GLTOKENGREEN_H
#include "gltoken.h"

/**
 * @brief GLTokenGreen stellt den roten Spielstein dar.
 * @authors Luca Graef, Julian Witte
 */
class GLTokenRed : public GLToken
{
public:
    /**
     * @brief GLTokenGreen Konstruktor.
     * @param name
     */
    GLTokenRed(const QString & name);
};

#endif // GLTOKENGREEN_H
