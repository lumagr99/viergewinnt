#ifndef GLTABLEPLATE_H
#define GLTABLEPLATE_H

#include "glbody.h"

/**
 * @brief The GLTablePlate class
 *
 * Tischplatte des Vier Gewinnt Spiels.
 *
 */
class GLTablePlate : public GLBody {

public:
    /**
     * @brief GLTablePlate
     *
     * Erstellt und Initialisiert ein neues GLTablePlate-Objekt.
     *
     * @param name Name des Objekts.
     * @param radius Radius des Objekts.
     * @param color Farbe des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLTablePlate(const QString& name, float radius = 1.0f, const GLColorRgba& color = GLColorRgba::clBlue, const QString textureFile = ":/textures/Table.png");

    /**
     * @brief ~GLTablePlate
     *
     * Destruktor. Löscht das Texturobjekt der Tischplatte.
     */
    virtual ~GLTablePlate() Q_DECL_OVERRIDE;
};

#endif // GLTABLEPLATE_H
