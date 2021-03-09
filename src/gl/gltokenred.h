#ifndef GLTOKENRED_H
#define GLTOKENRED_H

#include "gltoken.h"

/**
 * @brief The GLTokenRed class
 *
 * Objekte der Klasse stellen ein rotes Token des Vier Gewinnt Spiels dar.
 *
 */
class GLTokenRed : public GLToken {

public:
    /**
     * @brief GLTokenRed
     *
     * Erstellt und Initialisiert ein neues GLTokenRed-Objekt.
     *
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTokenRed(const QString& name, const QString& textureFile = ":/textures/TokenRed.png");

private:
    /**
     * @brief m_textureFile
     *
     * Texturdatei des Tokens.
     */
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
