#ifndef GLTOKENGREEN_H
#define GLTOKENGREEN_H

#include "gltoken.h"

/**
 * @brief Grünes Token des Vier Gewinnt Spiels.
 */
class GLTokenGreen : public GLToken {

public:
    /**
     * @brief Konstruktor. Erstellt und Initialisiert ein neues GLTokenGreen-Objekt.
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTokenGreen(const QString& name, const QString& textureFile = ":/textures/TokenGreen.png");

private:
    /**
     * @brief Texturdatei.
     */
    QString m_textureFile;
};

#endif // GLTOKENGREEN_H
