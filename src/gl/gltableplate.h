#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"

/**
 * @brief Tischplatte des Vier Gewinnt Spiels.
 */
class GLTablePlate : public GLBody {

public:
    /**
     * @brief Breite der Tischplatte.
     */
    static constexpr GLfloat WIDTH = 16.0f;

    /**
     * @brief Höhe der Tischplatte.
     */
    static constexpr GLfloat HEIGHT = 0.4f;

    /**
     * @brief Tiefe der Tischplatte.
     */
    static constexpr GLfloat DEPTH = 16.0f;

    /**
     * @brief Konstruktor. Erstellt und Initialisiert ein neues GLTablePlate-Objekt.
     * @param name Name des Objekts.
     * @param radius Radius des Objekts.
     * @param color Farbe des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTablePlate(const QString& name, float radius = 1.0f, const GLColorRgba& color = GLColorRgba::clBlue, const QString textureFile = ":/textures/Table.png");

    /**
     * @brief Destruktor. Löscht das Texturobjekt der Tischplatte.
     */
    virtual ~GLTablePlate() Q_DECL_OVERRIDE;
};

#endif // GLTABLEPLATE_H
