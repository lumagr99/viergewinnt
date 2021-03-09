#ifndef GLTOKENGREEN_H
#define GLTOKENGREEN_H

#include "gltoken.h"

/**
 * @brief The GLTokenGreen class
 *
 * Objekte der Klasse stellen ein grünes Token des Vier Gewinnt Spiels dar.
 *
 */
class GLTokenGreen : public GLToken {

public:
    /**
     * @brief GLTokenGreen
     *
     * Erstellt und Initialisiert ein neues GLTokenGreen-Objekt.
     *
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTokenGreen(const QString& name, const QString& textureFile = ":/textures/TokenGreen.png");

private:
    /**
     * @brief m_textureFile
     *
     * Texturdatei des Tokens.
     */
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
