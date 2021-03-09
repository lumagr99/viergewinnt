#ifndef GLTOKENRED_H
#define GLTOKENRED_H

#include "gltoken.h"

/**
 * @brief Rotes Token des Vier Gewinnt Spiels.
 */
class GLTokenRed : public GLToken {

public:
    /**
     * @brief Konstruktor. Erstellt und Initialisiert ein neues GLTokenRed-Objekt.
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTokenRed(const QString& name, const QString& textureFile = ":/textures/TokenRed.png");

private:
    /**
     * @brief Texturdatei.
     */
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
